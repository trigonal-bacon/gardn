#include <Client/Input.hh>

#include <Client/Game.hh>

namespace Input {
    float mouse_x = 0;
    float mouse_y = 0;
    float wheel_delta = 0;
    float prev_mouse_x = 0;
    float prev_mouse_y = 0;
    int num_touches = 0;
    uint8_t mouse_buttons_state = 0;
    uint8_t mouse_buttons_pressed = 0;
    uint8_t mouse_buttons_released = 0;
    uint8_t freeze_input = 0;
    uint8_t movement_helper = 0;
    uint8_t keyboard_movement = 0;
    std::unordered_set<char> keys_held;
    std::unordered_set<char> keys_held_this_tick;
    //std::vector<uint32_t> keys_pressed_this_tick;
    std::string clipboard;

    float input_x = 0;
    float input_y = 0;
    uint8_t input_flags = 0;
};

uint8_t Input::is_valid() {
    return Input::num_touches > 0 || BitMath::at(Input::mouse_buttons_released, Input::LeftMouse) || !Game::is_mobile;
}

void Input::reset() {
    Input::keys_held_this_tick.clear();
    //Input::keys_pressed_this_tick.clear();
    Input::mouse_buttons_pressed = Input::mouse_buttons_released = 0;
    Input::prev_mouse_x = Input::mouse_x;
    Input::prev_mouse_y = Input::mouse_y;
    Input::wheel_delta = 0;
}