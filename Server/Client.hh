#pragma once

#include <Shared/EntityDef.hh>

#include <set>
#include <stdint.h>
#include <App.h>

class Server; 

class Client {
public:
    EntityId camera;
    std::set<EntityId> last_in_view;
    uWS::WebSocket<false, true, Client> *ws;
    Client();
    void init(Server *);
    void remove();
    uint8_t alive();
    //void on_message();
};