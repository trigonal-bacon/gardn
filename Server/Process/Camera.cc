#include <Server/Process.hh>

#include <Shared/Entity.hh>
#include <Shared/Map.hh>
#include <Shared/Simulation.hh>
#include <Shared/StaticData.hh>

#include <iostream>

void tick_camera_behavior(Simulation *sim, Entity &ent) {
    if (sim->ent_exists(ent.player)) {
        Entity &player = sim->get_ent(ent.player);
        player.acceleration = ent.acceleration;
        player.input = ent.input;
        ent.set_camera_x(player.x);
        ent.set_camera_y(player.y);
        player.set_loadout_count(loadout_slots_at_level(score_to_level(player.score)));
        ent.last_damaged_by = player.last_damaged_by;
        struct Map::ZoneDefinition const &zone = Map::MAP[Map::get_zone_from_pos(player.x, player.y)];
        if (zone.difficulty < Map::difficulty_at_level(score_to_level(player.score))) {
            if (player.overlevel_timer < PETAL_DISABLE_DELAY * TPS)
                player.set_overlevel_timer(player.overlevel_timer + 1);
            else player.set_overlevel_timer(PETAL_DISABLE_DELAY * TPS);
        } else {
            if (player.overlevel_timer > 0)
                player.set_overlevel_timer(player.overlevel_timer - 0.1);
            else player.set_overlevel_timer(0);
        }
    } else {
        ent.set_fov(BASE_FOV * 0.9);
        if (sim->ent_exists(ent.last_damaged_by)){
            Entity &viewer = sim->get_ent(ent.last_damaged_by);
            ent.set_camera_x(viewer.x);
            ent.set_camera_y(viewer.y);
        }
    }
}
