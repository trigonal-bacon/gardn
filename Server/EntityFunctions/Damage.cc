#include <Server/EntityFunctions.hh>

#include <Server/Spawn.hh>
#include <Shared/Entity.hh>
#include <Shared/Simulation.hh>

#include <cmath>

static bool _yggdrasil_revival_clause(Simulation *sim, Entity &player) {
    if (BitMath::at(player.flags, EntityFlags::kZombie)) return false;
    for (uint32_t i = 0; i < player.get_loadout_count(); ++i) {
        if (!player.loadout[i].already_spawned) continue;
        if (player.loadout[i].get_petal_id() != PetalID::kYggdrasil) continue;
        if (frand() > 0.5) continue;
        return true;
    }
    return false;
}

void inflict_damage(Simulation *sim, EntityID const atk_id, EntityID const def_id, float amt, uint8_t type) {
    if (amt <= 0) return;
    if (!sim->ent_alive(def_id)) return;
    Entity &defender = sim->get_ent(def_id);
    if (!defender.has_component(kHealth)) return;
    // Invincibility for dev players
    if (defender.has_component(kFlower) && defender.get_dev()) return;
    if (defender.has_component(kPetal) && defender.get_petal_id() == PetalID::kBasil) return;
    DEBUG_ONLY(assert(!defender.pending_delete);)
    DEBUG_ONLY(assert(defender.has_component(kHealth));)
    if (defender.immunity_ticks > 0) return;
    if (type == DamageType::kContact) amt -= defender.armor;
    else if (type == DamageType::kPoison) amt -= defender.poison_armor;
    if (amt <= 0) return;
    //if (amt <= defender.armor) return;
    float old_health = defender.health;
    defender.set_damaged(1);
    defender.health = fclamp(defender.health - amt, 0, defender.health);  
    float damage_dealt = old_health - defender.health;
    //ant hole spawns
    //floor start, ceil end
    if (defender.has_component(kMob) && defender.get_mob_id() == MobID::kAntHole) {
        uint32_t const num_waves = ANTHOLE_SPAWNS.size() - 1;
        uint32_t start = ceilf((defender.max_health - old_health) / defender.max_health * num_waves);
        uint32_t end = ceilf((defender.max_health - defender.health) / defender.max_health * num_waves);
        if (defender.health <= 0) end = num_waves + 1;
        for (uint32_t i = start; i < end; ++i) {
            for (MobID::T mob_id : ANTHOLE_SPAWNS[i]) {
                Entity &child = alloc_mob(sim, mob_id, defender.get_x(), defender.get_y(), defender.get_team());
                child.set_parent(defender.id);
                child.target = defender.target;
            }
        }
    }
    // yggdrasil revive clause
    if (defender.health == 0 && defender.has_component(kFlower)) {
        if (_yggdrasil_revival_clause(sim, defender)) {
            defender.set_revived(1);
            defender.health = defender.max_health;
            defender.poison_ticks = 0;
            defender.slow_ticks = 0;
            defender.dandy_ticks = 0;
            defender.immunity_ticks = 1.0 * TPS;
        }
    }
    if (!sim->ent_exists(atk_id)) return;
    Entity &attacker = sim->get_ent(atk_id);

    if (type != DamageType::kReflect && defender.damage_reflection > 0)
        inflict_damage(sim, def_id, attacker.base_entity, damage_dealt * defender.damage_reflection, DamageType::kReflect);

    if (type == DamageType::kContact && defender.get_revived() == 0) {
        if (defender.poison_ticks < attacker.poison_damage.time * TPS) {
            defender.poison_ticks = attacker.poison_damage.time * TPS;
            defender.poison_inflicted = attacker.poison_damage.damage / TPS;
            defender.poison_dealer = attacker.base_entity;
        }

        if (defender.slow_ticks < attacker.slow_inflict)
            defender.slow_ticks = attacker.slow_inflict;

        if (attacker.has_component(kPetal) &&
            attacker.get_petal_id() == PetalID::kDandelion &&
            defender.dandy_ticks < 10 * TPS)
            defender.dandy_ticks = 10 * TPS;
    }

    if (!sim->ent_alive(defender.target)) {
        if (attacker.has_component(kPetal) && sim->ent_alive(attacker.get_parent()))
            defender.target = attacker.get_parent();
        else if (sim->ent_alive(atk_id))
            defender.target = atk_id;
    }
    defender.last_damaged_by = attacker.base_entity;
}

void inflict_heal(Simulation *sim, Entity &ent, float amt) {
    DEBUG_ONLY(assert(ent.has_component(kHealth));)
    if (ent.pending_delete || ent.health <= 0) return;
    if (ent.dandy_ticks > 0) return;
    if (BitMath::at(ent.flags, EntityFlags::kZombie)) return;
    if (ent.has_component(kFlower)) {
        uint32_t basil_count = 0;
        for (uint32_t i = 0; i < ent.get_loadout_count(); ++i) {
            auto const &slot = ent.loadout[i];
            if (slot.get_petal_id() != PetalID::kBasil) continue;
            if (slot.already_spawned) ++basil_count;
        }
        if (basil_count > 0) {
            float h = 0.0f;
            for (uint32_t n = 1; n <= basil_count; ++n) h += 1.0f / (float)n;
            amt *= (1.0f + 0.35f * h);
        }
    }
    ent.health = fclamp(ent.health + amt, 0, ent.max_health);
}