#include <Server/Game.hh>

#include <Server/Client.hh>
#include <Server/EntityFunctions.hh>
#include <Server/PetalTracker.hh>
#include <Server/Server.hh>
#include <Server/Spawn.hh>

#include <Shared/Binary.hh>
#include <Server/Bot.hh>
#include <Shared/Entity.hh>
#include <Shared/Map.hh>

static void _update_client(Simulation *sim, Client *client) {
    if (client == nullptr) return;
    if (!client->verified) return;
    if (sim == nullptr) return;
    if (!sim->ent_exists(client->camera)) return;
    std::set<EntityID> in_view;
    std::vector<EntityID> deletes;
    in_view.insert(client->camera);
    Entity &camera = sim->get_ent(client->camera);
    if (sim->ent_exists(camera.get_player())) 
        in_view.insert(camera.get_player());
    #ifdef GAMEMODE_TDM
    Entity &team = sim->get_ent(camera.get_team());
    in_view.insert(team.minimap_dots.begin(), team.minimap_dots.end());
    #endif
    Writer writer(Server::OUTGOING_PACKET);
    writer.write<uint8_t>(Clientbound::kClientUpdate);
    writer.write<EntityID>(client->camera);
    sim->spatial_hash.query(camera.get_camera_x(), camera.get_camera_y(), 
    960 / camera.get_fov() + 50, 540 / camera.get_fov() + 50, [&](Simulation *, Entity &ent){
        in_view.insert(ent.id);
    });

    for (EntityID const &i: client->in_view) {
        if (!in_view.contains(i)) {
            writer.write<EntityID>(i);
            deletes.push_back(i);
        }
    }

    for (EntityID const &i : deletes)
        client->in_view.erase(i);

    writer.write<EntityID>(NULL_ENTITY);
    //upcreates
    for (EntityID id: in_view) {
        DEBUG_ONLY(assert(sim->ent_exists(id));)
        Entity &ent = sim->get_ent(id);
        uint8_t create = !client->in_view.contains(id);
        writer.write<EntityID>(id);
        writer.write<uint8_t>(create | (ent.pending_delete << 1));
        ent.write(&writer, BitMath::at(create, 0));
        client->in_view.insert(id);
    }
    writer.write<EntityID>(NULL_ENTITY);
    //write arena stuff
    writer.write<uint8_t>(client->seen_arena);
    sim->arena_info.write(&writer, client->seen_arena);
    client->seen_arena = 1;
    client->send_packet(writer.packet, writer.at - writer.packet);
}

GameInstance::GameInstance() : simulation(), clients(), team_manager(&simulation) {}

void GameInstance::spawn_bots(uint32_t count) {
    for (uint32_t i = 0; i < count; ++i) {
        // Create a self-team camera
        Entity &cam = simulation.alloc_ent();
        cam.add_component(kCamera);
        cam.add_component(kRelations);
        cam.set_team(cam.id);
        cam.set_color(ColorID::kYellow);
        cam.set_fov(BASE_FOV);
        cam.set_respawn_level(1);
        // Seed some basics for tracking
        for (uint32_t s = 0; s < loadout_slots_at_level(cam.get_respawn_level()); ++s) {
            cam.set_inventory(s, PetalID::kBasic);
            PetalTracker::add_petal(&simulation, cam.get_inventory(s));
        }
        // Create player and spawn
        Entity &player = alloc_player(&simulation, cam.get_team());
        ensure_bot_name(&simulation, player);
        player_spawn(&simulation, cam, player);
        // Track bot by its camera (so we can respawn players if they die)
        bots.push_back(cam.id);
    }
}

void GameInstance::init() {
    for (uint32_t i = 0; i < ENTITY_CAP / 2; ++i)
        Map::spawn_random_mob(&simulation, frand() * ARENA_WIDTH, frand() * ARENA_HEIGHT);
    #ifdef GAMEMODE_TDM
    team_manager.add_team(ColorID::kBlue);
    team_manager.add_team(ColorID::kRed);
    #endif
    spawn_bots(8); // bots
}

void GameInstance::tick() {
    simulation.tick();
    #ifdef GAMEMODE_TDM
    team_manager.tick();
    #endif

    // Tick bots and auto-respawn their players if missing
    for (auto it = bots.begin(); it != bots.end(); ) {
        if (!simulation.ent_exists(*it)) { it = bots.erase(it); continue; }
        Entity &cam = simulation.get_ent(*it);
        if (!cam.has_component(kCamera)) { ++it; continue; }
        if (simulation.ent_exists(cam.get_player())) {
            Entity &p = simulation.get_ent(cam.get_player());
            if (p.has_component(kFlower)) {
                tick_bot_player_behavior(&simulation, p);
            }
        } else {
            // Respawn a new player for this bot camera
            Entity &p = alloc_player(&simulation, cam.get_team());
            ensure_bot_name(&simulation, p);
            player_spawn(&simulation, cam, p);
        }
            ++it;
    }
    for (Client *client : clients)
        _update_client(&simulation, client);
    simulation.post_tick();
}

void GameInstance::add_client(Client *client, uint64_t recovery_id) {
    DEBUG_ONLY(assert(client->game != this);)
    if (client->game != nullptr)
        client->game->remove_client(client);
    client->game = this;
    clients.insert(client);
    EntityID camera_id = NULL_ENTITY;

    // Only attempt recovery when a non-zero recovery ID was supplied.
    if (recovery_id != 0) {
        simulation.for_each<kCamera>([&](Simulation *sim, Entity &ent){
            if (ent.get_recovery_id() == recovery_id) {
                // If this assert still triggers, it means two ents share the same recovery_id.
                assert(camera_id == NULL_ENTITY);
                camera_id = ent.id;
            }
        });
    }

    if (camera_id == NULL_ENTITY) {
        if (Server::is_draining) {
            client->disconnect(CloseReason::kOutdated, "Outdated Version");
            return;
        }
        Entity &camera = alloc_camera(&simulation, team_manager);
        camera.set_recovery_id((static_cast<uint64_t>(std::time(0)) << 32) | std::rand());
        client->camera = camera.id;
    } else
        client->camera = camera_id;
    Entity &camera = simulation.get_ent(client->camera);
    if (camera.client != nullptr)
        camera.client->disconnect(CloseReason::kRecovered, "Session Recovered");
    camera.client = client;
    BitMath::unset(camera.flags, EntityFlags::kIsDespawning);
}


void GameInstance::remove_client(Client *client) {
    DEBUG_ONLY(assert(client->game == this);)
    clients.erase(client);
    if (simulation.ent_exists(client->camera)) {
        Entity &c = simulation.get_ent(client->camera);
        #ifdef DEBUG
        entity_set_despawn_tick(c, 0);
        #else
        entity_set_despawn_tick(c, 60 * TPS);
        #endif
        c.client = nullptr;
    }
    client->game = nullptr;
}