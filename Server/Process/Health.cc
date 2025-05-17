#include <Server/Process/Process.hh>

#include <Server/EntityFunctions.hh>
#include <Shared/Simulation.hh>
#include <Shared/Entity.hh>

#include <iostream>

void tick_health_behavior(Simulation *sim, Entity &ent) {
    ent.set_damaged(0);

    if (ent.poison.time > 0 && !ent.has_component(kPetal)) {
        ent.poison.time--;
        inflict_damage(sim, ent.target, ent, ent.poison.damage);
        if ((int) ent.poison.time % 10) ent.set_damaged(0);
    }
    if (ent.immunity_ticks > 0) --ent.immunity_ticks;
    if (ent.dandy_ticks > 0) --ent.dandy_ticks;
    if (ent.health <= 0) sim->request_delete(ent.id);
    if (ent.max_health == 0) return;
    ent.set_health_ratio(ent.health / ent.max_health);
}