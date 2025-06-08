#pragma once

#include <unordered_set>
#include <stdint.h>

namespace Input {
    enum MouseButton {
        LeftMouse = 0,
        RightMouse = 1
    };
    extern float mouse_x;
    extern float mouse_y;
    extern float wheel_delta;
    extern float prev_mouse_x;
    extern float prev_mouse_y;
    extern uint8_t mouse_buttons_state;
    extern uint8_t mouse_buttons_pressed;
    extern uint8_t mouse_buttons_released;
    extern uint8_t freeze_input;
    extern uint8_t movement_helper;
    extern uint8_t keyboard_movement;
    extern std::unordered_set<char> keys_pressed;
    extern std::unordered_set<char> keys_pressed_this_tick;
};

