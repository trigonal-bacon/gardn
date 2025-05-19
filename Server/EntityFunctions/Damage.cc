#include <Server/EntityFunctions.hh>

#include <Server/Spawn.hh>
#include <Shared/Entity.hh>
#include <Shared/Simulation.hh>
#include <Shared/Helpers.hh>

#include <cmath>

static std::vector<std::vector<MobID::T>> anthole_spawns = {
    {MobID::kBabyAnt},
    {MobID::kWorkerAnt,MobID::kBabyAnt},
    {MobID::kWorkerAnt,MobID::kWorkerAnt},
    {MobID::kSoldierAnt},
    {MobID::kBabyAnt,MobID::kSoldierAnt},
    {MobID::kSoldierAnt,MobID::kWorkerAnt,MobID::kWorkerAnt},
    {MobID::kSoldierAnt},
    {MobID::kQueenAnt},
    {MobID::kSoldierAnt,MobID::kWorkerAnt},
    {MobID::kSoldierAnt,MobID::kSoldierAnt,MobID::kSoldierAnt}
};

static uint32_t num_spawn_waves = anthole_spawns.size() - 1;

void inflict_damage(Simulation *sim, EntityID const &atk_id, Entity &defender, float amt) {
    assert(!defender.pending_delete);
    assert(defender.has_component(kHealth));
    if (defender.immunity_ticks > 0) return;
    if (amt <= defender.armor) return;
    float old_health = defender.health;
    defender.set_damaged(1);
    defender.health = fclamp(defender.health - amt, 0, defender.health);   
    //ant hole spawns
    //floor start, ceil end
    if (defender.has_component(kMob) && defender.mob_id == MobID::kAntHole) {
        uint32_t start = (old_health / defender.max_health) * num_spawn_waves;
        uint32_t end = ceilf((defender.health / defender.max_health) * num_spawn_waves);
        for (uint32_t i = start; i + 1 > end; --i) {
            for (MobID::T mob_id : anthole_spawns[num_spawn_waves - i]) {
                Entity &child = alloc_mob(mob_id, defender.x, defender.y, defender.team);
                child.target = defender.target;
            }
        }
    }
    if (!sim->ent_alive(atk_id)) return;
    Entity &attacker = sim->get_ent(atk_id);
    if (defender.poison.time < attacker.poison_damage.time * TPS) {
        defender.poison.time = attacker.poison_damage.time * TPS;
        defender.poison.damage = attacker.poison_damage.damage / TPS;
    }
    if (defender.slow_ticks < attacker.slow_inflict)
        defender.slow_ticks = attacker.slow_inflict;
    if (defender.has_component(kPetal)) {
        switch (defender.petal_id) {
            case PetalID::kDandelion:
                attacker.dandy_ticks = 10 * TPS;
                break;
            default:
                break;
        }
    }
    if (attacker.has_component(kPetal)) {
        if (!sim->ent_alive(defender.target))
            defender.target = attacker.parent;
        defender.last_damaged_by = attacker.parent;
    } else {
        if (!sim->ent_alive(defender.target))
            defender.target = atk_id;
        defender.last_damaged_by = atk_id;
    }
}

void inflict_heal(Simulation *sim, Entity &ent, float amt) {
    assert(ent.has_component(kHealth));
    if (ent.pending_delete || ent.health <= 0) return;
    if (ent.dandy_ticks > 0) return;
    ent.health = fclamp(ent.health + amt, 0, ent.max_health);
}