#include <Client/Render/RenderEntity.hh>

#include <Client/Render/Renderer.hh>

#include <Client/Assets/Assets.hh>

#include <Client/Particle.hh>

#include <Shared/Entity.hh>
#include <Shared/StaticData.hh>

void render_petal(Renderer &ctx, Entity const &ent) {
    //ctx.set_fill(0x20000000);
    //ctx.begin_path();
    //ctx.arc(0,0,ent.radius);
    //ctx.fill();
    ctx.set_global_alpha(1 - ent.deletion_animation);
    ctx.scale(1 + 0.5 * ent.deletion_animation);
    ctx.add_color_filter(0xffac0000, ent.damage_flash * 0.8);
    ctx.scale((ent.radius / 2) / PETAL_DATA[ent.petal_id].radius);
    draw_static_petal_single(ent.petal_id, ctx);
    if (PETAL_DATA[ent.petal_id].rarity == RarityID::kUnique && frand() < 0.25)
        Particle::add_unique_particle(ent.x, ent.y);
}