#include <Server/Client.hh>

#include <Shared/Simulation.hh>

#include <Server/Server.hh>

Client::Client() {
}

void Client::init(Server *server) {
    Entity &ent = server->simulation.alloc_ent();
    ent.add_component(kCamera);
    ent.set_fov(1.0f);
    ent.set_camera_x(frand() * ARENA_WIDTH);
    ent.set_camera_y(frand() * ARENA_HEIGHT);
    camera = ent.id;
}

void Client::remove() {
    /*
    if (global_server.simulation.ent_exists(camera)) {
        global_server.simulation.request_delete(camera);
    }
    std::cout << "deleting client " << this << "\n";
    */
}

uint8_t Client::alive() {
    Simulation &simulation = game_server->simulation;
    return simulation.ent_exists(camera) 
    && simulation.ent_exists(simulation.get_ent(camera).player);
}