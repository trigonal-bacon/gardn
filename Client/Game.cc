#include <Client/Game.hh>

#include <Client/Render/RenderEntity.hh>
#include <Shared/StaticData.hh>

#include <iostream>

Game *game = nullptr;
static double g_last_time = 0;

Game::Game() {
    game = this;
    socket.connect("ws://localhost:9001");
}

uint8_t Game::alive() {
    return simulation_ready
    && simulation.ent_exists(camera_id)
    && simulation.ent_alive(simulation.get_ent(camera_id).player);
}

uint8_t Game::in_game() {
    return simulation_ready && on_game_screen
    && simulation.ent_exists(camera_id);
}

void Game::tick(double time) {
    renderer.reset_transform();
    simulation.tick();
    simulation.tick_lerp(time - g_last_time);
    g_last_time = time;
    renderer.context.reset();
    renderer.reset_transform();

    if (alive()) on_game_screen = 1;
    if (in_game()) render_game();
    else render_title_screen();

    //render_ui();
    
    if (socket.ready) send_inputs();

    //clearing operations
    simulation.post_tick();
    input.keys_pressed_this_tick.clear();
    input.mouse_buttons_pressed = input.mouse_buttons_released = 0;
    input.prev_mouse_x = input.mouse_x;
    input.prev_mouse_y = input.mouse_y;
}

void Game::render_game() {
    RenderContext context(&renderer);
    assert(simulation.ent_exists(camera_id));
    Entity &camera = simulation.get_ent(camera_id);
    renderer.translate(renderer.width / 2, renderer.height / 2);
    renderer.scale(scale * camera.fov);
    renderer.translate(-camera.camera_x, -camera.camera_y);
    uint32_t alpha = (uint32_t)(camera.fov * 255 * 0.2) << 24;
    {
        RenderContext context(&renderer);
        renderer.reset_transform();
        renderer.set_fill(0xff1ea761);
        renderer.fill_rect(0,0,renderer.width,renderer.height);
        renderer.set_fill(alpha);
        renderer.fill_rect(0,0,renderer.width,renderer.height);
    }
    {
        RenderContext context(&renderer);
        renderer.set_fill(0xff1ea761);
        renderer.fill_rect(0,0,ARENA_WIDTH,ARENA_HEIGHT);
        renderer.set_stroke(alpha);
        renderer.set_line_width(1);
        float _scale = camera.fov * scale;
        float leftX = camera.camera_x - renderer.width / (2 * _scale);
        float rightX = camera.camera_x + renderer.width / (2 * _scale);
        float topY = camera.camera_y - renderer.height / (2 * _scale);
        float bottomY = camera.camera_y + renderer.height / (2 * _scale);
        float newLeftX = ceilf(leftX / 50) * 50;
        float newTopY = ceilf(topY / 50) * 50;
        renderer.begin_path();
        for (; newLeftX < rightX; newLeftX += 50)
        {
            renderer.move_to(newLeftX, topY);
            renderer.line_to(newLeftX, bottomY);
        }
        for (; newTopY < bottomY; newTopY += 50)
        {
            renderer.move_to(leftX, newTopY);
            renderer.line_to(rightX, newTopY);
        }
        renderer.stroke();
    }
    for (uint32_t i = 0; i < simulation.active_entities.length; ++i) {
        Entity &ent = simulation.get_ent(simulation.active_entities[i]);
        if (ent.has_component(kWeb)) {
            RenderContext context(&renderer);
            renderer.translate(ent.x, ent.y);
            renderer.rotate(ent.angle);
            render_web(renderer, ent);
        }
    }
    for (uint32_t i = 0; i < simulation.active_entities.length; ++i) {
        Entity &ent = simulation.get_ent(simulation.active_entities[i]);
        if (ent.has_component(kDrop)) {
            RenderContext context(&renderer);
            renderer.translate(ent.x, ent.y);
            render_drop(renderer, ent);
        }
    }
    for (uint32_t i = 0; i < simulation.active_entities.length; ++i) {
        Entity &ent = simulation.get_ent(simulation.active_entities[i]);
        if (ent.has_component(kHealth)) {
            RenderContext context(&renderer);
            renderer.translate(ent.x, ent.y);
            render_health(renderer, ent);
        }
    }
    for (uint32_t i = 0; i < simulation.active_entities.length; ++i) {
        Entity &ent = simulation.get_ent(simulation.active_entities[i]);
        if (ent.has_component(kPetal)) {
            RenderContext context(&renderer);
            renderer.translate(ent.x, ent.y);
            renderer.rotate(ent.angle);
            render_petal(renderer, ent);
        }
    }
    for (uint32_t i = 0; i < simulation.active_entities.length; ++i) {
        Entity &ent = simulation.get_ent(simulation.active_entities[i]);
        if (ent.has_component(kMob)) {
            RenderContext context(&renderer);
            renderer.translate(ent.x, ent.y);
            renderer.rotate(ent.angle);
            render_mob(renderer, ent);
        }
    }
    for (uint32_t i = 0; i < simulation.active_entities.length; ++i) {
        Entity &ent = simulation.get_ent(simulation.active_entities[i]);
        if (ent.has_component(kFlower)) {
            RenderContext context(&renderer);
            renderer.translate(ent.x, ent.y);
            renderer.rotate(ent.angle);
            render_flower(renderer, ent);
        }
    }
}

