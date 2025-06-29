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
    sim->spatial_hash.query(camera.camera_x, camera.camera_y, 960 / camera.fov + 50, 540 / camera.fov + 50, [&](Simulation *, Entity &ent){
        in_view.insert(ent.id);
    });

    for (EntityID i: client->last_in_view)
        if (!in_view.contains(i)) writer.write<EntityID>(i);

    writer.write<EntityID>(NULL_ENTITY);
    //upcreates
    for (EntityID id: in_view) {
        DEBUG_ONLY(assert(sim->ent_exists(id));)
        Entity &ent = sim->get_ent(id);
        uint8_t create = !client->last_in_view.contains(id);
        writer.write<EntityID>(id);
        writer.write<uint8_t>(create | (ent.pending_delete << 1));
        ent.write(&writer, create & 1);
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
    for (uint32_t i = 0; i < active_entities.size(); ++i) {
        Entity &ent = get_ent(active_entities[i]);
        if (ent.has_component(kPhysics)) {
            if (ent.deletion_tick > 0)
                request_delete(ent.id);
            spatial_hash.insert(ent);
        }
    }

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

    arena_info.reset_protocol();
    for (uint32_t i = 0; i < active_entities.size(); ++i) {
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
    
    for (uint32_t i = 0; i < pending_delete.size(); ++i) {
        //guarantee entity exists
        DEBUG_ONLY(assert(ent_exists(pending_delete[i]));)
        Entity &ent = get_ent(pending_delete[i]);
        if (!ent.has_component(kPhysics)) 
            _delete_ent(pending_delete[i]);
        else {
            if (ent.deletion_tick >= TPS / 5) 
                _delete_ent(pending_delete[i]);
            else {
                if (ent.deletion_tick == 0)
                    entity_on_death(this, ent);
                ent.deletion_tick += 1;
                ent.pending_delete = 0;
            }
        }
    }
}
