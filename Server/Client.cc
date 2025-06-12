#include <Server/Client.hh>

#include <Server/PetalTracker.hh>
#include <Server/Server.hh>

#include <Shared/Binary.hh>
#include <Shared/Simulation.hh>

Client::Client() {}

void Client::init() {
    Entity &ent = Server::simulation.alloc_ent();
    ent.add_component(kCamera);
    ent.set_fov(BASE_FOV);
    //ent.set_camera_x(frand() * ARENA_WIDTH);
    //ent.set_camera_y(frand() * ARENA_HEIGHT);
    ent.set_respawn_level(1); //respawn at level 0, NOT 1
    for (uint32_t i = 0; i < loadout_slots_at_level(ent.respawn_level); ++i) {
        PetalTracker::add_petal(PetalID::kBasic);
        ent.set_inventory(i, PetalID::kBasic);
    }
    if (frand() < 0.0001 && PetalTracker::get_count(PetalID::kUniqueBasic) == 0) {
        PetalTracker::remove_petal(PetalID::kBasic);
        PetalTracker::add_petal(PetalID::kUniqueBasic);
        ent.set_inventory(0, PetalID::kUniqueBasic);
    } 
    //no need to track PetalID::kNone
    camera = ent.id;
}

void Client::remove() {
    if (Server::simulation.ent_exists(camera)) {
        Entity &c = Server::simulation.get_ent(camera);
        if (Server::simulation.ent_exists(c.player))
            Server::simulation.request_delete(c.player);
        for (uint32_t i = 0; i < 2 * MAX_SLOT_COUNT; ++i)
            PetalTracker::remove_petal(c.inventory[i]);
        Server::simulation.request_delete(camera);
        std::cout << "deleting camera from " << this << "\n";
    }
}

void Client::disconnect() {
    if (ws == nullptr) return;
    remove();
    Writer writer(Server::OUTGOING_PACKET);
    writer.write<uint8_t>(kClientbound::kDisconnect);
    std::string_view message(reinterpret_cast<char const *>(writer.packet), writer.at - writer.packet);
    ws->send(message, uWS::OpCode::BINARY, 0);
    ws->end();
    std::cout << "deleting client " << this << "\n";
}

uint8_t Client::alive() {
    Simulation &simulation = Server::simulation;
    return simulation.ent_exists(camera) 
    && simulation.ent_exists(simulation.get_ent(camera).player);
}