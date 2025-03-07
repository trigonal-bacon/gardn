#include <Server/EntityFunctions.hh>

#include <Shared/Simulation.hh>
#include <Shared/Entity.hh>

#include <cmath>

static bool should_interact(Entity const &ent1, Entity const &ent2) {
    //if (ent1.has_component(kFlower) || ent2.has_component(kFlower)) return false;
    //if (ent1.has_component(kPetal) || ent2.has_component(kPetal)) return false;
    if (ent1.pending_delete || ent2.pending_delete) return false;
    if (!(ent1.team == ent2.team)) return true;
    //if (ent1.has_component(kPetal) || ent2.has_component(kPetal)) return false;
    if (ent1.has_component(kMob) && ent2.has_component(kMob)) return true;
    return false;
}

//static bool should_collide(Entity &ent1, Entity &ent2) {
    //if (ent1.has_component(kDrop) || ent2.has_component(kDrop)) return false;
    //return true;
//}

static void pickup_drop(Simulation *sim, Entity &player, Entity &drop) {
    if (!sim->ent_alive(player.parent)) return;
    if (drop.despawn_tick < 0.5 * TPS) return;
    Entity &camera = sim->get_ent(player.parent);

    for (uint32_t i = 0; i < MAX_SLOT_COUNT; ++i) {
        if (camera.loadout[i].id != PetalID::kNone) continue;
        camera.loadout[i].reset();
        camera.loadout[i].id = drop.drop_id;
        drop.set_x(player.x);
        drop.set_y(player.y);
        sim->request_delete(drop.id);
        return;
    }
    for (uint32_t i = MAX_SLOT_COUNT; i < camera.loadout_count + MAX_SLOT_COUNT; ++i) {
        if (camera.loadout_ids[i] != PetalID::kNone) continue;
        camera.set_loadout_ids(i, drop.drop_id);
        drop.set_x(player.x);
        drop.set_y(player.y);
        sim->request_delete(drop.id);
        return;
    }
}

void on_collide(Simulation *sim, Entity &ent1, Entity &ent2) {
    //check if collide (distance independent)
    if (!should_interact(ent1, ent2)) return;
    //distance dependent check
    Vector separation(ent1.x - ent2.x, ent1.y - ent2.y);
    float dist = ent1.radius + ent2.radius - separation.magnitude();
    if (dist < 0) return;
    if (!ent1.has_component(kDrop) && !ent2.has_component(kDrop)) {
        if (separation.x == 0 && separation.y == 0) separation.unit_normal(frand() * 2 * 3.14159);
        separation.normalize();
        float ratio = ent2.mass / (ent1.mass + ent2.mass);
        float bounce_factor = 0.05;
        Vector sep = separation;
        sep *= ratio * dist * (1 + bounce_factor);
        ent1.collision_velocity += sep;
        sep = separation;
        sep *= (ratio - 1) * dist * (1 + bounce_factor);
        ent2.collision_velocity += sep;
    }

    if (ent1.has_component(kHealth) && ent2.has_component(kHealth) && !(ent1.team == ent2.team)) {
        if (ent1.health > 0 && ent2.health > 0) {
            inflict_damage(sim, ent1.id, ent2, ent1.damage);
            inflict_damage(sim, ent2.id, ent1, ent2.damage);
        }
    }

    if (ent1.has_component(kDrop) && ent2.has_component(kFlower)) {
        pickup_drop(sim, ent2, ent1);
    } else if (ent2.has_component(kDrop) && ent1.has_component(kFlower)) {
        pickup_drop(sim, ent1, ent2);
    }
}