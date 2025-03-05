#include <Server/Process/Process.hh>

#include <Shared/Simulation.hh>
#include <Shared/Entity.hh>

void tick_health_behavior(Simulation *sim, Entity &ent) {
    /*
    if (ent.applied_poison.ticks_left == 0) ent.applied_poison.reset();
    else {
        inflict_damage(sim, ent, ent.target, ent.applied_poison.damage);
        --ent.applied_poison.ticks_left;
        if (ent.applied_poison.ticks_left % ((uint32_t) SERVER_TIME(1)) == 0) ent.set_damaged(1);
        else ent.set_damaged(0);
    }
    */
    ent.set_damaged(0);
    if (ent.immunity_ticks > 0) --ent.immunity_ticks;
    if (ent.health <= 0) sim->request_delete(ent.id);
    if (ent.max_health == 0) return;
    ent.set_health_ratio(ent.health / ent.max_health);
}