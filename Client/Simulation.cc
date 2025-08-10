#include <Shared/Simulation.hh>

#include <Client/Game.hh>

#include <Client/Ui/Extern.hh>

#include <cmath>
#include <iostream>

void Simulation::tick() {
    pre_tick();
    double const amt = Ui::lerp_amount;
    for_each_entity([=](Simulation *sim, Entity &ent) {
        if (ent.has_component(kPhysics)) {
            float prev_x = ent.x;
            float prev_y = ent.y;
            ent.x.step(amt);
            ent.y.step(amt);
            if (ent.has_component(kDrop) || ent.has_component(kWeb)) {
                if (ent.lifetime < TPS)
                    LERP(ent.animation, 1, amt * 0.75)
                else ent.animation = 1;
            } else {
                Vector vel(ent.x - prev_x, ent.y - prev_y);
                ent.animation += (1 + 0.75 * vel.magnitude()) * 0.075;
            }
            ent.radius.step(amt);
            ent.angle.step_angle(amt);
            if (ent.pending_delete)
                LERP(ent.deletion_animation, 1, amt);
        }
        if (ent.chat_timer > 0) {
            ent.chat_timer = std::max(0.0f, ent.chat_timer - (float)(Ui::dt/1000.0));
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
            ent.damaged.clear();
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
            if (BIT_AT(ent.face_flags, FaceFlags::kAttacking)
                || BIT_AT(ent.face_flags, FaceFlags::kPoisoned) 
                || BIT_AT(ent.face_flags, FaceFlags::kDandelioned)
                || ent.pending_delete) LERP(ent.mouth, 5, amt)
            else if (BIT_AT(ent.face_flags, FaceFlags::kDefending)) LERP(ent.mouth, 8, amt)
            else LERP(ent.mouth, 15, amt)
        }
    });
    for (uint32_t i = 0; i < arena_info.player_count; ++i)
        arena_info.scores[i].step(amt);

    for (uint32_t i = arena_info.player_count; i < LEADERBOARD_SIZE; ++i)
        arena_info.scores[i] = 0;
}

void Simulation::post_tick() {
    for_each_entity([](Simulation *sim, Entity &ent) {
        ent.reset_protocol();
    });
}