#pragma once

#include <Shared/Simulation.hh>

#include <App.h>

#include <set>

class Client;

static const int max_buffer_size = 1024 * 1024;
extern uint8_t OUTGOING_PACKET[max_buffer_size];

class Server {
public:
    Simulation simulation;
    uWS::App *server;
    std::set<Client *> clients;
    Server();
    void run();
    void tick();
};


extern Server *game_server;