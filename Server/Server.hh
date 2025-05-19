#pragma once

#include <Shared/Simulation.hh>

#include <App.h>

#include <set>

class Client;

static const int max_buffer_size = 1024 * 1024;

namespace Server {
    extern uint8_t OUTGOING_PACKET[max_buffer_size];
    extern Simulation simulation;
    extern uWS::App *socket;
    extern std::set<Client *> clients;
    void init();
    void run();
    void tick();
};


//extern Server *game_server;