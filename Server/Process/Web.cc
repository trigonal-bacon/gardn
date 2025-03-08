#include <Server/Process/Process.hh>

#include <Shared/Simulation.hh>
#include <Shared/Entity.hh>

void tick_web_behavior(Simulation *sim, Entity &ent) {
    if (ent.despawn_tick > 10 * TPS) {
        sim->request_delete(ent.id);
        return;
    }
    if (ent.despawn_tick == 1) ent.set_radius(ent.mass);
    ++ent.despawn_tick;
}