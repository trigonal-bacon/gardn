#include <Client/Render/RenderEntity.hh>

#include <Client/Assets/Assets.hh>
#include <Client/Render/Renderer.hh>

#include <Client/Game.hh>

#include <Shared/Entity.hh>

#include <iostream>

void render_flower(Renderer &ctx, Entity const &ent) {
    if (ent.has_component(kMob)) return;
    uint8_t face_flags = ent.face_flags;
    if (ent.deletion_animation > 0) BIT_SET(face_flags, FaceFlags::kDeadEyes);
    draw_static_flower(ctx, {
        .radius = ent.radius,
        .eye_x = ent.eye_x,
        .eye_y = ent.eye_y,
        .mouth = ent.mouth,
        .cutter_angle = (float) (Game::timestamp / 200),
        .face_flags = face_flags,
        .equip_flags = ent.equip_flags,
        .color = ent.color
    });
}