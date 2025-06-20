#include <Client/Render/RenderEntity.hh>

#include <Client/Render/Renderer.hh>

#include <Client/Assets/Assets.hh>

#include <Client/Particle.hh>

#include <Shared/Entity.hh>
#include <Shared/StaticData.hh>

void render_petal(Renderer &ctx, Entity const &ent) {
    ctx.scale((ent.radius / 2) / PETAL_DATA[ent.petal_id].radius);
    draw_static_petal_single(ent.petal_id, ctx);
    if (PETAL_DATA[ent.petal_id].rarity == RarityID::kUnique && frand() < 0.25)
        Particle::add_unique_particle(ent.x, ent.y);
}