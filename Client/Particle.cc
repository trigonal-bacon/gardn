#include <Client/Particle.hh>

#include <Client/Game.hh>

#include <Client/Assets/Assets.hh>

#include <cmath>
#include <vector>

using namespace Particle;

static std::vector<TitleParticleEntity> title_particles;
static std::vector<GameParticleEntity> game_particles;

void Particle::tick_title(Renderer &ctx, double dt) {
    RenderContext c(&ctx);
    ctx.reset_transform();
    size_t len = title_particles.size();
    for (size_t i = len; i > 0; --i) {
        TitleParticleEntity &part = title_particles[i - 1];
        if (part.x > ctx.width + 10 * part.radius) {
            part = title_particles[title_particles.size() - 1];
            title_particles.pop_back();
            continue;
        }
        RenderContext c(&ctx);
        part.x += part.x_velocity * (dt / 1000) * Ui::scale;
        part.angle += dt / 1000 * Ui::scale;
        ctx.translate(part.x, part.y + 12.5 * sin(Game::timestamp / 500 + part.sin_offset));
        ctx.scale(Ui::scale * part.radius);
        if (PETAL_DATA[part.id].attributes.rotation_style == PetalAttributes::kPassiveRot)
            ctx.rotate(part.angle);
        if (part.id == PetalID::kPeas || part.id == PetalID::kPoisonPeas)
            draw_static_petal(part.id, ctx);
        else
            draw_static_petal_single(part.id, ctx);
    }
    for (size_t i = 0; i < 4; ++i) {
        if (frand() > 0.02) continue;
        TitleParticleEntity npart;
        npart.x = -100;
        std::vector<PetalID::T> ids = {PetalID::kBasic};
        float freq_sum = 1;
        for (PetalID::T pot = PetalID::kBasic + 1; pot < PetalID::kNumPetals; ++pot)
            if (Game::seen_petals[pot]) { ids.push_back(pot); freq_sum += pow(0.5, PETAL_DATA[pot].rarity); }
        
        freq_sum *= frand();
        for (PetalID::T id : ids) {
            freq_sum -= pow(0.5, PETAL_DATA[id].rarity);
            if (freq_sum > 0) continue;
            npart.id = id;   
            npart.y = frand() * ctx.height;
            npart.angle = frand() * 2 * M_PI;
            npart.x_velocity = frand() * 100 + 100;
            npart.sin_offset = frand() * M_PI;
            npart.radius = frand() + 0.5;
            title_particles.push_back(std::move(npart));
            break;
        }
    }
}


void Particle::tick_game(Renderer &ctx, double dt) {
    size_t len = game_particles.size();
    for (size_t i = len; i > 0; --i) {
        GameParticleEntity &part = game_particles[i - 1];
        if (part.opacity < 0.1) {
            part = game_particles[game_particles.size() - 1];
            game_particles.pop_back();
            continue;
        }
        RenderContext c(&ctx);
        part.x += part.x_velocity * dt / 1000;
        part.y += part.y_velocity * dt / 1000;
        part.opacity = fclamp(part.opacity - dt / 1000, 0, 1);
        ctx.set_global_alpha(part.opacity);
        ctx.set_fill(part.color);
        ctx.begin_path();
        ctx.arc(part.x,part.y,part.radius);
        ctx.fill();
    }
}

void Particle::add_unique_particle(float x, float y) {
    GameParticleEntity part;
    part.x = x;
    part.y = y;
    part.radius = 4;
    part.opacity = 1;
    part.color = 0x80ffffff;
    Vector rand = Vector::rand(50);
    part.x_velocity = rand.x;
    part.y_velocity = rand.y;
    game_particles.push_back(std::move(part));
}

void Particle::add_revival_particle(float x, float y) {
    GameParticleEntity part;
    part.x = x;
    part.y = y;
    part.radius = 4;
    part.opacity = 1;
    part.color = 0xffffe763;
    Vector rand = Vector::rand(150);
    part.x_velocity = rand.x;
    part.y_velocity = rand.y;
    game_particles.push_back(std::move(part));
}
