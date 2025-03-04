#pragma once

#include <Client/Render/Renderer.hh>

#include <stdint.h>

void draw_static_petal_single(uint8_t, Renderer &);

void draw_static_petal(uint8_t, Renderer &);

void draw_static_mob(uint8_t, Renderer &, float, float, uint32_t);