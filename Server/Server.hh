#pragma once

#include <Server/Game.hh>

#include <Shared/Config.hh>
#include <set>

class Client;

#ifdef WASM_SERVER
class WebSocketServer {
public:
  WebSocketServer();
};
#else
#include <App.h>
using WebSocketServer = uWS::App;
#endif

namespace Server {
extern uint8_t OUTGOING_PACKET[MAX_CLIENTBOUND_PACKET_LEN];
extern GameInstance game;
extern WebSocketServer server;
extern void init();
extern void run();
extern void tick();
}; // namespace Server