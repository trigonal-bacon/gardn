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
            Input::mouse_buttons_pressed |= 1 << button;
            Input::mouse_buttons_state |= 1 << button;
        }
        else if (type == 2) {
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

template<char const *c>
char p(){
    return *c;
};

void setup_localstorage() {
    //for (PetalID::T i = 0; i < PetalID::kNumPetals; ++i) Game::seen_petals[i] = frand() * 2;
    //for (MobID::T i = 0; i < MobID::kNumMobs; ++i) Game::seen_mobs[i] = frand() * 2;
    Game::seen_petals[PetalID::kBasic] = 1;
    {
        uint32_t len = Storage::retrieve("mobs", 256);
        Reader reader(&Storage::buffer[0]);
        while (reader.at < Storage::buffer + len) {
            MobID::T mob_id = reader.read<uint8_t>();
            if (mob_id >= MobID::kNumMobs) break;
            Game::seen_mobs[mob_id] = 1;
        }
    }
    {
        uint32_t len = Storage::retrieve("petals", 256);
        Reader reader(&Storage::buffer[0]);
        while (reader.at < Storage::buffer + len) {
            PetalID::T petal_id = reader.read<uint8_t>();
            if (petal_id >= PetalID::kNumPetals || petal_id == PetalID::kNone) break;
            Game::seen_petals[petal_id] = 1;
        }
    }
    {
        uint32_t len = Storage::retrieve("settings", 1);
        if (len > 0) {
            Reader reader(&Storage::buffer[0]);
            uint8_t opts = reader.read<uint8_t>();
            Input::movement_helper = BIT_AT(opts, 0);
            Input::keyboard_movement = BIT_AT(opts, 1);
        }
    }
    {
        uint32_t len = Storage::retrieve("nickname", 72);
        if (len > 0) {
            Reader reader(&Storage::buffer[0]);
            std::string name;
            reader.read<std::string>(name);
            DOM::update_text("t0", name, 16);
        }
    }
}