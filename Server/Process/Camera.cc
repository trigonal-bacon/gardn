#include <Server/Process/Process.hh>

#include <Shared/Simulation.hh>
#include <Shared/Entity.hh>

#include <Shared/StaticData.hh>

void tick_camera_behavior(Simulation *sim, Entity &ent) {
    if (sim->ent_exists(ent.player)) {
        Entity &player = sim->get_ent(ent.player);
        ent.set_camera_x(player.x);
        ent.set_camera_y(player.y);
        player.set_loadout_count(loadOutSlotsAtLevel(scoreToLevel(player.score)));
        ent.last_damaged_by = player.last_damaged_by;
    } else {
        ent.set_fov(BASE_FOV * 0.9);
        if (sim->ent_exists(ent.last_damaged_by)){
            Entity &viewer = sim->get_ent(ent.last_damaged_by);
            ent.set_camera_x(viewer.x);
            ent.set_camera_y(viewer.y);
        }
    }
}
