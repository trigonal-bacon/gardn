#include <Server/Process.hh>

#include <Shared/Entity.hh>
#include <Shared/Map.hh>
#include <Shared/Simulation.hh>
#include <Shared/StaticData.hh>

constexpr float CULL_RADIUS = 1500;

void tick_culling_behavior(Simulation *sim, Entity &ent) {
    sim->spatial_hash.query(ent.camera_x, ent.camera_y, CULL_RADIUS, CULL_RADIUS, [](Simulation *, Entity &ent) {
        BIT_UNSET(ent.flags, EntityFlags::kIsCulled);
    });
}
