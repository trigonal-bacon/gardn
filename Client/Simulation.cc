#include <Client/Simulation.hh>

#include <Client/Game.hh>

#include <Client/Ui/Extern.hh>

#include <cmath>

void Entity::tick_lerp(float amt) {
  if (has_component<kPhysics>()) {
    float prev_x = get_x();
    float prev_y = get_y();
    if (!pending_delete) {
      get_x_mut().step(amt);
      get_y_mut().step(amt);
    }
    if (has_component<kDrop>() || has_component<kWeb>()) {
      if (lifetime < TPS)
        animation = lerp(animation, 1, amt * 0.75);
      else
        animation = 1;
    } else {
      Vector vel(get_x() - prev_x, get_y() - prev_y);
      animation += (1 + 0.75 * vel.magnitude()) * 0.075;
    }
    get_radius_mut().step(amt);
    get_angle_mut().step_angle(amt);
    if (pending_delete)
      deletion_animation = fclamp(deletion_animation + Ui::dt / 150, 0, 1);
  }
  if (has_component<kCamera>()) {
    get_camera_x_mut().step(amt);
    get_camera_y_mut().step(amt);
    get_fov_mut().step(amt);
    Game::respawn_level = get_respawn_level();
  }
  if (has_component<kHealth>()) {
    get_health_ratio_mut().step(amt);
    if (get_damaged() == 1 && damage_flash < 0.1 && !pending_delete)
      damage_flash = 1;
    else
      damage_flash = fclamp(damage_flash - Ui::dt / 150, 0, 1);
    if (get_damaged())
      last_damaged_time = Game::timestamp;
    get_damaged_mut().clear();
    if ((float) get_health_ratio() > 0.999)
      healthbar_opacity = lerp(healthbar_opacity, 0, amt);
    else
      healthbar_opacity = 1;
    if (healthbar_lag < get_health_ratio())
      healthbar_lag = get_health_ratio();
    else if (Game::timestamp - last_damaged_time > 250)
      healthbar_lag = lerp(healthbar_lag, get_health_ratio(), amt / 3);
  }
  if (has_component<kFlower>()) {
    float eye_angle = get_angle().anchor();
    uint32_t face_flags = get_face_flags();
    eye_x = lerp(eye_x, cosf(eye_angle) * 2, amt);
    eye_y = lerp(eye_y, sinf(eye_angle) * 4, amt);
    if (Bits::at(face_flags, FaceFlags::kAttacking) ||
        Bits::at(face_flags, FaceFlags::kPoisoned) ||
        Bits::at(face_flags, FaceFlags::kDandelioned) || pending_delete)
      mouth = lerp(mouth, 5, amt);
    else if (Bits::at(face_flags, FaceFlags::kDefending))
      mouth = lerp(mouth, 8, amt);
    else
      mouth = lerp(mouth, 15, amt);
  }
}

void Simulation::on_tick() {
  for_each_entity([](Simulation& sim, Entity& ent) { 
    ent.children.clear();
    ent.tick_lerp(Ui::lerp_amount); 
  });

  for_each<kAnimation>([](Simulation& sim, Entity& ent) {
    if (sim.ent_exists(ent.get_parent())) {
      Entity& parent = sim.get_ent(ent.get_parent());
      parent.children.push_back(ent.id);
    }
  });

  for (uint32_t i = 0; i < std::min(arena_info.player_count, LEADERBOARD_SIZE);
       ++i)
    arena_info.scores[i].step(Ui::lerp_amount);

  for (uint32_t i = arena_info.player_count; i < LEADERBOARD_SIZE; ++i)
    arena_info.scores[i] = 0;
}

void Simulation::post_tick() {
  for_each_entity([](Simulation& sim, Entity& ent) { ent.reset_protocol(); });
}


Entity& Simulation::force_alloc_ent(const EntityID id) {
  assert(id.id < ENTITY_CAP);
  DEBUG_ONLY(std::cout << "ent_create " << id << "\n";)
  assert(!entity_tracker[id.id]);
  entities[id.id].init();
  entity_tracker.set(id.id, 1);
  hash_tracker[id.id] = id.hash;
  entities[id.id].id = id;
  return entities[id.id];
}