#pragma once

#include <Shared/EntityDef.hh>

#include <set>
#include <stdint.h>
#include <App.h>

class Client {
public:
    EntityID camera;
    std::set<EntityID> last_in_view;
    uWS::WebSocket<false, true, Client> *ws;
    Client();
    void init();
    void remove();
    uint8_t alive();
    //void on_message();
};