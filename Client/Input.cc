#include <Client/Input.hh>

namespace Input {
    float mouse_x = 0;
    float mouse_y = 0;
    float wheel_delta = 0;
    float prev_mouse_x = 0;
    float prev_mouse_y = 0;
    uint8_t mouse_buttons_state = 0;
    uint8_t mouse_buttons_pressed = 0;
    uint8_t mouse_buttons_released = 0;
    uint8_t freeze_input = 0;
    uint8_t keyboard_movement = 0;
    std::unordered_set<char> keys_pressed;
    std::unordered_set<char> keys_pressed_this_tick;
};
