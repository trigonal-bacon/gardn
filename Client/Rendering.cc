#include <Client/Game.hh>

#include <Client/Ui/Ui.hh>
#include <Client/Render/RenderEntity.hh>
#include <Shared/StaticData.hh>
#include <Shared/Vector.hh>

#include <iostream>

void Game::render_game() {
    RenderContext context(&renderer);
    assert(simulation.ent_exists(camera_id));
    Entity &camera = simulation.get_ent(camera_id);
    renderer.translate(renderer.width / 2, renderer.height / 2);
    renderer.scale(Ui::scale * camera.fov);
    renderer.translate(-camera.camera_x, -camera.camera_y);
    if (alive()) {
        Entity &player = simulation.get_ent(player_id);
        Game::loadout_count = player.loadout_count;
        if (player.damaged) {
            Vector rand = Vector::rand(6);
            renderer.translate(rand.x, rand.y);
        }
    }
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

    simulation.for_each<kWeb>([](Simulation *sim, Entity &ent){
        RenderContext context(&renderer);
        renderer.translate(ent.x, ent.y);
        renderer.rotate(ent.angle);
        render_web(renderer, ent);
    });
    simulation.for_each<kDrop>([](Simulation *sim, Entity &ent){
        RenderContext context(&renderer);
        renderer.translate(ent.x, ent.y);
        render_drop(renderer, ent);
    });
    simulation.for_each<kHealth>([](Simulation *sim, Entity &ent){
        RenderContext context(&renderer);
        renderer.translate(ent.x, ent.y);
        render_health(renderer, ent);
    });
    simulation.for_each<kPetal>([](Simulation *sim, Entity &ent){
        RenderContext context(&renderer);
        renderer.translate(ent.x, ent.y);
        renderer.rotate(ent.angle);
        render_petal(renderer, ent);
    });
    simulation.for_each<kMob>([](Simulation *sim, Entity &ent){
        if (ent.mob_id != MobID::kAntHole) return;
        RenderContext context(&renderer);
        renderer.translate(ent.x, ent.y);
        renderer.rotate(ent.angle);
        render_mob(renderer, ent);
    });
    simulation.for_each<kMob>([](Simulation *sim, Entity &ent){
        if (ent.mob_id == MobID::kAntHole) return;
        RenderContext context(&renderer);
        renderer.translate(ent.x, ent.y);
        renderer.rotate(ent.angle);
        render_mob(renderer, ent);
    });
    simulation.for_each<kFlower>([](Simulation *sim, Entity &ent){
        RenderContext context(&renderer);
        renderer.translate(ent.x, ent.y);
        renderer.rotate(ent.angle);
        render_flower(renderer, ent);
    });
    simulation.for_each<kName>([](Simulation *sim, Entity &ent){
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