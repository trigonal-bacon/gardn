#include <Server/Process.hh>

#include <Server/EntityFunctions.hh>
#include <Shared/Entity.hh>
#include <Shared/Simulation.hh>

#include <iostream>

void tick_health_behavior(Simulation& sim, Entity& ent) {
  ent.set_damaged(0);
  if (ent.poison_inflicted.ticks_remaining > 0 && !ent.has_component(kPetal)) {
    ent.poison_inflicted.ticks_remaining--;
    inflict_damage(sim, ent.poison_inflicted.inflicted_by, ent.id, ent.poison_inflicted.damage_per_tick, DamageType::kPoison);
    if (ent.poison_inflicted.ticks_remaining % (TPS / 2) != 0)
      ent.set_damaged(0);
  } else {
    ent.poison_inflicted.ticks_remaining = 0;
    ent.poison_inflicted.inflicted_by = NULL_ENTITY;
  }
  if (ent.sponge_duration > 0) {
    for (auto it = ent.continuous_damage.begin(); it != ent.continuous_damage.end();) {
      if (it->ticks_remaining > 0) {
        inflict_damage(sim, it->inflicted_by, ent.id, it->damage_per_tick, it->damage_type);
        --it->ticks_remaining;
        ++it;
      } else {
        it = ent.continuous_damage.erase(it);
      }
    }
  } else {
    for (const auto& dmg : ent.continuous_damage) {
      ent.set_damaged(1);
      inflict_damage(sim, dmg.inflicted_by, ent.id, dmg.damage_per_tick * dmg.ticks_remaining, dmg.damage_type);
    }
    ent.continuous_damage.clear();
  }
  if (ent.dandy_ticks > 0)
    --ent.dandy_ticks;
  if (ent.health <= 0)
    sim.request_delete(ent.id);
  if (ent.max_health == 0)
    return;
  if (ent.has_component(kFlower))
    ent.set_health_ratio(ent.health / ent.max_health);
  else
    ent.set_health_ratio(1);
}