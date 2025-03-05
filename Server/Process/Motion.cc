#include <Server/Process/Process.hh>

#include <Shared/Simulation.hh>
#include <Shared/Entity.hh>

void tick_entity_motion(Simulation *sim, Entity &ent) {
    if (ent.pending_delete) return;
    ent.velocity *= 1 - ent.friction;
    ent.velocity += ent.acceleration;
    {
        if (ent.collision_velocity.magnitude() > 0) {
            Vector col_vel = ent.collision_velocity;
            col_vel.normalize();
            float align = ent.velocity.x * col_vel.x
                        + ent.velocity.y + col_vel.y;
            if (align < 0) {
                ent.velocity.x -= col_vel.x * align;
                ent.velocity.y -= col_vel.y * align;
            }
        }
    }
    ent.set_x(ent.x + ent.velocity.x + ent.collision_velocity.x);
    ent.set_y(ent.y + ent.velocity.y + ent.collision_velocity.y);
    if (!ent.has_component(kPetal)) {
        ent.set_x(fclamp(ent.x, ent.radius, ARENA_WIDTH - ent.radius));
        ent.set_y(fclamp(ent.y, ent.radius, ARENA_HEIGHT - ent.radius));
    }
    if (ent.has_component(kFlower)) {
        if (ent.acceleration.x != 0 || ent.acceleration.y != 0)
            ent.set_eye_angle(ent.acceleration.angle());
    }
    ent.acceleration.set(0,0);
    ent.collision_velocity.set(0,0);
}