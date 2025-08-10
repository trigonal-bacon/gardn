#include <Server/Server.hh>

#include <Server/Client.hh>

#include <Shared/Binary.hh>
#include <Shared/Map.hh>
#include <Server/Stats.hh>

#include <chrono>
#include <iostream>

namespace Server {
    uint8_t OUTGOING_PACKET[max_buffer_size] = {0};
    Simulation simulation;
    std::set<Client *> clients;
    double timestamp;
}

using namespace Server;

static void _update_client(Simulation *sim, Client *client) {
    if (client == nullptr) return;
    if (!client->verified) return;
    if (sim == nullptr) return;
    if (!sim->ent_exists(client->camera)) return;
    std::set<EntityID> in_view;
    in_view.insert(client->camera);
    Entity &camera = sim->get_ent(client->camera);
    if (sim->ent_exists(camera.player)) 
        in_view.insert(camera.player);
    Writer writer(Server::OUTGOING_PACKET);
    writer.write<uint8_t>(Clientbound::kClientUpdate);
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

void Server::tick() {
    using namespace std::chrono_literals;
    auto start = std::chrono::steady_clock::now();
    Server::simulation.tick();
    for (Client *client : Server::clients) _update_client(client->simulation, client);
    Server::simulation.post_tick();
    Stats::tick();
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> tick_time = end - start;
    if (tick_time > 2ms) std::cout << "tick took " << tick_time << '\n';
}

void Server::init() {
    for (uint32_t i = 0; i < ENTITY_CAP / 2; ++i)
        Map::spawn_random_mob(&Server::simulation);
    Stats::init();
    Server::run();
}
