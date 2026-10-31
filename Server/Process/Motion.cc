#include <Server/Process.hh>

#include <Server/Simulation.hh>
#include <Shared/Entity.hh>

#include <cmath>

void tick_entity_motion(Simulation& sim, Entity& ent) {
  if (ent.pending_delete)
    return;
  if (ent.slow_ticks > 0) {
    ent.speed_ratio *= 0.5;
    --ent.slow_ticks;
  }
  const float dt = (1000.0f / TPS);
  const float damping = std::powf(0.992, dt);
  ent.velocity += ent.acceleration * (dt / 1000) * ent.speed_ratio;
  const float factor = 125 * (1 - damping);
  Vector position{ent.get_x(), ent.get_y()};
  position += (ent.velocity * factor) + ent.collision_velocity +
              (ent.constant_velocity * dt);
  ent.set_x(position.x);
  ent.set_y(position.y);
  ent.velocity *= damping;

  if (!ent.has_component<kPetal>() && !ent.has_component<kWeb>()) {
    ent.set_x(
        fclamp(ent.get_x(), ent.get_radius(), ARENA_WIDTH - ent.get_radius()));
    ent.set_y(
        fclamp(ent.get_y(), ent.get_radius(), ARENA_HEIGHT - ent.get_radius()));
  }

  ent.collision_velocity.set(0, 0);
  ent.speed_ratio = 1;
}