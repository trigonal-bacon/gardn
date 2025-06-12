#include <Server/Process.hh>

#include <Shared/Simulation.hh>
#include <Shared/Entity.hh>

void tick_entity_motion(Simulation *sim, Entity &ent) {
    if (ent.pending_delete) return;
    if (ent.slow_ticks > 0) {
        ent.speed_ratio *= 0.5;
        --ent.slow_ticks;
    }
    ent.velocity *= (1 - ent.friction);
    ent.acceleration *= ent.speed_ratio;
    ent.velocity += ent.acceleration;
    ent.set_x(ent.x + ent.velocity.x + ent.collision_velocity.x);
    ent.set_y(ent.y + ent.velocity.y + ent.collision_velocity.y);
    ent.collision_velocity *= 0.5;
    ent.velocity += ent.collision_velocity;
    if (!ent.has_component(kPetal) && !ent.has_component(kWeb)) {
        ent.set_x(fclamp(ent.x, ent.radius, Map::ARENA_WIDTH - ent.radius));
        ent.set_y(fclamp(ent.y, ent.radius, Map::ARENA_HEIGHT - ent.radius));
    }
    if (ent.has_component(kFlower)) {
        if (ent.acceleration.x != 0 || ent.acceleration.y != 0)
            ent.set_eye_angle(ent.acceleration.angle());
    }
    ent.acceleration.set(0,0);
    ent.collision_velocity.set(0,0);
    ent.speed_ratio = 1;
}