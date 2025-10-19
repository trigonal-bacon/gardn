#include <Server/Process.hh>

#include <Server/PetalTracker.hh>
#include <Server/Server.hh>

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
        if (BitMath::at(ent.flags, EntityFlags::kIsDespawning)) {
            BitMath::set(player.flags, EntityFlags::kZombie);
            player.input = 0;
            player.acceleration.set(0, 0);
            float dmg = player.max_health / (60 * TPS);
            player.health = fclamp(player.health - dmg, 0, player.max_health);
        } else
            BitMath::unset(player.flags, EntityFlags::kZombie);
    } else {
        if (BitMath::at(ent.flags, EntityFlags::kCPUControlled)) {
            //temp: cpu cameras die
            return sim->request_delete(ent.id);
        }
        ent.set_player(NULL_ENTITY);
        ent.set_fov(BASE_FOV * 0.9);
        if (sim->ent_exists(ent.last_damaged_by)){
            Entity &spectating = sim->get_ent(ent.last_damaged_by);
            ent.set_camera_x(spectating.get_x());
            ent.set_camera_y(spectating.get_y());
        }
    }
    if (BitMath::at(ent.flags, EntityFlags::kIsDespawning) && ent.despawn_tick == 0) {
        --Server::player_count;
        if (sim->ent_exists(ent.get_team()))
            --sim->get_ent(ent.get_team()).player_count;
        if (sim->ent_exists(ent.get_player()))
            sim->request_delete(ent.get_player());
        for (uint32_t i = 0; i < 2 * MAX_SLOT_COUNT; ++i)
            PetalTracker::remove_petal(sim, ent.get_inventory(i));
        sim->request_delete(ent.id);
    }
}
