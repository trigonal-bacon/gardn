#include <Server/Server.hh>

#include <Server/Client.hh>
#include <Server/Spawn.hh>
#include <Shared/Vector.hh>

#include <chrono>
#include <cmath>
#include <thread>

namespace Server {
    uint8_t OUTGOING_PACKET[max_buffer_size] = {0};
    Simulation simulation;
    uWS::App *socket = nullptr;
    std::set<Client *> clients;
}
//Server *game_server;
using namespace Server;

void Server::run() {
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
        .open = [](auto *ws) {
			std::cout << "client connection\n";
            Client *client = ws->getUserData();
            client->init();
            client->ws = ws;
            Server::clients.insert(client);
        },
        .message = [](auto *ws, std::string_view message, uWS::OpCode opCode) {
            uint8_t const *data = reinterpret_cast<uint8_t const *>(message.data());
            Reader reader(data);
            Client *client = ws->getUserData();
            if (client == nullptr) {
                ws->end();
                return;
            }
            if (!client->verified) {
                if (reader.read_uint8() != kServerbound::kVerify) {
                    //disconnect
                    client->disconnect();
                    return;
                }
                if (reader.read_uint64() != VERSION_HASH) {
                    client->disconnect();
                    return;
                }
                client->verified = 1;
                return;
            }
            switch (reader.read_uint8()) {
                case kServerbound::kVerify:
                    client->disconnect();
                    return;
                case kServerbound::kClientInput: {
                    if (!client->alive()) break;
                    Entity &camera = Server::simulation.get_ent(client->camera);
                    Entity &player = Server::simulation.get_ent(camera.player);
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
                    Entity &camera = Server::simulation.get_ent(client->camera);
                    Entity &player = alloc_player(camera);
                    player_spawn(&Server::simulation, camera, player);
                    break;
                }
                case kServerbound::kPetalDelete: {
                    if (!client->alive()) break;
                    Entity &camera = Server::simulation.get_ent(client->camera);
                    Entity &player = Server::simulation.get_ent(camera.player);
                    uint8_t pos = reader.read_uint8();
                    if (pos >= MAX_SLOT_COUNT + player.loadout_count) break;
                    PetalID::T old_id = player.loadout_ids[pos];
                    if (old_id != PetalID::kNone && old_id != PetalID::kBasic) {
                        uint8_t rarity = PETAL_DATA[old_id].rarity;
                        float rarity_to_xp[RarityID::kNumRarities] = { 1, 4, 15, 100, 500, 5000 };
                        player.set_score(player.score + rarity_to_xp[rarity]);
                    }
                    player.set_loadout_ids(pos, PetalID::kNone);
                    //std::cout << "Deleted pos " << (int) pos << '\n';
                    break;
                }
                case kServerbound::kPetalSwap: {
                    if (!client->alive()) break;
                    Entity &camera = Server::simulation.get_ent(client->camera);
                    Entity &player = Server::simulation.get_ent(camera.player);
                    uint8_t pos1 = reader.read_uint8();
                    if (pos1 >= MAX_SLOT_COUNT + player.loadout_count) break;
                    uint8_t pos2 = reader.read_uint8();
                    if (pos2 >= MAX_SLOT_COUNT + player.loadout_count) break;
                    PetalID::T tmp = player.loadout_ids[pos1];
                    player.set_loadout_ids(pos1, player.loadout_ids[pos2]);
                    player.set_loadout_ids(pos2, tmp);
                    //std::cout << "Deleted pos " << (int) pos << '\n';
                    break;
                }
            }
        },
        .dropped = [](auto *ws, std::string_view /*message*/, uWS::OpCode /*opCode*/) {
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
        .drain = [](auto */*ws*/) {
            //assert(!1);
            /* Check ws->getBufferedAmount() here */
        },
        .close = [](auto *ws, int /*code*/, std::string_view /*message*/) {
            /* You may access ws->getUserData() here */
            std::cout << "client disconnection\n";
            Client *client = ws->getUserData();
            client->remove();
            Server::clients.erase(client);
            //delete player in systems
        }
    }).listen(9001, [](auto *listen_socket) {
        if (listen_socket) {
            std::cout << "Listening on port " << 9001 << std::endl;
        }
    });
    
    Server::socket = &app;

    struct us_loop_t *loop = (struct us_loop_t *) uWS::Loop::get();
    struct us_timer_t *delayTimer = us_create_timer(loop, 0, 0);

    us_timer_set(delayTimer, [](struct us_timer_t *t) {
        struct timespec ts;
        struct timespec te;
        timespec_get(&ts, TIME_UTC);
        Server::simulation.tick();
        //update all clients
        timespec_get(&te, TIME_UTC);

        double mss = ts.tv_sec * 1000 + ts.tv_nsec / 1000000.0;
        double mse = te.tv_sec * 1000 + te.tv_nsec / 1000000.0;
        if (mse - mss > 10) std::cout << "tick took " << (mse - mss) << "ms\n";
        //std::cout << (ts.tv_nsec / 1000000.0) << '\n';
        //std::cout << (1000 / TPS) << '\n';
    }, 1, 960 / TPS);
    //4% boost since the timer is slow
    /*
    std::thread([]() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        while (1) {
            struct timespec ts;
            struct timespec te;
            timespec_get(&ts, TIME_UTC);
            Server::simulation.tick();
            //update all clients
            timespec_get(&te, TIME_UTC);

            double mss = ts.tv_sec * 1000 + ts.tv_nsec / 1000000.0;
            double mse = te.tv_sec * 1000 + te.tv_nsec / 1000000.0;
            if (mse - mss > 10) std::cout << "tick took " << (mse - mss) << "ms\n";
            if ((1000 / TPS) > (mse - mss))
                std::this_thread::sleep_for(std::chrono::microseconds((int) ((1000 / TPS) - (mse - mss)) * 1000));
        }
    });
    */
    app.run();
}