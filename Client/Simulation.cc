#include <Shared/Simulation.hh>

#include <Client/Game.hh>

#include <cmath>
#include <iostream>

void Simulation::tick() {
    pre_tick();
}

void Simulation::tick_lerp(double dt) {
    double amt = 1 - (pow(1 - 0.25, dt * 60 / 1000));
    for (uint32_t i = 0; i < active_entities.size(); ++i) {
        Entity &ent = get_ent(active_entities[i]);
        if (ent.has_component(kPhysics)) {
            float prevx = ent.x;
            float prevy = ent.y;
            ent.x.step(amt);
            ent.y.step(amt);
            Vector vel(ent.x - prevx, ent.y - prevy);
            ent.animation += (1 + 0.75 * vel.magnitude()) * 0.075;
            ent.radius.step(amt);
            ent.angle.step_angle(amt);
            if (ent.deleting)
                LERP(ent.deletion_animation, 1, amt * 1.5);
        }
        if (ent.has_component(kCamera)) {
            ent.camera_x.step(amt);
            ent.camera_y.step(amt);
            ent.fov.step(amt);
            Game::respawn_level = ent.respawn_level;
        }
        if (ent.has_component(kHealth)) {
            ent.health_ratio.step(amt);
            if (ent.damaged == 1 && ent.damage_flash < 0.1)
                ent.damage_flash = 1;
            else //ent.damage_flash = fclamp(ent.damage_flash - Ui::dt / 150, 0, 1);
                LERP(ent.damage_flash, 0, amt)
            if (ent.damaged)
                ent.last_damaged_time = Game::timestamp;
            if ((float) ent.health_ratio > 0.999)
                LERP(ent.healthbar_opacity, 0, amt)
            else
                ent.healthbar_opacity = 1;
            if (ent.healthbar_lag < ent.health_ratio)
                ent.healthbar_lag = ent.health_ratio;
            else if (Game::timestamp - ent.last_damaged_time > 250)
                LERP(ent.healthbar_lag, ent.health_ratio, amt / 3)
        }
        if (ent.has_component(kFlower)) {
            LERP(ent.eye_x, cosf(ent.eye_angle) * 3, amt);
            LERP(ent.eye_y, sinf(ent.eye_angle) * 3, amt);
            if (BIT_AT(ent.face_flags, 0) 
               || BIT_AT(ent.face_flags, 2) 
               || BIT_AT(ent.face_flags, 3)) LERP(ent.mouth, 5, amt)
            else if (BIT_AT(ent.face_flags, 1)) LERP(ent.mouth, 8, amt)
            else LERP(ent.mouth, 15, amt)
        }
    }
    for (uint32_t i = 0; i < arena_info.player_count; ++i)
        arena_info.scores[i].step(amt);

    for (uint32_t i = arena_info.player_count; i < LEADERBOARD_SIZE; ++i)
        arena_info.scores[i] = 0;
}

void Simulation::post_tick() {
   for (uint32_t i = 0; i < active_entities.size(); ++i) {
        assert(ent_exists(active_entities[i]));
        Entity &ent = get_ent(active_entities[i]);
        ent.reset_protocol();
    }
    /*
    MUST delete instantly in protocol, or it won't allow instant realloc
    for (uint32_t i = 0; i < pending_delete.size(); ++i) {
        assert(ent_exists(pending_delete[i]));
        _delete_ent(pending_delete[i]);
    }
    pending_delete.clear();
    */
}