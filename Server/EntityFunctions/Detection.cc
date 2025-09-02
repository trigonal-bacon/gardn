#include <Server/EntityFunctions.hh>

#include <Shared/Simulation.hh>
#include <Shared/Entity.hh>

EntityID find_nearest_enemy(Simulation *simulation, Entity const &entity, float radius) {
    if ((entity.id.id - entity.lifetime) % (TPS / 5) != 0) return NULL_ENTITY;
    if (entity.immunity_ticks > 0) return NULL_ENTITY;
    EntityID ret;
    float min_dist = radius;
    simulation->spatial_hash.query(entity.get_x(), entity.get_y(), radius, radius, [&](Simulation *sim, Entity &ent){
        if (!sim->ent_alive(ent.id)) return;
        if (ent.get_team() == entity.get_team()) return;
        if (ent.immunity_ticks > 0) return;
        if (!ent.has_component(kMob) && !ent.has_component(kFlower)) return;
        if (sim->ent_alive(entity.get_parent())) {
            Entity &parent = sim->get_ent(entity.get_parent());
            float dist = Vector(ent.get_x()-parent.get_x(),ent.get_y()-parent.get_y()).magnitude();
            if (dist > SUMMON_RETREAT_RADIUS) return;
        }
        float dist = Vector(ent.get_x()-entity.get_x(),ent.get_y()-entity.get_y()).magnitude();
        if (dist < min_dist) { min_dist = dist; ret = ent.id; }
    });
    return ret;
}