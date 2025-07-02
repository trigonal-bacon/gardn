#include <Shared/Simulation.hh>

#include <Server/Process.hh>
#include <Server/Client.hh>
#include <Server/EntityFunctions.hh>
#include <Server/Server.hh>
#include <Server/Spawn.hh>
#include <Server/SpatialHash.hh>

#include <Shared/Map.hh>

#include <algorithm>
#include <vector>

static void calculate_leaderboard(Simulation *sim) {
    std::vector<Entity const *> players;
    sim->for_each<kCamera>([&](Simulation *sim, Entity &ent) { 
        if (sim->ent_alive(ent.player)) players.push_back(&sim->get_ent(ent.player));
    });
    std::stable_sort(players.begin(), players.end(), [](Entity const *a, Entity const *b){
        return a->score > b->score;
    });
    uint32_t num = players.size();
    sim->arena_info.set_player_count(num);
    if (num > LEADERBOARD_SIZE) num = LEADERBOARD_SIZE;
    for (uint32_t i = 0; i < num; ++i) {
        sim->arena_info.set_names(i, players[i]->name);
        sim->arena_info.set_scores(i, players[i]->score);
    }
}

void Simulation::tick() {
    pre_tick();
    spatial_hash.refresh(ARENA_WIDTH, ARENA_HEIGHT);
    if (frand() < 0.01)
        for (uint32_t i = 0; i < 10; ++i) Map::spawn_random_mob(this);
    for_each_entity([](Simulation *sim, Entity &ent) {
        if (ent.has_component(kPhysics)) {
            //if (ent.deletion_tick > 0)
                //sim->request_delete(ent.id); //no need, pending_delete is sticky
            sim->spatial_hash.insert(ent);
        }
    });

    for_each<kFlower>(tick_player_behavior);
    for_each<kMob>(tick_ai_behavior);
    for_each<kPetal>(tick_petal_behavior);
    for_each<kHealth>(tick_health_behavior);
    spatial_hash.collide(on_collide);
    for_each<kPhysics>(tick_entity_motion);
    for_each<kSegmented>(tick_segment_behavior);
    for_each<kCamera>(tick_camera_behavior);
    calculate_leaderboard(this);
}

void Simulation::post_tick() {
    arena_info.reset_protocol();
    for_each_entity([](Simulation *sim, Entity &ent) {
        //no deletions mid tick
        ent.reset_protocol();
        ++ent.lifetime;
        if (BIT_AT(ent.flags, EntityFlags::kIsDespawning)) {
            if (ent.despawn_tick == 0)
                sim->request_delete(ent.id);
            else
                --ent.despawn_tick;
        }
        if (ent.immunity_ticks > 0) --ent.immunity_ticks;
    });
    for_each_entity([](Simulation *sim, Entity &ent) {
        if (!ent.pending_delete) return;
        if (!ent.has_component(kPhysics)) 
            sim->_delete_ent(ent.id);
        else {
            if (ent.deletion_tick >= TPS / 5) 
                sim->_delete_ent(ent.id);
            else {
                if (ent.deletion_tick == 0)
                    entity_on_death(sim, ent);
                ent.deletion_tick += 1;
            }
        }
    });
}
