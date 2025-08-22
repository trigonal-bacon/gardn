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
        sim->arena_info.set_colors(i, players[i]->color);
    }
}

void Simulation::tick() {
    pre_tick();
    //systems (for_each loops) are ONLY SAFE TO USE HERE
    spatial_hash.refresh(ARENA_WIDTH, ARENA_HEIGHT);
    if (frand() < 1.0f / TPS)
        for (uint32_t i = 0; i < 10; ++i)
            Map::spawn_random_mob(this);
    for_each_entity([](Simulation *sim, Entity &ent) {
        if (ent.has_component(kPhysics))
            sim->spatial_hash.insert(ent);
        if (BIT_AT(ent.flags, EntityFlags::kHasCulling))
            BIT_SET(ent.flags, EntityFlags::kIsCulled);
    });
    for_each<kCamera>(tick_culling_behavior);
    for_each<kFlower>(tick_player_behavior);
    for_each<kMob>(tick_ai_behavior);
    for_each<kPetal>(tick_petal_behavior);
    for_each<kHealth>(tick_health_behavior);
    spatial_hash.collide(on_collide);
    for_each<kPhysics>(tick_entity_motion);
    for_each<kSegmented>(tick_segment_behavior);
    for_each<kCamera>(tick_camera_behavior);
    for_each<kScore>(tick_score_behavior);
    for_each<kChat>(tick_chat_behavior);
    calculate_leaderboard(this);
}

void Simulation::post_tick() {
    arena_info.reset_protocol();
    for_each_entity([](Simulation *sim, Entity &ent) {
        //no deletions mid tick
        ent.reset_protocol();
        ++ent.lifetime;
        ent.chat_sent = NULL_ENTITY;
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
            return sim->_delete_ent(ent.id);
        if (ent.deletion_tick >= TPS / 5) 
            return sim->_delete_ent(ent.id);
        if (ent.deletion_tick == 0)
            entity_on_death(sim, ent);
        ++ent.deletion_tick;
    });
    //systems are NO LONGER SAFE - active entities may have been changed as some were deleted
}
