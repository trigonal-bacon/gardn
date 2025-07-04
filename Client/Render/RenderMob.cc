#include <Client/Render/RenderEntity.hh>

#include <Client/Render/Renderer.hh>

#include <Client/Assets/Assets.hh>

#include <Shared/Entity.hh>
#include <Shared/StaticData.hh>

#include <Client/Game.hh>

#include <cmath>

void render_mob(Renderer &ctx, Entity const &ent) {
    uint32_t flags = 0;
    flags |= (ent.team == Game::camera_id);
    flags |= (ent.is_tail << 1);
    MobRenderAttributes attrs = {ent.animation, ent.radius, ent.id.id, flags};
    if (ent.has_component(kFlower)) {
        attrs.flower_attrs = {
            .radius = ent.radius,
            .eye_x = ent.eye_x,
            .eye_y = ent.eye_y,
            .mouth = ent.mouth,
            .cutter_angle = (float) (Game::timestamp / 200),
            .face_flags = ent.face_flags,
            .flags = static_cast<uint8_t>(1 | ((ent.deletion_animation > 0 ? 1 : 0) << 1))
        };
    }
    draw_static_mob(ent.mob_id, ctx, attrs);
    if (ent.deletion_animation > 0)
        Game::seen_mobs[ent.mob_id] = 1;
}