void Game::render_title_screen() {
    RenderContext context(&renderer);
    renderer.reset_transform();
    renderer.set_fill(0xff1ea761);
    renderer.fill_rect(0,0,renderer.width,renderer.height);
}
void Game::on_message(uint8_t *ptr, uint32_t len) {
    Reader reader(ptr);
    switch(reader.read_uint8()) {
        case kClientbound::kClientUpdate: {
            simulation_ready = 1;
            camera_id = reader.read_entid();
            EntityID curr_id = reader.read_entid();
            while(1) {
                if (curr_id.null()) break;
                assert(simulation.ent_exists(curr_id));
                Entity &ent = simulation.get_ent(curr_id);
                simulation.delete_ent(curr_id);
                curr_id = reader.read_entid();
            }
            curr_id = reader.read_entid();
            assert(!curr_id.null());
            while(1) {
                if (curr_id.null()) break;
                uint8_t create = reader.read_uint8();
                if (create) simulation.force_alloc_ent(curr_id);
                assert(simulation.ent_exists(curr_id));
                Entity &ent = simulation.get_ent(curr_id);
                ent.read(&reader);
                curr_id = reader.read_entid();
            }
            break;
        }
        default:
            break;
    }
}

void Game::send_inputs() {
    uint8_t packet[1024];
    Writer writer(static_cast<uint8_t *>(packet));
    if (alive()) {
        writer.write_uint8(kServerbound::kClientInput);
        //float x = input.keys_pressed.contains('D') - input.keys_pressed.contains('A');
        //float y = input.keys_pressed.contains('S') - input.keys_pressed.contains('W');
        float x = (input.mouse_x - renderer.width / 2) / scale;
        float y = (input.mouse_y - renderer.height / 2) / scale;
        writer.write_float(x);
        writer.write_float(y);
        uint8_t attack = input.keys_pressed.contains('\x20') || BIT_AT(input.mouse_buttons_state, 0);
        uint8_t defend = input.keys_pressed.contains('\x10') || BIT_AT(input.mouse_buttons_state, 1);
        writer.write_uint8(attack | (defend << 1));
        socket.send(writer.packet, writer.at - writer.packet);
    } else {
        if (input.keys_pressed_this_tick.contains('\x20')) {
            if (on_game_screen == 0) {
                writer.write_uint8(kServerbound::kClientSpawn);
                socket.send(writer.packet, writer.at - writer.packet);
            }
            on_game_screen = 0;
        }
    }
}