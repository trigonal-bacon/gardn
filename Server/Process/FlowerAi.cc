#include <Server/Process.hh>

#include <Shared/Entity.hh>
#include <Shared/Simulation.hh>
#include <Shared/StaticData.hh>

void tick_player_ai_behavior(Simulation *sim, Entity &ent) {
    if (!sim->ent_alive(ent.get_player())) return;
    if (!BitMath::at(ent.flags, EntityFlags::kCPUControlled)) return;
    Entity &player = sim->get_ent(ent.get_player());
    //cpu behavior
}