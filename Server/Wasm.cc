#ifdef WASM_SERVER
#include <Server/Client.hh>
#include <Server/Server.hh>

#include <Shared/StaticData.hh>

#include <emscripten.h>

#include <map>
#include <string>

std::map<int, WebSocket *> WS_MAP;

uint32_t const MAX_BUFFER_LEN = 8 * 1024;
static uint8_t INCOMING_BUFFER[MAX_BUFFER_LEN] = {0};

extern "C" {
    void on_connect(int ws_id) {
        std::cout << "client conection\n";
        WebSocket *ws = new WebSocket(ws_id);
        Server::clients.insert(&ws->client);
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
}

WebSocketServer::WebSocketServer() {
    int const port = 9001;
    EM_ASM({
        const WSS = require("ws");
        const http = require("http");
        const server = http.createServer(function(req, res) {
            res.writeHead(200, {"Content-Type": "text/plain"});
            res.write("Hello World!");
            res.end();
        });

        server.listen($0, function() {
            console.log("Server running at http://localhost:"+$0);
        });
        
        const wss = new WSS.Server({server});
        Module.ws_connections = {};
        let curr_id = 0;
        wss.on("connection", function(ws, req){
            const ws_id = curr_id;
            Module.ws_connections[ws_id] = ws;
            Module._on_connect(ws_id);
            curr_id = (curr_id + 1) | 0;
            ws.on("message", function(message){
                let data = new Uint8Array(message);
                const len = data.length > $2 ? $2 : data.length;
                data = data.subarray(0, len);
                Module.HEAPU8.set(data, $1);
                Module._on_message(ws_id, len);
            });
            ws.on("close", function(reason){
                Module._on_disconnect(ws_id, reason);
                delete Module.ws_connections[ws_id];
            });
        })
    }, port, INCOMING_BUFFER, MAX_BUFFER_LEN);
}

void Server::run() {
    EM_ASM({
        setInterval(Module._tick, $0);
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
        ws.send(Module.HEAPU8.subarray($1,$1+$2));
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
