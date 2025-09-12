#include <Server/Process.hh>

#include <Shared/Entity.hh>
#include <Shared/Map.hh>
#include <Shared/Simulation.hh>
#include <Shared/StaticData.hh>

void tick_camera_behavior(Simulation *sim, Entity &ent) {
    if (sim->ent_exists(ent.get_player())) {
        Entity &player = sim->get_ent(ent.get_player());
        ent.set_camera_x(player.get_x());
        ent.set_camera_y(player.get_y());
        player.set_loadout_count(loadout_slots_at_level(score_to_level(player.get_score())));
        if (player.acceleration.x != 0 || player.acceleration.y != 0)
            player.set_angle(player.acceleration.angle());

        ent.last_damaged_by = player.last_damaged_by;
        struct ZoneDefinition const &zone = MAP_DATA[Map::get_zone_from_pos(player.get_x(), player.get_y())];
        if (zone.difficulty < Map::difficulty_at_level(score_to_level(player.get_score()))) {
            if (player.get_overlevel_timer() < PETAL_DISABLE_DELAY * TPS)
                player.set_overlevel_timer(player.get_overlevel_timer() + 1);
            else player.set_overlevel_timer(PETAL_DISABLE_DELAY * TPS);
        } else {
            if (player.get_overlevel_timer() > 0)
                player.set_overlevel_timer(player.get_overlevel_timer() - 0.1);
            else player.set_overlevel_timer(0);
        }
    } else {
        ent.set_player(NULL_ENTITY);
        ent.set_fov(BASE_FOV * 0.9);
        if (sim->ent_exists(ent.last_damaged_by)){
            Entity &viewer = sim->get_ent(ent.last_damaged_by);
            ent.set_camera_x(viewer.get_x());
            ent.set_camera_y(viewer.get_y());
        }
    }
}
