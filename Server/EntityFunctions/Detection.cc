#include <Server/EntityFunctions.hh>

#include <Shared/Simulation.hh>
#include <Shared/Entity.hh>

EntityID find_nearest_enemy(Simulation *simulation, Entity const &entity, float radius) {
    if ((entity.id.id - entity.lifetime) % (TPS / 5) != 0) return NULL_ENTITY;
    if (entity.immunity_ticks > 0) return NULL_ENTITY;
    EntityID ret;
    float min_dist = radius;
    simulation->spatial_hash.query(entity.x, entity.y, radius, radius, [&](Simulation *sim, Entity &ent){
        if (!sim->ent_alive(ent.id)) return;
        if (ent.team == entity.team) return;
        if (ent.immunity_ticks > 0) return;
        if (!ent.has_component(kMob) && !ent.has_component(kFlower)) return;
        if (sim->ent_alive(entity.parent)) {
            Entity &parent = sim->get_ent(entity.parent);
            float dist = Vector(ent.x-parent.x,ent.y-parent.y).magnitude();
            if (dist > SUMMON_RETREAT_RADIUS) return;
        }
        float dist = Vector(ent.x-entity.x,ent.y-entity.y).magnitude();
        if (dist < min_dist) { min_dist = dist; ret = ent.id; }
    });
    return ret;
}