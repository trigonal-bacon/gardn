#include <Server/EntityFunctions.hh>

#include <Server/Spawn.hh>
#include <Server/PetalTracker.hh>
#include <Shared/Entity.hh>
#include <Shared/Simulation.hh>

#include <algorithm>
#include <cmath>

static bool _yggdrasil_revival_clause(Simulation& sim, Entity& player) {
  for (uint32_t i = 0; i < player.get_loadout_count(); ++i) {
    if (!player.loadout[i].already_spawned)
      continue;
    if (player.loadout[i].get_petal_id() != PetalID::kYggdrasil)
      continue;
    player.set_loadout_ids(i, PetalID::kNone);
    PetalTracker::remove_petal(sim, PetalID::kYggdrasil);
    return true;
  }
  return false;
}

void inflict_damage(Simulation& sim, const EntityID atk_id,
                    const EntityID def_id, float amt, uint8_t type) {
  if (amt <= 0)
    return;
  if (!sim.ent_alive(def_id))
    return;
  Entity& defender = sim.get_ent(def_id);
  if (!defender.has_component(kHealth))
    return;
  if (defender.immunity_ticks > 0)
    return;
  if (type == DamageType::kContact)
    amt -= defender.armor;
  else if (type == DamageType::kPoison)
    amt -= defender.poison_armor;
  if (amt <= 0)
    return;
  float old_health = defender.health;
  if (defender.sponge_duration > 0 && type == DamageType::kContact) {
    //only contact damage is sponged
    defender.continuous_damage.emplace_back(ContinuousDamage{
        .damage_per_tick = amt / defender.sponge_duration,
        .ticks_remaining = defender.sponge_duration,
        .inflicted_by = atk_id,
        .damage_type = DamageType::kSponge,
    });
  } else {
    if (type != DamageType::kSponge) defender.set_damaged(1);
    defender.health = fclamp(defender.health - amt, 0, defender.health);
  }
  const float damage_dealt = old_health - defender.health;
  // ant hole spawns
  // floor start, ceil end
  if (defender.has_component(kMob) &&
      defender.get_mob_id() == MobID::kAntHole) {
    const uint32_t num_waves = ANTHOLE_SPAWNS.size() - 1;
    uint32_t start = ceilf((defender.max_health - old_health) /
                           defender.max_health * num_waves);
    uint32_t end = ceilf((defender.max_health - defender.health) /
                         defender.max_health * num_waves);
    if (defender.health <= 0)
      end = num_waves + 1;
    for (uint32_t i = start; i < end; ++i) {
      for (MobID::T mob_id : ANTHOLE_SPAWNS[i]) {
        Entity& child =
            alloc_mob(sim, mob_id, defender.get_x(), defender.get_y(),
                      defender.get_team(), [](Entity& mob) {
                        mob.score_reward = MOB_DATA[mob.get_mob_id()].xp;
                        Bits::set(mob.flags, EntityFlags::kHasCulling);
                      });
        child.set_parent(defender.id);
        child.target = defender.target;
      }
    }
  }
  /* yggdrasil revive clause
  if (defender.health == 0 && defender.has_component(kFlower)) {
      if (_yggdrasil_revival_clause(sim, defender)) {
          defender.health = defender.max_health * 0.25;
          defender.immunity_ticks = 1.0 * TPS;
      }
  }
  */
  if (!sim.ent_exists(atk_id))
    return;
  Entity& attacker = sim.get_ent(atk_id);

  if (type != DamageType::kReflect && type != DamageType::kSponge && defender.damage_reflection > 0) {
    inflict_damage(sim, def_id, attacker.base_entity,
                   damage_dealt * defender.damage_reflection,
                   DamageType::kReflect);
  }

  if (!sim.ent_alive(atk_id))
    return;

  if (defender.slow_ticks < attacker.slow_inflict)
    defender.slow_ticks = attacker.slow_inflict;

  if (attacker.has_component(kPetal)) {
    switch (attacker.get_petal_id()) {
    case PetalID::kDandelion:
      defender.dandy_ticks = 10 * TPS;
      break;
    default:
      break;
    }
  }

  if (attacker.has_component(kPetal)) {
    if (!sim.ent_alive(defender.target))
      defender.target = attacker.base_entity;

  } else {
    if (!sim.ent_alive(defender.target))
      defender.target = atk_id;
  }
  defender.last_damaged_by = attacker.base_entity;

  if (type == DamageType::kContact &&
      defender.poison_inflicted.ticks_remaining < attacker.poison_damage.time * TPS) {
    defender.poison_inflicted.ticks_remaining = attacker.poison_damage.time * TPS;
    defender.poison_inflicted.damage_per_tick = attacker.poison_damage.damage / TPS;
    defender.poison_inflicted.inflicted_by = attacker.id;
  }
}

void inflict_lightning(Simulation& sim, const EntityID atk_id,
                      const EntityID def_id, float amt, uint32_t bounces) {
  if (bounces <= 0)
    return;
  if (amt <= 0)
    return;
  if (!sim.ent_alive(def_id))
    return;
  if (!sim.ent_alive(atk_id))
    return;
  const Entity& attacker = sim.get_ent(atk_id);
  std::vector<EntityID> hit_entities = {def_id};
  EntityID last = def_id;
  for (uint32_t i = 0; i < bounces - 1; ++i) {
    EntityID target = find_nearest(sim, sim.get_ent(last), LIGHTNING_BOUNCE_RADIUS, [&](const Entity& ent) {
      if (!sim.ent_alive(ent.id))
        return false;
      if (!ent.has_component(kHealth))
        return false;
      if (ent.has_component(kPetal))
        return false;
      if (ent.get_team() == attacker.get_team())
        return false;
      return std::find(hit_entities.begin(), hit_entities.end(), ent.id) == hit_entities.end();
    });
    if (!sim.ent_alive(target))
      break;
    hit_entities.push_back(target);
    last = target;
  }
  last = NULL_ENTITY;
  for (const EntityID& id : hit_entities) {
    inflict_damage(sim, atk_id, id, amt, DamageType::kLightning);
    const Entity& target = sim.get_ent(id);
    Entity& anim = sim.alloc_ent();
    anim.add_component(kPhysics);
    anim.set_radius(3.5);
    anim.set_x(target.get_x());
    anim.set_y(target.get_y());
    anim.add_component(kRelations);
    anim.set_parent(last);
    anim.add_component(kAnimation);
    entity_set_despawn_tick(anim, 0.1 * TPS);
    Bits::set(anim.flags, EntityFlags::kNoPhysicalCollision);

    last = anim.id;
  }
}

void inflict_heal(Simulation& sim, Entity& ent, float amt) {
  DEBUG_ONLY(assert(ent.has_component(kHealth));)
  if (ent.pending_delete || ent.health <= 0)
    return;
  if (ent.dandy_ticks > 0)
    return;
  ent.health = fclamp(ent.health + amt, 0, ent.max_health);
}