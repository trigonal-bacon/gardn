#pragma once

#include <Client/Render/Renderer.hh>

#include <Shared/StaticData.hh>

namespace Particle {
    class TitleParticleEntity {
    public:
        float x;
        float y;
        float x_velocity;
        float angle;
        float sin_offset;
        float radius;
        PetalID::T id;
    };

    class GameParticleEntity {
    public:
        float x;
        float y;
        float x_velocity;
        float y_velocity;
        float radius;
        float opacity;
    };

    void tick_title(Renderer &, double);
    void tick_game(Renderer &, double);
    void add_unique_particle(float, float);
}