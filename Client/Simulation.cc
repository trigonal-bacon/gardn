#include <Shared/Simulation.hh>

#include <cmath>
#include <iostream>

void Simulation::tick() {
    pre_tick();
}

void Simulation::tick_lerp(double dt) {
    double amt = 1 - (pow(1 - 0.2, dt * 60 / 1000));
    for (uint32_t i = 0; i < active_entities.length; ++i) {
        Entity &ent = get_ent(active_entities[i]);
        if (ent.has_component(kPhysics)) {
            float prevx = ent.x;
            float prevy = ent.y;
            ent.x.step(amt);
            ent.y.step(amt);
            Vector vel(ent.x - prevx, ent.y - prevy);
            ent.animation += (1 + 0.3 * vel.magnitude()) * 0.1;
            ent.radius.step(amt);
            ent.angle.step_angle(amt);
            ent.deletion_tick.step(amt * 1.25);
        }
        if (ent.has_component(kCamera)) {
            ent.camera_x.step(amt);
            ent.camera_y.step(amt);
            ent.fov.step(amt);
        }
        if (ent.has_component(kHealth)) {
            ent.health_ratio.step(amt);
            if (ent.damaged == 1 && ent.damage_flash < 0.25) ent.damage_flash = 1;
            else LERP(ent.damage_flash, 0, amt);
        }
        if (ent.has_component(kFlower)) {
            LERP(ent.eye_x, cosf(ent.eye_angle) * 3, amt);
            LERP(ent.eye_y, sinf(ent.eye_angle) * 3, amt);
            if (BIT_AT(ent.face_flags, 0)) LERP(ent.mouth, 5, amt)
            else if (BIT_AT(ent.face_flags, 1)) LERP(ent.mouth, 8, amt)
            else LERP(ent.mouth, 15, amt)
        }
    }
}

void Simulation::post_tick() {
    /*
    for (uint32_t i = 0; i < pending_delete.length; ++i) {
        //guarantee entity exists
        assert(ent_exists(pending_delete[i]));
        Entity &ent = get_ent(pending_delete[i]);
        delete_ent(pending_delete[i]);
    }
    pending_delete.clear();
    */
   for (uint32_t i = 0; i < active_entities.length; ++i) {
        assert(ent_exists(active_entities[i]));
        Entity &ent = get_ent(active_entities[i]);
        ent.reset_protocol();
    }
}