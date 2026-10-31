#include <Server/Simulation.hh>

#include <Server/Client.hh>
#include <Server/EntityFunctions.hh>
#include <Server/Process.hh>
#include <Server/Server.hh>
#include <Server/SpatialHash.hh>
#include <Server/Spawn.hh>

#include <Shared/Map.hh>

#include <algorithm>
#include <functional>
#include <vector>

static void calculate_leaderboard(Simulation& sim) {
  std::vector<std::reference_wrapper<Entity>> players;
  sim.for_each<kCamera>([&](Simulation& sim, Entity& ent) {
    if (sim.ent_alive(ent.get_player()))
      players.push_back(sim.get_ent(ent.get_player()));
  });
  std::stable_sort(players.begin(), players.end(),
                   [](const Entity& a, const Entity& b) {
                     return a.get_score() > b.get_score();
                   });
  uint32_t num = players.size();
  sim.arena_info.set_player_count(num);
  num = std::min(num, LEADERBOARD_SIZE);
  for (uint32_t i = 0; i < num; ++i) {
    sim.arena_info.set_names(i, players[i].get().get_name());
    sim.arena_info.set_scores(i, players[i].get().get_score());
    sim.arena_info.set_colors(i, players[i].get().get_color());
  }
}

void Simulation::on_tick() {
  spatial_hash.refresh(ARENA_WIDTH, ARENA_HEIGHT);
  if (frand() < 1.0f / TPS) {
    for (uint32_t i = 0; i < 10; ++i) {
      Vector v;
      if (find_spawn_location(500, v))
        spawn_random_mob(v.x, v.y);
    }
  }
  for_each_entity([](Simulation& sim, Entity& ent) {
    if (ent.has_component<kPhysics>())
      sim.spatial_hash.insert(ent);
    if (Bits::at(ent.flags, EntityFlags::kHasCulling))
      Bits::set(ent.flags, EntityFlags::kIsCulled);
  });
  for_each<kCamera>(tick_culling_behavior);
  for_each<kFlower>(tick_player_behavior);
  for_each<kMob>(tick_ai_behavior);
  for_each<kCamera>(tick_player_ai_behavior);
  for_each<kPetal>(tick_petal_behavior);
  for_each<kHealth>(tick_health_behavior);
  spatial_hash.collide(on_collide);
  tick_curse_behavior(*this);
  for_each<kPhysics>(tick_entity_motion);
  for_each<kSegmented>(tick_segment_behavior);
  for_each<kCamera>(tick_camera_behavior);
  for_each<kScore>(tick_score_behavior);
  for_each_entity(entity_clear_references);
  calculate_leaderboard(*this);
}

void Simulation::post_tick() {
  arena_info.reset_protocol();
  for_each_entity([](Simulation& sim, Entity& ent) {
    // no deletions mid tick
    ent.reset_protocol();
    ++ent.lifetime;
    if (Bits::at(ent.flags, EntityFlags::kIsDespawning)) {
      if (ent.despawn_tick == 0)
        sim.request_delete(ent.id);
      else
        --ent.despawn_tick;
    }
    if (ent.immunity_ticks > 0)
      --ent.immunity_ticks;
  });
  for_each_entity([](Simulation& sim, Entity& ent) {
    if (!ent.pending_delete)
      return;
    if (!ent.has_component<kPhysics>())
      return sim._delete_ent(ent.id);
    if (ent.deletion_tick >= TPS / 5)
      return sim._delete_ent(ent.id);
    if (ent.deletion_tick == 0)
      entity_on_death(sim, ent);
    ++ent.deletion_tick;
  });
}

void Simulation::remove_mob(uint32_t zone) {
  DEBUG_ONLY(assert(zone < MAP_DATA.size());)
  --zone_mob_counts[zone];
}

void Simulation::spawn_random_mob(float x, float y) {
  uint32_t zone_id = Map::get_zone_from_pos(x, y);
  const ZoneDefinition& zone = MAP_DATA[zone_id];
  if (zone.density * (zone.right - zone.left) * (zone.bottom - zone.top) /
      (500 * 500) <
      zone_mob_counts[zone_id])
    return;
  float sum = 0;
  for (const SpawnChance& s : zone.spawns)
    sum += s.chance;
  sum *= frand();
  for (const SpawnChance& s : zone.spawns) {
    sum -= s.chance;
    if (sum <= 0) {
      Entity& ent = alloc_mob(*this, s.id, x, y, NULL_ENTITY, [=, this](Entity& mob) {
        mob.zone = zone_id;
        mob.immunity_ticks = TPS;
        Bits::set(mob.flags, EntityFlags::kSpawnedFromZone);
        Bits::set(mob.flags, EntityFlags::kHasCulling);
        this->zone_mob_counts[zone_id]++;
        mob.score_reward = MOB_DATA[mob.get_mob_id()].xp;
      });
      return;
    }
  }
}

bool Simulation::find_spawn_location(float d, Vector& vref) {
  for (uint32_t i = 0; i < 10; ++i) {
    vref.set(frand() * ARENA_WIDTH, frand() * ARENA_HEIGHT);
    bool valid = true;
    for_each<kFlower>([&](Simulation&, Entity& ent) {
      if (ent.has_component<kMob>())
        return;
      if (!valid)
        return;
      if (Vector(ent.get_x() - vref.x, ent.get_y() - vref.y).magnitude() < d)
        valid = false;
    });
    if (valid)
      return true;
  }
  return false;
}
