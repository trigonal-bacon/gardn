#include <Client/Render/RenderEntity.hh>

#include <Client/Render/Renderer.hh>

#include <Client/Assets/Assets.hh>

#include <Shared/Entity.hh>
#include <Shared/StaticData.hh>

#include <Client/Game.hh>

#include <cmath>

void render_mob(Renderer &ctx, Entity const &ent) {
    uint32_t flags = 0;
    if (ent.get_team() == Game::simulation.get_ent(Game::camera_id).get_team()) BitMath::set(flags, 0);
    if (ent.has_component(kSegmented)) BitMath::set(flags, 1);
    MobRenderAttributes attrs = {ent.animation, ent.get_radius(), ent.id.id, flags, ent.get_color()};
    if (ent.has_component(kFlower)) {
        attrs.flower_attrs = {
            .radius = ent.get_radius(),
            .eye_x = ent.eye_x,
            .eye_y = ent.eye_y,
            .mouth = ent.mouth,
            .cutter_angle = (float) (Game::timestamp / 200),
            .face_flags = ent.get_face_flags(),
            .equip_flags = ent.get_equip_flags(),
            .color = ent.get_color()
        };
    }
    draw_static_mob(ent.get_mob_id(), ctx, attrs);
    if (ent.deletion_animation > 0)
        Game::seen_mobs[ent.get_mob_id()] = 1;
    /*
    #ifdef DEBUG
    ctx.set_stroke(0x80ff0000);
    ctx.set_line_width(1);
    ctx.begin_path();
    ctx.arc(0,0,MOB_DATA[ent.get_mob_id()].attributes.aggro_radius);
    ctx.stroke();
    #endif
    */
}