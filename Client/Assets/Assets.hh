#pragma once

#include <Client/Render/Renderer.hh>

#include <stdint.h>

class MobRenderAttributes {
public:
    float animation;
    float radius;
    uint32_t seed;
    uint32_t flags;
};

void draw_loadout_background(Renderer &, uint32_t);

void draw_static_petal_single(uint8_t, Renderer &);

void draw_static_petal(uint8_t, Renderer &);

void draw_static_mob(uint8_t, Renderer &, MobRenderAttributes);

void draw_web(Renderer &);
