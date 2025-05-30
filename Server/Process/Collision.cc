#include <Server/EntityFunctions.hh>

#include <Shared/Simulation.hh>
#include <Shared/Entity.hh>

#include <cmath>

static bool should_interact(Entity const &ent1, Entity const &ent2) {
    //if (ent1.has_component(kFlower) || ent2.has_component(kFlower)) return false;
    //if (ent1.has_component(kPetal) || ent2.has_component(kPetal)) return false;
    if (ent1.pending_delete || ent2.pending_delete) return false;
    if (!(ent1.team == ent2.team)) return true;
    if ((ent1.flags | ent2.flags) & EntityFlags::NoFriendlyCollision) return false;
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
    if (drop.immunity_ticks < 0.5 * TPS) return;
    //Entity &camera = sim->get_ent(player.parent);
    for (uint32_t i = 0; i < MAX_SLOT_COUNT; ++i) {
        if (player.loadout_ids[i] != PetalID::kNone) continue;
        //no need since we reset in player behavior
        //player.loadout[i].reset();
        //player.loadout[i].id = drop.drop_id;
        player.set_loadout_ids(i, drop.drop_id);
        drop.set_x(player.x);
        drop.set_y(player.y);
        sim->request_delete(drop.id);
        return;
    }
    for (uint32_t i = MAX_SLOT_COUNT; i < player.loadout_count + MAX_SLOT_COUNT; ++i) {
        if (player.loadout_ids[i] != PetalID::kNone) continue;
        player.set_loadout_ids(i, drop.drop_id);
        drop.set_x(player.x);
        drop.set_y(player.y);
        sim->request_delete(drop.id);
        return;
    }
}

#define NO(component) (!ent1.has_component(component) && !ent2.has_component(component))

void on_collide(Simulation *sim, Entity &ent1, Entity &ent2) {
    //check if collide (distance independent)
    if (!should_interact(ent1, ent2)) return;
    //distance dependent check
    Vector separation(ent1.x - ent2.x, ent1.y - ent2.y);
    float dist = ent1.radius + ent2.radius - separation.magnitude();
    if (dist < 0) return;
    if (NO(kDrop) && NO(kWeb)) {
        if (separation.x == 0 && separation.y == 0) separation.unit_normal(frand() * 2 * 3.14159);
        separation.normalize();
        float ratio = ent2.mass / (ent1.mass + ent2.mass);
        Vector sep = separation;
        if (!(ent1.team == ent2.team)) {
            float scale = PLAYER_ACCELERATION * 5;
            Vector norm_sep = separation * scale * ratio;
            ent1.velocity += norm_sep;
            norm_sep = separation * scale * (ratio - 1);
            ent2.velocity += norm_sep;
        }
        sep *= ratio * dist;
        ent1.collision_velocity += sep;
        sep = separation;
        sep *= (ratio - 1) * dist;
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

    if (ent1.has_component(kWeb) && !ent2.has_component(kPetal) && !ent2.has_component(kDrop)) {
        ent2.speed_ratio = 0.8;
    } else if (ent2.has_component(kWeb) && !ent1.has_component(kPetal) && !ent1.has_component(kDrop)) {
        ent1.speed_ratio = 0.8;
    }
}