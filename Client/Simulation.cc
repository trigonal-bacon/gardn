#include <Shared/Simulation.hh>

#include <cmath>

void Simulation::tick() {
    pre_tick();
}

void Simulation::tick_lerp(double dt) {
    double lerp_amt = 1 - (pow(1 - 0.15, dt * 60 / 1000));
    for (uint32_t i = 0; i < active_entities.length; ++i) {
        Entity &ent = get_ent(active_entities[i]);
        double amt = ent.touched ? lerp_amt : 1;
        ent.touched = 1;
        if (ent.has_component(kFlower)) {
            LERP(ent.eye_x, cosf(ent.eye_angle)*3, amt);
            LERP(ent.eye_y, sinf(ent.eye_angle)*3, amt);
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