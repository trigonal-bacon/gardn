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

static void update_client(Simulation *sim, Client *client) {
    if (client == nullptr) return;
    if (!client->verified) return;
    if (!sim->ent_exists(client->camera)) return;
    std::set<EntityID> in_view;
    in_view.insert(client->camera);
    Entity &camera = sim->get_ent(client->camera);
    if (sim->ent_exists(camera.player)) 
        in_view.insert(camera.player);
    Writer writer(Server::OUTGOING_PACKET);
    writer.write<uint8_t>(kClientbound::kClientUpdate);
    writer.write<EntityID>(client->camera);
    sim->spatial_hash.query(camera.camera_x, camera.camera_y, 960 / camera.fov, 540 / camera.fov, [&](Simulation *, Entity &ent){
        in_view.insert(ent.id);
    });

    for (EntityID i: client->last_in_view)
        if (!in_view.contains(i)) writer.write<EntityID>(i);

    writer.write<EntityID>(NULL_ENTITY);
    //upcreates
    for (EntityID id: in_view) {
        assert(sim->ent_exists(id));
        Entity &ent = sim->get_ent(id);
        uint8_t create = !client->last_in_view.contains(id);
        writer.write<EntityID>(id);
        writer.write<uint8_t>(create);
        ent.write(&writer, create);
    }
    writer.write<EntityID>(NULL_ENTITY);
    //write arena stuff
    writer.write<uint8_t>(client->seen_arena);
    sim->arena_info.write(&writer, client->seen_arena);
    client->seen_arena = 1;
    //set client->last_in_view
    client->last_in_view.clear();
    for (EntityID i: in_view) client->last_in_view.insert(i);

    client->send_packet(writer.packet, writer.at - writer.packet);
}

static void calculate_leaderboard(Simulation *sim) {
    std::vector<Entity *> players;
    sim->for_each<kFlower>([&](Simulation *s, Entity &ent) { if (!ent.has_component(kMob)) players.push_back(&ent); });
    std::stable_sort(players.begin(), players.end(), [](Entity *a, Entity *b){ return a->score > b->score; });
    uint32_t num = players.size();
    sim->arena_info.set_player_count(num);
    if (num > 10) num = 10;
    for (uint32_t i = 0; i < num; ++i) {
        sim->arena_info.set_names(i, players[i]->name);
        sim->arena_info.set_scores(i, players[i]->score);
    }
}

void Simulation::tick() {
    pre_tick();
    if (frand() < 0.01)
        for (uint32_t i = 0; i < 10; ++i) Map::spawn_random_mob();
    for (uint32_t i = 0; i < active_entities.length; ++i) {
        Entity &ent = get_ent(active_entities[i]);
        if (ent.has_component(kPhysics)) {
            if (ent.deletion_tick > 0)
                request_delete(ent.id);
            spatial_hash.insert(ent);
        }
    }

    for_each<kWeb>(tick_web_behavior);
    for_each<kFlower>(tick_player_behavior);
    for_each<kMob>(tick_ai_behavior);
    for_each<kPetal>(tick_petal_behavior);
    for_each<kHealth>(tick_health_behavior);
    spatial_hash.collide(on_collide);
    for_each<kPhysics>(tick_entity_motion);
    for_each<kSegmented>(tick_segment_behavior);
    for_each<kDrop>(tick_drop_behavior);
    for_each<kCamera>(tick_camera_behavior);
    calculate_leaderboard(this);
    post_tick();
}

void Simulation::post_tick() {
    for (Client *client: Server::clients) update_client(this, client);
    //send_state & reset all remaining active entities
    //reset state of all entities FIRST
    arena_info.reset_protocol();
    for (uint32_t i = 0; i < active_entities.length; ++i) {
        //no deletions mid tick
        DEBUG_ONLY(assert(ent_exists(active_entities[i]));)
        Entity &ent = get_ent(active_entities[i]);
        ent.reset_protocol();
        ++ent.lifetime;
        if (BIT_AT(ent.flags, EntityFlags::kIsDespawning)) {
            if (ent.despawn_tick == 0)
                request_delete(ent.id);
            else
                --ent.despawn_tick;
        }
    }
    
    for (uint32_t i = 0; i < pending_delete.length; ++i) {
        //guarantee entity exists
        DEBUG_ONLY(assert(ent_exists(pending_delete[i]));)
        Entity &ent = get_ent(pending_delete[i]);
        if (!ent.has_component(kPhysics)) 
            _delete_ent(pending_delete[i]);
        else {
            if (ent.deletion_tick >= DELETION_ANIMATION_TICKS) 
                _delete_ent(pending_delete[i]);
            else {
                if (ent.deletion_tick == 0)
                    entity_on_death(this, ent);
                ent.set_deletion_tick(ent.deletion_tick + 1);
                ent.pending_delete = 0;
            }
        }
    }
    pending_delete.clear();
    active_entities.clear();
    spatial_hash.clear();
}
