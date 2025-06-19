#include <Client/Game.hh>

#include <Client/Input.hh>
#include <Client/Particle.hh>

#include <Client/Ui/Extern.hh>
#include <Client/Render/RenderEntity.hh>

#include <Shared/Map.hh>
#include <Shared/StaticData.hh>
#include <Shared/Vector.hh>

#include <cmath>

void Game::render_game() {
    RenderContext context(&renderer);
    DEBUG_ONLY(assert(simulation.ent_exists(camera_id));)
    Entity const &camera = simulation.get_ent(camera_id);
    renderer.translate(renderer.width / 2, renderer.height / 2);
    renderer.scale(Ui::scale * camera.fov);
    renderer.translate(-camera.camera_x, -camera.camera_y);
    if (alive()) {
        Entity const &player = simulation.get_ent(player_id);
        if (player.damaged) {
            Vector rand = Vector::rand(3);
            renderer.translate(rand.x, rand.y);
        }
    }
    uint32_t alpha = (uint32_t)(camera.fov * 255 * 0.2) << 24;
    {
        RenderContext context(&renderer);
        renderer.reset_transform();
        renderer.set_fill(0xff987d72);
        renderer.fill_rect(0,0,renderer.width,renderer.height);
        renderer.set_fill(alpha);
        renderer.fill_rect(0,0,renderer.width,renderer.height);
    }
    {
        RenderContext context(&renderer);
        for (ZoneDefinition const &def : MAP) {
            renderer.set_fill(def.color);
            renderer.fill_rect(def.x - def.w/2,def.y-def.h/2,def.w,def.h);
            if (Map::difficulty_at_level(score_to_level(Game::score)) > def.difficulty) {
                renderer.set_fill(0x40000000);
                renderer.fill_rect(def.x - def.w/2,def.y-def.h/2,def.w,def.h);
            }
        }
        renderer.set_stroke(alpha);
        renderer.set_line_width(0.5);
        float scale = 1 / (2 * camera.fov * Ui::scale);
        float leftX = camera.camera_x - renderer.width * scale;
        float rightX = camera.camera_x + renderer.width * scale;
        float topY = camera.camera_y - renderer.height * scale;
        float bottomY = camera.camera_y + renderer.height * scale;
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

    if (alive() && Input::movement_helper && !Input::keyboard_movement) {
        Entity const &player = simulation.get_ent(player_id);
        float norm_mouse_x = (Input::mouse_x - renderer.width / 2) / Ui::scale;
        float norm_mouse_y = (Input::mouse_y - renderer.height / 2) / Ui::scale;
        Vector delta{norm_mouse_x, norm_mouse_y};
        float dist = delta.magnitude();
        if (dist >= player.radius + 80) {
            RenderContext context(&renderer);
            renderer.reset_transform();
            renderer.translate(renderer.width/2,renderer.height/2);
            renderer.scale(Ui::scale);
            uint8_t alpha = (uint8_t) (fclamp((dist - player.radius - 80) / 80, 0, 1) * 255 * 0.15);
            delta.set_magnitude(player.radius + 40);
            renderer.set_line_width(18);
            renderer.round_line_cap();
            renderer.round_line_join();
            renderer.set_stroke(alpha << 24);
            renderer.begin_path();
            renderer.move_to(delta.x,delta.y);
            renderer.line_to(norm_mouse_x,norm_mouse_y);
            renderer.translate(norm_mouse_x,norm_mouse_y);
            renderer.rotate(delta.angle());
            renderer.rotate(2.5);
            renderer.move_to(0,0);
            renderer.line_to(40,0);
            renderer.rotate(-5);
            renderer.move_to(0,0);
            renderer.line_to(40,0);
            renderer.stroke();
        }
    }

    Particle::tick_game(renderer, Ui::dt);

    simulation.for_each<kWeb>([](Simulation *sim, Entity const &ent){
        RenderContext context(&renderer);
        renderer.translate(ent.x, ent.y);
        renderer.rotate(ent.angle);
        render_web(renderer, ent);
    });
    simulation.for_each<kDrop>([](Simulation *sim, Entity const &ent){
        RenderContext context(&renderer);
        renderer.translate(ent.x, ent.y);
        renderer.rotate(ent.angle);
        render_drop(renderer, ent);
    });
    simulation.for_each<kHealth>([](Simulation *sim, Entity const &ent){
        RenderContext context(&renderer);
        renderer.translate(ent.x, ent.y);
        render_health(renderer, ent);
    });
    simulation.for_each<kPetal>([](Simulation *sim, Entity const &ent){
        RenderContext context(&renderer);
        renderer.translate(ent.x, ent.y);
        renderer.rotate(ent.angle);
        render_petal(renderer, ent);
    });
    simulation.for_each<kMob>([](Simulation *sim, Entity const &ent){
        if (ent.mob_id != MobID::kAntHole) return;
        RenderContext context(&renderer);
        renderer.translate(ent.x, ent.y);
        renderer.rotate(ent.angle);
        render_mob(renderer, ent);
    });
    simulation.for_each<kMob>([](Simulation *sim, Entity const &ent){
        if (ent.mob_id == MobID::kAntHole) return;
        RenderContext context(&renderer);
        renderer.translate(ent.x, ent.y);
        renderer.rotate(ent.angle);
        render_mob(renderer, ent);
    });
    simulation.for_each<kFlower>([](Simulation *sim, Entity const &ent){
        RenderContext context(&renderer);
        renderer.translate(ent.x, ent.y);
        renderer.rotate(ent.angle);
        render_flower(renderer, ent);
    });
    simulation.for_each<kName>([](Simulation *sim, Entity const &ent){
        RenderContext context(&renderer);
        renderer.translate(ent.x, ent.y);
        render_name(renderer, ent);
    });
}

void Game::render_title_screen() {
    RenderContext context(&renderer);
    renderer.reset_transform();
    renderer.set_fill(0xff1ea761);
    renderer.fill_rect(0,0,renderer.width,renderer.height);
}

void Game::process_ui() {
    renderer.reset_transform();
    window.refactor();
    window.render(renderer);
    window.poll_events();
}