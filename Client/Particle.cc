#include <Client/Particle.hh>

#include <Client/Game.hh>

#include <Client/Assets/Assets.hh>

#include <cmath>
#include <vector>

using namespace Particle;

static std::vector<ParticleEntity> particles;

void Particle::tick(Renderer &ctx, double dt) {
    RenderContext c(&ctx);
    ctx.reset_transform();
    size_t len = particles.size();
    for (size_t i = len; i > 0; --i) {
        ParticleEntity &part = particles[i - 1];
        if (part.x > ctx.width + 10 * part.radius) {
            part = particles[particles.size() - 1];
            particles.pop_back();
            continue;
        }
        RenderContext c(&ctx);
        part.x += part.x_velocity * (dt / 1000);
        //part.y += dt / 10 * ;
        part.angle += dt / 1000;
        ctx.translate(part.x, part.y + 10 * sin(Game::timestamp / 500 + part.sin_offset));
        ctx.scale(Ui::scale * part.radius);
        ctx.rotate(part.angle);
        draw_static_petal(part.id, ctx);
    }
    for (size_t i = 0; i < 5; ++i) {
        if (frand() > 0.01) continue;
        ParticleEntity npart;
        npart.x = -30;
        npart.id = (float) PetalID::kBasic + frand() * (PetalID::kNumPetals-PetalID::kBasic);
        npart.y = frand() * ctx.height;
        npart.angle = frand() * 2 * M_PI;
        npart.x_velocity = frand() * 50 + 150;
        npart.sin_offset = frand() * M_PI;
        npart.radius = frand() * 0.5 + 1;
        particles.push_back(std::move(npart));
    }
}