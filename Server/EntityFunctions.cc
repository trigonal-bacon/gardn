#include <Server/EntityFunctions.hh>

#include <Server/Spawn.hh>

#include <Shared/Simulation.hh>
#include <Shared/Entity.hh>
#include <Shared/Helpers.hh>
#include <Shared/StaticData.hh>

#include <vector>

void inflict_damage(Simulation *sim, EntityID const &atk_id, Entity &defender, float amt) {
    assert(!defender.pending_delete);
    assert(defender.has_component(kHealth));
    if (amt <= defender.armor) return;
    defender.set_damaged(1);
    defender.health = fclamp(defender.health - amt, 0, defender.health);
    if (!sim->ent_alive(atk_id)) return;
    Entity &attacker = sim->get_ent(atk_id);
    if (defender.poison.time < attacker.poison_damage.time * TPS) {
        defender.poison.time = attacker.poison_damage.time * TPS;
        defender.poison.damage = attacker.poison_damage.damage / TPS;
    }
    if (defender.has_component(kPetal)) {
        switch (defender.petal_id) {
            case PetalID::kDandelion:
                attacker.dandy_ticks = 10 * TPS;
                break;
            default:
                break;
        }
    }
    if (!sim->ent_alive(defender.target)) {
        if (attacker.has_component(kPetal)) {
            defender.target = attacker.parent;
        } else {
            defender.target = atk_id;
        }
    }
}

void inflict_heal(Simulation *sim, Entity &ent, float amt) {
    assert(ent.has_component(kHealth));
    if (ent.pending_delete || ent.health <= 0) return;
    if (ent.dandy_ticks > 0) return;
    ent.health = fclamp(ent.health + amt, 0, ent.max_health);
}

void entity_on_death(Simulation *sim, Entity &ent) {
    if (ent.has_component(kMob)) {
        if (ent.team != NULL_ENTITY) return;
        struct MobData const &mob_data = MOB_DATA[ent.mob_id];
        std::vector<struct MobDrop> const &drops = mob_data.drops;
        std::vector<uint8_t> success_drops = {};
        for (MobDrop const &d : drops) {
            if (frand() < d.chance) success_drops.push_back(d.id);
        }
        if (success_drops.size() > 1) {
            size_t count = success_drops.size();
            for (size_t i = 0; i < count; ++i) {
                Entity &drop = alloc_drop(success_drops[i]);
                drop.set_x(ent.x);
                drop.set_y(ent.y);
                drop.velocity.unit_normal(i * 2 * M_PI / count).set_magnitude(PLAYER_ACCELERATION * 8.5);
            }
        } else if (success_drops.size() == 1) {
            Entity &drop = alloc_drop(success_drops[0]);
            drop.set_x(ent.x);
            drop.set_y(ent.y);
        }
        //Entity &drop = alloc_drop(PetalID::kTriplet);
        //drop.set_x(ent.x);
        //drop.set_y(ent.y);
    }
}

EntityID find_nearest_enemy(Simulation *simulation, Entity &entity, float radius) {
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