#include <Server/Process/Process.hh>

#include <Shared/Simulation.hh>
#include <Shared/Entity.hh>

#include <iostream>

void tick_entity_motion(Simulation *sim, Entity &ent) {
    ent.velocity *= 1 - ent.friction;
    ent.velocity += ent.acceleration;
    ent.set_x(ent.x + ent.velocity.x);
    ent.set_y(ent.y + ent.velocity.y);
    if (!ent.has_component(kPetal)) {
        ent.set_x(fclamp(ent.x, ent.radius, ARENA_WIDTH - ent.radius));
        ent.set_y(fclamp(ent.y, ent.radius, ARENA_HEIGHT - ent.radius));
    }
    if (ent.has_component(kFlower)) {
        if (ent.acceleration.x != 0 || ent.acceleration.y != 0)
            ent.set_eye_angle(ent.acceleration.angle());
    }
    ent.acceleration.set(0,0);
    //ent.collision_velocity.set(0,0);
}