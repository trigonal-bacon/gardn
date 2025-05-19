#include <Server/EntityFunctions.hh>

#include <Shared/Entity.hh>

void entity_set_owner(Entity &ent, EntityID const owner) {
    if (owner == NULL_ENTITY)
        ent.owner = ent.id;
    else 
        ent.owner = owner;
    ent.set_parent(owner);
}