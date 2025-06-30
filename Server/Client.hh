#pragma once

#include <Shared/EntityDef.hh>

#include <set>
#include <cstdint>
#include <string>

#ifdef WASM_SERVER
class WebSocket;
#else
#include <App.h>
class Client;
typedef uWS::WebSocket<false, true, Client> WebSocket;
#endif

class Simulation;

class Client {
public:
    Simulation *simulation;
    EntityID camera;
    std::set<EntityID> last_in_view;
    WebSocket *ws;
    uint8_t verified = 0;
    uint8_t seen_arena = 0;
    Client();
    void init();
    void remove();
    void disconnect();
    uint8_t alive();

    void send_packet(uint8_t const *, size_t);
    //void on_message();
    static void on_message(WebSocket *, std::string_view, uint64_t);
    static void on_disconnect(WebSocket *, int, std::string_view);
};

#ifdef WASM_SERVER
class WebSocket {
    int ws_id;
public:
    Client client;
    WebSocket(int);
    Client *getUserData();
    void send(uint8_t const *, size_t);
    void end();
};
#endif