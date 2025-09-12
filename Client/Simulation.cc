#include <Shared/Simulation.hh>

#include <Client/Game.hh>

#include <Client/Ui/Extern.hh>

#include <cmath>
#include <iostream>

void Entity::tick_lerp(float amt) {
    if (has_component(kPhysics)) {
        float prev_x = x;
        float prev_y = y;
        if (!pending_delete) {
            x.step(amt);
            y.step(amt);
        }
        if (has_component(kDrop) || has_component(kWeb)) {
            if (lifetime < TPS)
                animation = lerp(animation, 1, amt * 0.75);
            else animation = 1;
        } else {
            Vector vel(x - prev_x, y - prev_y);
            animation += (1 + 0.75 * vel.magnitude()) * 0.075;
        }
        radius.step(amt);
        angle.step_angle(amt);
        if (pending_delete)
            deletion_animation = fclamp(deletion_animation + Ui::dt / 150, 0, 1);
    }
    if (has_component(kCamera)) {
        camera_x.step(amt);
        camera_y.step(amt);
        fov.step(amt);
        Game::respawn_level = respawn_level;
    }
    if (has_component(kHealth)) {
        health_ratio.step(amt);
        if (damaged == 1 && damage_flash < 0.1 && !pending_delete)
            damage_flash = 1;
        else //damage_flash = fclamp(damage_flash - Ui::dt / 150, 0, 1);
            damage_flash = lerp(damage_flash, 0, amt);
        if (damaged)
            last_damaged_time = Game::timestamp;
        damaged.clear();
        if ((float) health_ratio > 0.999)
            healthbar_opacity = lerp(healthbar_opacity, 0, amt);
        else
            healthbar_opacity = 1;
        if (healthbar_lag < health_ratio)
            healthbar_lag = health_ratio;
        else if (Game::timestamp - last_damaged_time > 250)
            healthbar_lag = lerp(healthbar_lag, health_ratio, amt / 3);
    }
    if (has_component(kFlower)) {
        float eye_angle = angle.anchor();
        eye_x = lerp(eye_x, cosf(eye_angle) * 2, amt);
        eye_y = lerp(eye_y, sinf(eye_angle) * 4, amt);
        if (BitMath::at(face_flags, FaceFlags::kAttacking)
            || BitMath::at(face_flags, FaceFlags::kPoisoned) 
            || BitMath::at(face_flags, FaceFlags::kDandelioned)
            || pending_delete) mouth = lerp(mouth, 5, amt);
        else if (BitMath::at(face_flags, FaceFlags::kDefending)) mouth = lerp(mouth, 8, amt);
        else mouth = lerp(mouth, 15, amt);
    }
}

void Simulation::on_tick() {
    for_each_entity([](Simulation *sim, Entity &ent) {
        ent.tick_lerp(Ui::lerp_amount);
    });

    for (uint32_t i = 0; i < std::min(arena_info.player_count, LEADERBOARD_SIZE); ++i)
        arena_info.scores[i].step(Ui::lerp_amount);

    for (uint32_t i = arena_info.player_count; i < LEADERBOARD_SIZE; ++i)
        arena_info.scores[i] = 0;
}

void Simulation::post_tick() {
    for_each_entity([](Simulation *sim, Entity &ent) {
        ent.reset_protocol();
    });
}