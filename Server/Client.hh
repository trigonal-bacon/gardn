#pragma once

#include <Shared/EntityDef.hh>

#include <set>
#include <stdint.h>
#include <App.h>

class Client {
public:
    uint8_t verified = 0;
    EntityID camera;
    std::set<EntityID> last_in_view;
    uWS::WebSocket<false, true, Client> *ws;
    Client();
    void init();
    void remove();
    void disconnect();
    uint8_t alive();
    //void on_message();
};