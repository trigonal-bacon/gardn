#pragma once

#include <unordered_set>
#include <stdint.h>

class Input {
public:
    float mouse_x;
    float mouse_y;
    float prev_mouse_x;
    float prev_mouse_y;
    uint8_t mouse_buttons_state;
    uint8_t mouse_buttons_pressed;
    uint8_t mouse_buttons_released;
    std::unordered_set<char> keys_pressed;
    std::unordered_set<char> keys_pressed_this_tick;
    Input();
};