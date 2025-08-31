#pragma once

#include <cstdint>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

namespace Input {
    enum MouseButton {
        LeftMouse = 0,
        RightMouse = 1
    };

    struct Touch {
        uint32_t id;
        float x = 0;
        float y = 0;
        float dx = 0;
        float dy = 0;
        uint8_t saturated = 0;
    };

    struct GameInput {
        float x;
        float y;
        uint8_t flags;
    };

    extern GameInput game_inputs;
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
    extern uint8_t is_mobile;
    //use these for game inputs that can be held down
    extern std::unordered_set<char> keys_held;
    extern std::unordered_set<char> keys_held_this_tick;
    extern std::unordered_map<uint32_t, Touch> touches;
    //use these for text input
    //extern std::vector<uint32_t> keys_pressed_this_tick;
    extern std::string clipboard;

    extern float input_x;
    extern float input_y;

    extern uint8_t is_valid();
    extern void reset();
};

