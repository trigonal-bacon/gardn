#include <Server/Server.hh>

#include <Server/Client.hh>

#include <chrono>

namespace Server {
    uint8_t OUTGOING_PACKET[max_buffer_size] = {0};
    Simulation simulation;
    std::set<Client *> clients;
}

using namespace Server;

void Server::tick() {
    struct timespec ts;
    struct timespec te;
    timespec_get(&ts, TIME_UTC);
    Server::simulation.tick();
    timespec_get(&te, TIME_UTC);

    double mss = ts.tv_sec * 1000 + ts.tv_nsec / 1000000.0;
    double mse = te.tv_sec * 1000 + te.tv_nsec / 1000000.0;
    if (mse - mss > 10) std::cout << "tick took " << (mse - mss) << "ms\n";
}

void Server::init() {
    for (uint32_t i = 0; i < 10000; ++i)
        Map::spawn_random_mob(&Server::simulation);
    Server::run();
}
