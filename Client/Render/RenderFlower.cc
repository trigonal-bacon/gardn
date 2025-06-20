#include <Client/Render/RenderEntity.hh>

#include <Client/Assets/Assets.hh>
#include <Client/Render/Renderer.hh>

#include <Client/Game.hh>

#include <Shared/Entity.hh>

#include <iostream>

void render_flower(Renderer &ctx, Entity const &ent) {
    if (ent.has_component(kMob)) return;
    draw_static_flower(ctx, {
        .radius = ent.radius,
        .eye_x = ent.eye_x,
        .eye_y = ent.eye_y,
        .mouth = ent.mouth,
        .cutter_angle = (float) (Game::timestamp / 200),
        .face_flags = ent.face_flags,
        .flags = static_cast<uint8_t>(((ent.deletion_animation > 0) ? 1 : 0) << 1)
    });
}