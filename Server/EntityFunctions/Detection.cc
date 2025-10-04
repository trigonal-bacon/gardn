#include <Server/EntityFunctions.hh>

#include <Shared/Simulation.hh>
#include <Shared/Entity.hh>
#include <Server/Spawn.hh>

static void process_fire_ant_holes(Simulation *sim) {
    sim->for_each<kMob>([&](Simulation *sim2, Entity &hole){
        if (!sim2->ent_alive(hole.id)) return;
        if (hole.get_mob_id() != MobID::kFireAntHole) return;
        if (hole.pending_delete) return;
        if (hole.player_count > 0) {
            ++hole.secondary_reload;
            if (hole.secondary_reload % 3 == 0) {
                Vector rand = Vector::rand(hole.get_radius() * 2);
                Entity &ant = alloc_mob(sim2, MobID::kFireAntSoldier, hole.get_x() + rand.x, hole.get_y() + rand.y, hole.get_team());
                ant.set_parent(hole.get_parent());
                ant.set_color(hole.get_color());
                if (hole.player_count > 0) --hole.player_count;
                if (hole.player_count == 0) {
                    sim2->request_delete(hole.id);
                }
            }
            return;
        }
        bool triggered = false;
        float radius = 100.0f;
        sim2->spatial_hash.query(hole.get_x(), hole.get_y(), radius, radius, [&](Simulation *sim3, Entity &other){
            if (!sim3->ent_alive(other.id)) return;
            if (other.id == hole.id) return;
            if (other.get_team() == hole.get_team()) return;
            if (!other.has_component(kFlower) && !other.has_component(kPetal) && !other.has_component(kMob)) return;
            float d = Vector(other.get_x()-hole.get_x(), other.get_y()-hole.get_y()).magnitude();
            if (d <= radius) triggered = true;
        });
        if (triggered) {
            hole.player_count = 10;        // remaining ants to spawn
            hole.secondary_reload = 0;     // tick counter for pacing
        }
    });
}

EntityID find_nearest_enemy(Simulation *simulation, Entity const &entity, float radius) {
    if ((entity.id.id - entity.lifetime) % (TPS / 5) != 0) { process_fire_ant_holes(simulation); return NULL_ENTITY; }
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
    // Also advance fire ant hole spill-out spawning on this cadence
    process_fire_ant_holes(simulation);
    return ret;
}