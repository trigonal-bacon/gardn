#include <Server/Process.hh>

#include <Shared/Simulation.hh>
#include <Shared/Entity.hh>

#include <Shared/Helpers.hh>

void tick_drop_behavior(Simulation *sim, Entity &ent) {
    if (ent.lifetime > 0)
        ent.set_radius(20);
    if (ent.immunity_ticks < 0.5 * TPS)
        ++ent.immunity_ticks;
}