#include <Client/Game.hh>
#include <Client/Input.hh>
#include <Client/Setup.hh>

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
    void loop(double d, float width, float height) {
        Game::renderer.width = width;
        Game::renderer.height = height;
        Game::tick(d);
    }
}

int main() {
    //setup_canvas();
    //setup_inputs();
    //static Game game;
    Game::init();
    main_loop();
    return 0;
}