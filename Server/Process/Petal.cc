#include <Server/Process.hh>

#include <Server/EntityFunctions.hh>
#include <Server/Spawn.hh>
#include <Shared/Entity.hh>
#include <Shared/Simulation.hh>
#include <Shared/StaticData.hh>

#include <cmath>

void tick_petal_behavior(Simulation& sim, Entity& petal) {
  if (petal.pending_delete)
    return;
  if (!sim.ent_alive(petal.get_owner())) {
    sim.request_delete(petal.id);
    return;
  }
  Entity& player = sim.get_ent(petal.get_owner());
  const PetalData& petal_data = PETAL_DATA[petal.get_petal_id()];
  if (petal_data.attributes.rotation_style == PetalAttributes::kPassiveRot) {
    // simulate on clientside
    float rot_amt = petal.get_petal_id() == PetalID::kWing ? 10.0 : 1.0;
    if (petal.id.id % 2)
      petal.set_angle(petal.get_angle() + rot_amt / TPS);
    else
      petal.set_angle(petal.get_angle() - rot_amt / TPS);
  } else if (petal_data.attributes.rotation_style ==
                 PetalAttributes::kFollowRot &&
             !(Bits::at(petal.flags, EntityFlags::kIsDespawning))) {
    Vector delta(petal.get_x() - player.get_x(),
                 petal.get_y() - player.get_y());
    petal.set_angle(delta.angle());
  }
  if (Bits::at(petal.flags, EntityFlags::kIsDespawning) ||
      Bits::at(petal.flags, EntityFlags::kIsDetached)) {
    switch (petal.get_petal_id()) {
    case PetalID::kMoon: {
      /*
      Vector delta(player.get_x() - petal.get_x(), player.get_y() -
      petal.get_y()); float magnitude = 20000 * PLAYER_ACCELERATION / (delta.x *
      delta.x + delta.y * delta.y + 1e-4); magnitude = std::fmin(magnitude,
      PLAYER_ACCELERATION * 2); delta.set_magnitude(magnitude);
      petal.acceleration.set(delta.x, delta.y);
      */
      float wanting_angle = petal.get_angle() * 2.5;
      float wanting_radius = 100 + player.get_radius();
      float wanting_x =
          player.get_x() + wanting_radius * std::cosf(wanting_angle);
      float wanting_y =
          player.get_y() + wanting_radius * std::sinf(wanting_angle);
      Vector delta(wanting_x - petal.get_x(), wanting_y - petal.get_y());
      petal.acceleration.set(delta.x * 0.5, delta.y * 0.5);
      break;
    }
    default:
      petal.acceleration.set(0, 0);
      break;
    }
    return;
  }
  if (petal_data.attributes.secondary_reload == 0)
    return;
  if (petal.secondary_reload <= petal_data.attributes.secondary_reload * TPS) {
    ++petal.secondary_reload;
    return;
  }
  if (petal_data.attributes.burst_heal > 0 &&
      player.health < player.max_health && player.dandy_ticks == 0) {
    Vector delta(player.get_x() - petal.get_x(),
                 player.get_y() - petal.get_y());
    if (delta.magnitude() < petal.get_radius()) {
      inflict_heal(sim, player, petal_data.attributes.burst_heal);
      sim.request_delete(petal.id);
      return;
    }
    delta.set_magnitude(PLAYER_ACCELERATION * 4);
    petal.acceleration = delta;
  }
  switch (petal.get_petal_id()) {
  case PetalID::kMissile:
    if (Bits::at(player.input, InputFlags::kAttacking)) {
      petal.velocity.unit_normal(petal.get_angle()).set_magnitude(1.4);
      petal.constant_velocity.unit_normal(petal.get_angle()).set_magnitude(0.6);
      entity_set_despawn_tick(petal, 2.5 * TPS);
    }
    break;
  case PetalID::kTriweb:
  case PetalID::kWeb: {
    if (Bits::at(player.input, InputFlags::kAttacking)) {
      Vector delta(petal.get_x() - player.get_x(),
                   petal.get_y() - player.get_y());
      float angle = delta.angle();
      if (petal.get_petal_id() == PetalID::kTriweb)
        angle += frand() - 0.5;
      petal.velocity.unit_normal(angle).set_magnitude(2.4);
      entity_set_despawn_tick(petal, 0.6 * TPS);
    } else if (Bits::at(player.input, InputFlags::kDefending))
      entity_set_despawn_tick(petal, 0.6 * TPS);
    break;
  }
  case PetalID::kBubble:
    if (Bits::at(player.input, InputFlags::kDefending)) {
      Vector v(player.get_x() - petal.get_x(), player.get_y() - petal.get_y());
      v.set_magnitude(PLAYER_ACCELERATION * 0.56);
      player.velocity += v;
      sim.request_delete(petal.id);
    }
    break;
  case PetalID::kPollen:
    if (Bits::at(player.input, InputFlags::kAttacking)) {
      Vector v(petal.get_x() - player.get_x(), petal.get_y() - player.get_y());
      v.normalize().set_magnitude(0.5);
      petal.velocity += v;
      entity_set_despawn_tick(petal, 4.0 * TPS);
    } else if (Bits::at(player.input, InputFlags::kDefending)) {
      Vector v(petal.get_x() - player.get_x(), petal.get_y() - player.get_y());
      v.normalize().set_magnitude(-0.5);
      petal.velocity += v;
      entity_set_despawn_tick(petal, 4.0 * TPS);
    }
    break;
  case PetalID::kPeas:
  case PetalID::kPoisonPeas:
    if (Bits::at(player.input, InputFlags::kAttacking)) {
      float spray_angle = frand() * 2 * M_PI;
      for (uint32_t i = 0; i < petal_data.count; ++i) {
        Entity& new_petal =
            i == 0 ? petal : alloc_petal(sim, petal.get_petal_id(), player);
        new_petal.set_x(petal.get_x());
        new_petal.set_y(petal.get_y());
        new_petal.set_split_projectile(0);
        new_petal.heading_angle =
            spray_angle + (2 * M_PI * i) / petal_data.count;
        new_petal.velocity.unit_normal(new_petal.heading_angle)
            .set_magnitude(1.4);
        new_petal.constant_velocity.unit_normal(new_petal.heading_angle)
            .set_magnitude(0.6);
        entity_set_despawn_tick(new_petal, 1.0 * TPS);
      }
    }
    break;
  case PetalID::kMoon: {
    break;
  }
  default:
    break;
  }
}