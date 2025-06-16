#include <Server/EntityFunctions.hh>

#include <Shared/Entity.hh>

void entity_set_despawn_tick(Entity &ent, uint16_t t) {
    ent.despawn_tick = t;
    BIT_SET(ent.flags, EntityFlags::kIsDespawning);
}