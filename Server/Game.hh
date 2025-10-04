#pragma once

#include <Server/TeamManager.hh>

#include <Shared/Simulation.hh>

#include <set>
#include <vector>

class Client;

class GameInstance {
    std::set<Client *> clients;
    TeamManager team_manager;
    std::vector<EntityID> bots;
    void spawn_bots(uint32_t count);
public:
    Simulation simulation;
    GameInstance();
    void init();
    void tick();
    void add_client(Client *, uint64_t);
    void remove_client(Client *);
};