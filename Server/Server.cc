#include <Server/Server.hh>

#include <Server/Client.hh>
#include <Server/Spawn.hh>
#include <Shared/Vector.hh>

#include <chrono>
#include <cmath>
#include <thread>
#include <stdio.h>

uint8_t OUTGOING_PACKET[max_buffer_size] = {0};
Server *game_server;

Server::Server() : simulation() {}

void Server::run() {
    game_server = this;
    /* Keep in mind that uWS::SSLApp({options}) is the same as uWS::App() when compiled without SSL support.
     * You may swap to using uWS:App() if you don't need SSL */
    uWS::App app = uWS::App({
        /* There are example certificates in uWebSockets.js repo */
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
        .open = [this](auto *ws) {
			std::cout << "client connection\n";
            Client *client = ws->getUserData();
            client->init(this);
            client->ws = ws;
            game_server->clients.insert(client);
        },
        .message = [](auto *ws, std::string_view message, uWS::OpCode opCode) {
            uint8_t const *data = reinterpret_cast<uint8_t const *>(message.data());
            Reader reader(data);
            Client *client = ws->getUserData();
            switch (reader.read_uint8()) {
                case kServerbound::kClientInput: {
                    if (!client->alive()) break;
                    Entity &camera = game_server->simulation.get_ent(client->camera);
                    Entity &player = game_server->simulation.get_ent(camera.player);
                    float x = reader.read_float();
                    float y = reader.read_float();
                    if (x == 0 && y == 0) player.acceleration.set(0,0);
                    else {
                        if (fabsf(x) > 5e3 || fabsf(y) > 5e3) break;
                        Vector accel(x,y);
                        float m = accel.magnitude();
                        if (m > 200) accel.normalize().set_magnitude(PLAYER_ACCELERATION);
                        else accel.normalize().set_magnitude(m / 200 * PLAYER_ACCELERATION);
                        player.acceleration = accel;
                    }
                    player.input = reader.read_uint8() & 3;
                    break;
                }
                case kServerbound::kClientSpawn: {
                    if (client->alive()) break;
                    Entity &camera = game_server->simulation.get_ent(client->camera);
                    Entity &player = alloc_player(camera);
                    player_spawn(&game_server->simulation, camera, player);
                    break;
                }
            }
        },
        .dropped = [](auto *ws, std::string_view /*message*/, uWS::OpCode /*opCode*/) {
            std::cout << "dropped packet, uh oh\n";
            ws->end();
            /* A message was dropped due to set maxBackpressure and closeOnBackpressureLimit limit */
        },
        .drain = [](auto */*ws*/) {
            assert(!1);
            /* Check ws->getBufferedAmount() here */
        },
        .close = [](auto *ws, int /*code*/, std::string_view /*message*/) {
            /* You may access ws->getUserData() here */
            std::cout << "client disconnection\n";
            Client *client = ws->getUserData();
            client->remove();
            game_server->clients.erase(client);
            //delete player in systems
        }
    }).listen(9001, [](auto *listen_socket) {
        if (listen_socket) {
            std::cout << "Listening on port " << 9001 << std::endl;
        }
    });
    server = &app;
    struct us_loop_t *loop = (struct us_loop_t *) uWS::Loop::get();
    struct us_timer_t *delayTimer = us_create_timer(loop, 0, 0);


    us_timer_set(delayTimer, [](struct us_timer_t *t) {

        struct timespec ts;
        struct timespec te;
        timespec_get(&ts, TIME_UTC);
        game_server->simulation.tick();
        //update all clients
        timespec_get(&te, TIME_UTC);

        double mss = ts.tv_sec * 1000 + ts.tv_nsec / 1000000.0;
        double mse = te.tv_sec * 1000 + te.tv_nsec / 1000000.0;
        if (mse - mss > 10) std::cout << "tick took " << (mse - mss) << "ms\n";

    }, 1000 / TPS, 1000 / TPS);
    app.run();
}