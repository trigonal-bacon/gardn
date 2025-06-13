#ifndef WASM_SERVER
#include <Server/Server.hh>

#include <Server/Client.hh>
#include <Server/PetalTracker.hh>
#include <Server/Spawn.hh>

#include <Shared/Map.hh>
#include <Shared/Vector.hh>

#include <chrono>
#include <cmath>
#include <thread>

uWS::App Server::server = uWS::App({
    .key_file_name = "misc/key.pem",
    .cert_file_name = "misc/cert.pem",
    .passphrase = "1234"
}).ws<Client>("/*", {
    /* Settings */
    .compression = uWS::CompressOptions(uWS::DEDICATED_COMPRESSOR_4KB | uWS::DEDICATED_DECOMPRESSOR),
    .maxPayloadLength = 1024 * 1024,
    .idleTimeout = 16,
    .maxBackpressure = 100 * 1024 * 1024,
    .closeOnBackpressureLimit = false,
    .resetIdleTimeoutOnSend = false,
    .sendPingsAutomatically = true,
    /* Handlers */
    .upgrade = nullptr,
    .open = [](WebSocket *ws) {
        std::cout << "client connection\n";
        Client *client = ws->getUserData();
        client->ws = ws;
        Server::clients.insert(client);
    },
    .message = [](WebSocket *ws, std::string_view message, uWS::OpCode opCode) {
        Client::on_message(ws, message, opCode);
    },
    .dropped = [](WebSocket *ws, std::string_view /*message*/, uWS::OpCode /*opCode*/) {
        std::cout << "dropped packet, uh oh\n";
        Client *client = ws->getUserData();
        if (client == nullptr) {
            ws->end();
            return;
        }
        client->disconnect();
        //ws->end();
        /* A message was dropped due to set maxBackpressure and closeOnBackpressureLimit limit */
    },
    .drain = [](WebSocket */*ws*/) {
        //assert(!1);
        /* Check ws->getBufferedAmount() here */
    },
    .close = [](WebSocket *ws, int code, std::string_view message) {
        Client::on_disconnect(ws, code, message);
    }
}).listen(9001, [](auto *listen_socket) {
    if (listen_socket) {
        std::cout << "Listening on port " << 9001 << std::endl;
    }
});

void Server::run() {
    struct us_loop_t *loop = (struct us_loop_t *) uWS::Loop::get();
    struct us_timer_t *delayTimer = us_create_timer(loop, 0, 0);

    us_timer_set(delayTimer, [](us_timer_t *x){ Server::tick(); }, 1, 1000 / TPS);
    Server::server.run();
}

void Client::send_packet(uint8_t const *packet, size_t size) {
    if (ws == nullptr) return;
    std::string_view message(reinterpret_cast<char const *>(packet), size);
    ws->send(message, uWS::OpCode::BINARY, 0);
}
#endif