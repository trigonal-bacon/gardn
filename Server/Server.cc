#include <Server/Server.hh>

#include <Server/Game.hh>
#include <Server/Client.hh>

#include <Shared/Binary.hh>

#include <chrono>
#include <iostream>

namespace Server {
    uint8_t OUTGOING_PACKET[MAX_PACKET_LEN] = {0};
    GameInstance game;
}

using namespace Server;

void Server::tick() {
    using namespace std::chrono_literals;
    auto start = std::chrono::steady_clock::now();
    Server::game.tick();
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> tick_time = end - start;
    if (tick_time > 5ms) std::cout << "tick took " << tick_time << '\n';
}

void Server::init() {
    Server::game.init();
    Server::run();
}
