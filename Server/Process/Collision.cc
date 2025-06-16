#include <Server/EntityFunctions.hh>

#include <Shared/Simulation.hh>
#include <Shared/Entity.hh>

#include <cmath>

static bool should_interact(Entity const &ent1, Entity const &ent2) {
    //if (ent1.has_component(kFlower) || ent2.has_component(kFlower)) return false;
    //if (ent1.has_component(kPetal) || ent2.has_component(kPetal)) return false;
    if (ent1.pending_delete || ent2.pending_delete) return false;
    if (!(ent1.team == ent2.team)) return true;
    if (BIT_AT((ent1.flags | ent2.flags), EntityFlags::kNoFriendlyCollision)) return false;
    //if (ent1.has_component(kPetal) || ent2.has_component(kPetal)) return false;
    if (ent1.has_component(kMob) && ent2.has_component(kMob)) return true;
    return false;
}

static void pickup_drop(Simulation *sim, Entity &player, Entity &drop) {
    if (!sim->ent_alive(player.parent)) return;
    if (drop.immunity_ticks > 0) return;

    for (uint32_t i = 0; i <  player.loadout_count + MAX_SLOT_COUNT; ++i) {
        if (player.loadout_ids[i] != PetalID::kNone) continue;
        player.set_loadout_ids(i, drop.drop_id);
        drop.set_x(player.x);
        drop.set_y(player.y);
        BIT_UNSET(drop.flags, EntityFlags::kIsDespawning);
        sim->request_delete(drop.id);
        //peaceful transfer, no petal tracking needed
        return;
    }
}

#define NO(component) (!ent1.has_component(component) && !ent2.has_component(component))

void on_collide(Simulation *sim, Entity &ent1, Entity &ent2) {
    //do a distance dependent check first (it's faster)
    float min_dist = ent1.radius + ent2.radius;
    if (fabs(ent1.x - ent2.x) > min_dist || fabs(ent1.y - ent2.y) > min_dist) return;
    //check if collide (distance independent)
    if (!should_interact(ent1, ent2)) return;
    //finer distance check
    Vector separation(ent1.x - ent2.x, ent1.y - ent2.y);
    float dist = min_dist - separation.magnitude();
    if (dist < 0) return;
    if (NO(kDrop) && NO(kWeb)) {
        if (separation.x == 0 && separation.y == 0)
            separation.unit_normal(frand() * 2 * M_PI);
        separation.normalize();
        float ratio = ent2.mass / (ent1.mass + ent2.mass);
        Vector sep = separation;
        if (!(ent1.team == ent2.team) && (ent1.has_component(kFlower) || ent2.has_component(kFlower))) {
            float scale = PLAYER_ACCELERATION * 2;
            Vector norm_sep = separation * scale * ratio;
            ent1.collision_velocity += norm_sep;
            ent1.velocity += norm_sep * 2;
            norm_sep = separation * scale * (ratio - 1);
            ent2.collision_velocity += norm_sep;
            ent2.velocity += norm_sep * 2;
        }
        sep *= ratio * dist;
        if (ratio > 0.01) ent1.collision_velocity += sep;
        sep = separation;
        sep *= (ratio - 1) * dist;
        if (1 - ratio > 0.01) ent2.collision_velocity += sep;
    }

    if (ent1.has_component(kHealth) && ent2.has_component(kHealth) && !(ent1.team == ent2.team)) {
        if (ent1.health > 0 && ent2.health > 0) {
            inflict_damage(sim, ent1.id, ent2.id, ent1.damage, DamageType::kContact);
            inflict_damage(sim, ent2.id, ent1.id, ent2.damage, DamageType::kContact);
        }
    }

    if (ent1.has_component(kDrop) && ent2.has_component(kFlower)) {
        pickup_drop(sim, ent2, ent1);
    } else if (ent2.has_component(kDrop) && ent1.has_component(kFlower)) {
        pickup_drop(sim, ent1, ent2);
    }

    if (ent1.has_component(kWeb) && !ent2.has_component(kPetal) && !ent2.has_component(kDrop))
        ent2.speed_ratio = 0.5;
    else if (ent2.has_component(kWeb) && !ent1.has_component(kPetal) && !ent1.has_component(kDrop))
        ent1.speed_ratio = 0.5;
}