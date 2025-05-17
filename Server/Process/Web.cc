#include <Server/Process/Process.hh>

#include <Shared/Simulation.hh>
#include <Shared/Entity.hh>

void tick_web_behavior(Simulation *sim, Entity &ent) {
    ent.set_radius(ent.mass);
    //++ent.despawn_tick;
}