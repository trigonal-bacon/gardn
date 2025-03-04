#include <Client/Game.hh>

#include <stdint.h>
#include <emscripten.h>
#include <iostream>

extern "C" {
    void mouse_event(float x, float y, uint8_t type, uint8_t button) {
        gardn->input.mouse_x = x;
        gardn->input.mouse_y = y;
        if (type == 0) {
            gardn->input.mouse_buttons_pressed |= 1 << button;
            gardn->input.mouse_buttons_state |= 1 << button;
        }
        else if (type == 2) {
            gardn->input.mouse_buttons_released |= 1 << button;
            gardn->input.mouse_buttons_state &= ~(1 << button);
        }
    }
    void key_event(char button, uint8_t type) {
        if (type == 0) {
            gardn->input.keys_pressed.insert(button);
            gardn->input.keys_pressed_this_tick.insert(button);
        }
        else if (type == 1) gardn->input.keys_pressed.erase(button);
    }
    void loop(double d, float width, float height) {
        gardn->renderer.width = width;
        gardn->renderer.height = height;
        float a = width / 1920;
        float b = height / 1080;
        gardn->scale = a > b ? a : b;
        gardn->tick(d);
    }
}

void setup_canvas() {
EM_ASM({
    Module.canvas = document.createElement("canvas");
    Module.canvas.id = "canvas";
    Module.canvas.width = innerWidth * devicePixelRatio;
    Module.canvas.height = innerHeight * devicePixelRatio;
    Module.canvas.oncontextmenu = function() { return false; };
    window.onbeforeunload = function(e) { return "Are you sure?"; };
    document.body.appendChild(Module.canvas);
    Module.ctxs = [Module.canvas.getContext('2d')];
    Module.availableCtxs =
        new Array(256).fill(0).map(function(_, i) { return i; });
    Module.TextDecoder = new TextDecoder('utf8');
});
}

void setup_inputs() {
EM_ASM({
    window.addEventListener("keydown", (e) => {
        _key_event(e.which, 0);
    });
    window.addEventListener("keyup", (e) => {
        _key_event(e.which, 1);
    });
    window.addEventListener("mousedown", (e) => {
        _mouse_event(e.clientX * devicePixelRatio, e.clientY * devicePixelRatio, 0, +!!e.button);
    });
    window.addEventListener("mousemove", (e) => {
        _mouse_event(e.clientX * devicePixelRatio, e.clientY * devicePixelRatio, 1, +!!e.button);
    });
    window.addEventListener("mouseup", (e) => {
        _mouse_event(e.clientX * devicePixelRatio, e.clientY * devicePixelRatio, 2, +!!e.button);
    });
});
}

void main_loop() {
EM_ASM({
    function loop(time)
    {
        Module.canvas.width = innerWidth * devicePixelRatio;
        Module.canvas.height = innerHeight * devicePixelRatio;
        _loop(time, Module.canvas.width, Module.canvas.height);
        requestAnimationFrame(loop);
    };
    requestAnimationFrame(loop);
});
}

int main() {
    setup_canvas();
    setup_inputs();
    static Game game;
    main_loop();
    return 0;
}