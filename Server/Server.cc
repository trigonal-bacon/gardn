#include <Server/Server.hh>

#include <Server/Game.hh>
#include <Server/Client.hh>

#include <Shared/Binary.hh>

#include <chrono>
#include <iostream>

static bool was_draining = false;

namespace Server {
    uint8_t OUTGOING_PACKET[MAX_PACKET_LEN] = {0};
    GameInstance game;
    volatile sig_atomic_t is_draining = false;
    bool is_stopping = false;
    uint32_t player_count = 0;
}

using namespace Server;

void Server::tick() {
    using namespace std::chrono_literals;
    auto start = std::chrono::steady_clock::now();
    Server::game.tick();
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> tick_time = end - start;
    if (tick_time > 1000ms / TPS / 2) std::cout << "tick took " << tick_time << '\n';

    if (Server::is_draining && !was_draining) {
        was_draining = true;
        std::cout << "draining...\n";
    }
    if (Server::is_draining && Server::player_count == 0)
        Server::stop();
}
