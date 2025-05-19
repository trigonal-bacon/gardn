#include <Server/Process.hh>

#include <Shared/Simulation.hh>
#include <Shared/Entity.hh>

void tick_web_behavior(Simulation *sim, Entity &ent) {
    if (ent.lifetime > 0) ent.set_radius(ent.mass);
    //++ent.despawn_tick;
}