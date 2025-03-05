#include <Server/EntityFunctions.hh>

#include <Server/Spawn.hh>

#include <Shared/Simulation.hh>
#include <Shared/Entity.hh>
#include <Shared/Helpers.hh>

void inflict_damage(Simulation *sim, Entity &attacker, Entity &defender, float amt) {
    assert(!defender.pending_delete);
    assert(defender.has_component(kHealth));
    if (amt <= defender.armor) return;
    defender.set_damaged(1);
    defender.health = fclamp(defender.health - amt, 0, defender.health);
    if (!sim->ent_alive(defender.target)) {
        if (attacker.has_component(kPetal)) {
            defender.target = attacker.parent;
        } else {
            defender.target = attacker.id;
        }
    }
}

void entity_on_death(Simulation *sim, Entity &ent) {
    if (ent.has_component(kMob)) {
        Entity &drop = alloc_drop(PetalID::kTriplet);
        drop.set_x(ent.x);
        drop.set_y(ent.y);
    }
}
