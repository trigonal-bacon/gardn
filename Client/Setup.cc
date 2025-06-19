#include <Client/Setup.hh>

#include <Client/DOM.hh>
#include <Client/Game.hh>
#include <Client/Input.hh>
#include <Client/Storage.hh>
#include <emscripten.h>

extern "C" {
    void mouse_event(float x, float y, uint8_t type, uint8_t button) {
        Input::mouse_x = x;
        Input::mouse_y = y;
        if (type == 0) {
            ++Input::num_touches;
            Input::mouse_buttons_pressed |= 1 << button;
            Input::mouse_buttons_state |= 1 << button;
        }
        else if (type == 2) {
            --Input::num_touches;
            Input::mouse_buttons_released |= 1 << button;
            Input::mouse_buttons_state &= ~(1 << button);
        }
    }
    void key_event(char button, uint8_t type) {
        if (type == 0) {
            Input::keys_pressed.insert(button);
            Input::keys_pressed_this_tick.insert(button);
        }
        else if (type == 1) Input::keys_pressed.erase(button);
    }
    void wheel_event(float wheel) {
        Input::wheel_delta = wheel;
    }
    void loop(double d, float width, float height) {
        Game::renderer.width = width;
        Game::renderer.height = height;
        Game::tick(d);
    }
}

int setup_inputs() {
    EM_ASM({
        window.addEventListener("keydown", (e) => {
            //e.preventDefault();
            _key_event(e.which, 0);
        });
        window.addEventListener("keyup", (e) => {
            //e.preventDefault();
            _key_event(e.which, 1);
        });
        window.addEventListener("mousedown", (e) => {
            //e.preventDefault();
            _mouse_event(e.clientX * devicePixelRatio, e.clientY * devicePixelRatio, 0, +!!e.button);
        });
        window.addEventListener("mousemove", (e) => {
            //e.preventDefault();
            _mouse_event(e.clientX * devicePixelRatio, e.clientY * devicePixelRatio, 1, +!!e.button);
        });
        window.addEventListener("mouseup", (e) => {
            //e.preventDefault();
            _mouse_event(e.clientX * devicePixelRatio, e.clientY * devicePixelRatio, 2, +!!e.button);
        });
        window.addEventListener("touchstart", (e) => {
            //e.preventDefault();
            const t = e.changedTouches[0];
            _mouse_event(t.clientX * devicePixelRatio, t.clientY * devicePixelRatio, 0, 0);
        });
        window.addEventListener("touchmove", (e) => {
            //e.preventDefault();
            const t = e.changedTouches[0];
            _mouse_event(t.clientX * devicePixelRatio, t.clientY * devicePixelRatio, 1, 0);
        });
        window.addEventListener("touchend", (e) => {
            //e.preventDefault();
            const t = e.changedTouches[0];
            _mouse_event(t.clientX * devicePixelRatio, t.clientY * devicePixelRatio, 2, 0);
        });
        window.addEventListener("wheel", (e) => {
            //e.preventDefault();
            _wheel_event(e.deltaY);
        });
    });
    return 0;
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

int setup_canvas() {
    EM_ASM({
        Module.canvas = document.getElementById("canvas");
        Module.canvas.id = "canvas";
        Module.canvas.width = innerWidth * devicePixelRatio;
        Module.canvas.height = innerHeight * devicePixelRatio;
        Module.canvas.oncontextmenu = function() { return false; };
        window.onbeforeunload = function(e) { return "Are you sure?"; };
        //document.body.appendChild(Module.canvas);
        Module.ctxs = [Module.canvas.getContext('2d')];
        Module.availableCtxs =
            new Array(256).fill(0).map(function(_, i) { return i; });
        Module.TextDecoder = new TextDecoder('utf8');
    });
    return 0;
}

uint8_t check_mobile() {
    return EM_ASM_INT({
        return /iPhone|iPad|iPod|Android|BlackBerry/i.test(navigator.userAgent);
    });
}