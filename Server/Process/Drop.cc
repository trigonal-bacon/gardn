#include <Server/Process/Process.hh>

#include <Shared/Simulation.hh>
#include <Shared/Entity.hh>

void tick_drop_behavior(Simulation *sim, Entity &ent) {
    if (ent.despawn_tick >= (20 + PETAL_DATA[ent.drop_id].rarity * 10) * TPS) {
        sim->request_delete(ent.id);
    }
    else {
        if (ent.despawn_tick == 1) ent.set_radius(15);
        ++ent.despawn_tick;
    }
}