#include <Server/EntityFunctions.hh>

#include <Shared/Entity.hh>
#include <Shared/Simulation.hh>

void entity_set_despawn_tick(Entity &ent, game_tick_t t) {
    ent.despawn_tick = t;
    BitMath::set(ent.flags, EntityFlags::kIsDespawning);
}