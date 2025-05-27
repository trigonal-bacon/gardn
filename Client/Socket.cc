#include <Client/Socket.hh>
#include <Client/Game.hh>

#include <Shared/Binary.hh>

#include <emscripten.h>

uint8_t INCOMING_PACKET[1024 * 1024] = {0};

extern "C" {
    void on_message(uint8_t type, uint32_t len) {
        if (type == 0) {
            Writer w(INCOMING_PACKET);
            w.write_uint8(kServerbound::kVerify);
            w.write_uint64(VERSION_HASH);
            Game::socket.ready = 1;
            Game::on_game_screen = 0;
            Game::socket.send(&INCOMING_PACKET[0], w.at - w.packet);
            Game::socket.ready = 0;
        } 
        else if (type == 2) { 
            Game::on_game_screen = 0;
            Game::socket.ready = Game::simulation_ready = 0;
            Game::camera_id = NULL_ENTITY;
            Game::simulation.reset();
        }
        else if (type == 1) {
            Game::socket.ready = 1;
            Game::on_message(static_cast<uint8_t *>(INCOMING_PACKET), len);
        }
    }
}

Socket::Socket() {}

void Socket::connect(char const *url) {
    EM_ASM({
        let string = UTF8ToString($1);
        function connect() {
            let socket = Module.socket = new WebSocket(string);
            socket.binaryType = "arraybuffer";
            socket.onopen = function()
            {
                console.log("Connected");
                _on_message(0);
            };
            socket.onclose = function(a)
            {
                console.log("Disconnected");
                _on_message(2, a.code);
                setTimeout(connect, 1000);
            };
            socket.onmessage = function(event)
            {
                HEAPU8.set(new Uint8Array(event.data), $0);
                _on_message(1, event.data.length);
            };
        }
        setTimeout(connect, 1000);
    }, INCOMING_PACKET, url);
}

void Socket::send(uint8_t *ptr, uint32_t len) {
    if (ready == 0) return; 
    EM_ASM({
        if (Module.socket?.readyState == 1) {
            Module.socket.send(HEAPU8.subarray($0, $0+$1));
        }
    }, ptr, len);
}