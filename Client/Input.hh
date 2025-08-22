#pragma once

#include <cstdint>
#include <string>
#include <unordered_set>
#include <vector>

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
    extern int num_touches;
    extern uint8_t mouse_buttons_state;
    extern uint8_t mouse_buttons_pressed;
    extern uint8_t mouse_buttons_released;
    extern uint8_t freeze_input;
    extern uint8_t movement_helper;
    extern uint8_t keyboard_movement;
    //use these for game inputs that can be held down
    extern std::unordered_set<char> keys_held;
    extern std::unordered_set<char> keys_held_this_tick;
    //use these for text input
    //extern std::vector<uint32_t> keys_pressed_this_tick;
    extern std::string clipboard;

    extern float input_x;
    extern float input_y;
    extern uint8_t input_flags;

    extern uint8_t is_valid();
    extern void reset();
};

