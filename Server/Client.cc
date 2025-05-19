#include <Server/Client.hh>

#include <Shared/Simulation.hh>

#include <Server/Server.hh>

Client::Client() {
}

void Client::init(Server *server) {
    Entity &ent = server->simulation.alloc_ent();
    ent.add_component(kCamera);
    ent.set_fov(BASE_FOV);
    //ent.set_camera_x(frand() * ARENA_WIDTH);
    //ent.set_camera_y(frand() * ARENA_HEIGHT);
    ent.set_respawn_level(0); //respawn at level 0, NOT 1
    for (uint32_t i = 0; i < loadOutSlotsAtLevel(ent.respawn_level); ++i)
        ent.set_inventory(i, frand() * (float) PetalID::kNumPetals);
    camera = ent.id;
}

void Client::remove() {
    if (game_server->simulation.ent_exists(camera)) {
        Entity &c = game_server->simulation.get_ent(camera);
        if (game_server->simulation.ent_exists(c.player))
            game_server->simulation.request_delete(c.player);
        game_server->simulation.request_delete(camera);
    }
    std::cout << "deleting client " << this << "\n";
}

uint8_t Client::alive() {
    Simulation &simulation = game_server->simulation;
    return simulation.ent_exists(camera) 
    && simulation.ent_exists(simulation.get_ent(camera).player);
}