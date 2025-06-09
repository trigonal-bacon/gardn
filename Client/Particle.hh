#pragma once

#include <Client/Render/Renderer.hh>

#include <Shared/StaticData.hh>
#include <Shared/Vector.hh>

namespace Particle {
    class ParticleEntity {
    public:
        float x;
        float y;
        float x_velocity;
        float angle;
        float sin_offset;
        float radius;
        PetalID::T id;
    };

    void tick(Renderer &, double);
}