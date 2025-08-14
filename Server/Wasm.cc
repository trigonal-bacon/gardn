#ifdef WASM_SERVER
#include <Server/Client.hh>
#include <Server/PetalTracker.hh>
#include <Server/Server.hh>

#include <Shared/Config.hh>
#include <Shared/Map.hh>

#include <iostream>
#include <map>
#include <string>

#include <emscripten.h>

std::map<int, WebSocket *> WS_MAP;

size_t const MAX_BUFFER_LEN = 1024;
static uint8_t INCOMING_BUFFER[MAX_BUFFER_LEN] = {0};

extern "C" {
    void on_connect(int ws_id) {
        std::printf("client connection with id %d\n", ws_id);
        WebSocket *ws = new WebSocket(ws_id);
    }

    void on_disconnect(int ws_id, int reason) {
        WebSocket *ws = WS_MAP[ws_id];
        if (ws == nullptr) return;
        Client::on_disconnect(ws,reason, {});
    }

    void tick() {
        Server::tick();
    }

    void on_message(int ws_id, uint32_t len) {
        WebSocket *ws = WS_MAP[ws_id];
        if (ws == nullptr) return;
        std::string_view message(reinterpret_cast<char const *>(INCOMING_BUFFER), len);
        Client::on_message(ws, message, 0);
    }

    bool restore_player(EntityID::hash_type hash, EntityID::id_type id, uint32_t score, PetalID::T *loadout_ids) {
        Simulation *sim = &Server::game.simulation;
        if (id >= ENTITY_CAP) return false;
        EntityID player_id = EntityID(id, hash);
        if (!sim->ent_alive(player_id)) return false;
        Entity &player = sim->get_ent(player_id);
        if (!player.has_component(kFlower) || player.has_component(kMob)) return false;
        uint32_t loadout_count = loadout_slots_at_level(score_to_level(score));
        for (uint32_t i = 0; i < loadout_count + MAX_SLOT_COUNT; ++i)
            if (loadout_ids[i] >= PetalID::kNumPetals) return false;

        player.set_score(score);
        uint32_t difficulty = MAP[Map::get_zone_from_pos(player.x, player.y)].difficulty;
        uint32_t power = Map::difficulty_at_level(score_to_level(score));
        if (difficulty < power) {
            ZoneDefinition const &zone = MAP[Map::get_suitable_difficulty_zone(power)];
            player.set_x(zone.x - 0.49 * zone.w);
        }
        for (uint32_t i = 0; i < loadout_count + MAX_SLOT_COUNT; ++i) {
            PetalTracker::remove_petal(sim, player.loadout_ids[i]);
            player.set_loadout_ids(i, loadout_ids[i]);
            PetalTracker::add_petal(sim, loadout_ids[i]);
        }
        for (uint32_t i = 0; i < loadout_count; ++i) {
            LoadoutSlot &slot = player.loadout[i];
            slot.update_id(sim, loadout_ids[i]);
            slot.force_reload();
        }
        return true;
    }
}

WebSocketServer::WebSocketServer() {
    EM_ASM({
        const WSS = require("ws");
        const http = require("http");
        const fs = require("fs");
        const server = http.createServer(function(req, res) {
            let encodeType = "text/html";
            let file = "index.html";
            switch (req.url) {
                case "/":
                    break;
                case "/gardn-client.js":
                    encodeType = "application/javascript";
                    file = "gardn-client.js";
                    break;
                case "/gardn-client.wasm":
                    encodeType = "application/wasm";
                    file = "gardn-client.wasm";
                    break;
                default:
                    file = "";
                    break;
            }
            if (fs.existsSync(file)) {            
                res.writeHead(200, {"Content-Type": encodeType});
                res.end(fs.readFileSync(file));
                return;
            }
            res.writeHead(404, {"Content-Type": encodeType});
            res.end();
        });

        server.listen($0, function() {
            console.log("Server running at http://localhost:"+$0);
        });
        
        const wss = new WSS.Server({ "server": server });
        Module.ws_connections = {};
        let curr_id = 0;
        wss.on("connection", function(ws, req){
            const ws_id = curr_id;
            Module.ws_connections[ws_id] = ws;
            _on_connect(ws_id);
            curr_id = (curr_id + 1) | 0;
            ws.on("message", function(message){
                let data = new Uint8Array(message);
                const len = data.length > $2 ? $2 : data.length;
                data = data.subarray(0, len);
                HEAPU8.set(data, $1);
                _on_message(ws_id, len);
            });
            ws.on("close", function(reason){
                _on_disconnect(ws_id, reason);
                delete Module.ws_connections[ws_id];
            });
        })
    }, SERVER_PORT, INCOMING_BUFFER, MAX_BUFFER_LEN);
}

void Server::run() {
    EM_ASM({
        globalThis.Module = Module;
        setInterval(_tick, $0);
    }, 1000 / TPS);
}

void Client::send_packet(uint8_t const *packet, size_t size) {
    if (ws == nullptr) return;
    ws->send(packet, size);
}

WebSocket::WebSocket(int id) : ws_id(id) {
    //client.init();
    client.ws = this;
    WS_MAP.insert({id, this});
}

void WebSocket::send(uint8_t const *packet, size_t size) {
    EM_ASM({
        if (!Module.ws_connections || !Module.ws_connections[$0]) return;
        const ws = Module.ws_connections[$0];
        ws.send(HEAPU8.subarray($1,$1+$2));
    }, ws_id, packet, size);
}

void WebSocket::end() {
    EM_ASM({
        if (!Module.ws_connections || !Module.ws_connections[$0]) return;
        const ws = Module.ws_connections[$0];
        ws.close();
    }, ws_id);
}

Client *WebSocket::getUserData() {
    return &client;
}

WebSocketServer Server::server;
#endif
