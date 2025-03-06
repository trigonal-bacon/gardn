#include <Server/Process/Process.hh>

#include <Server/EntityFunctions.hh>
#include <Server/Spawn.hh>
#include <Shared/Entity.hh>
#include <Shared/Simulation.hh>
#include <Shared/StaticData.hh>

static void default_tick_idle(Simulation *sim, Entity &ent) {
    if (ent.ai_tick >= 2 * TPS) {
        ent.set_angle(frand() * 2 * M_PI);
        ent.ai_tick = 0;
        ent.ai_state = AIState::kIdleMoving;
    }
}

static void default_tick_idle_moving(Simulation *sim, Entity &ent) {
    if (ent.ai_tick > 2.5 * TPS) {
        ent.ai_tick = 0;
        ent.ai_state = AIState::kIdle;
        return;
    }
    float r = ent.ai_tick / (2.5f * TPS);
    ent.acceleration
        .unit_normal(ent.angle)
        .set_magnitude(2.5 * PLAYER_ACCELERATION * (r - r * r));
}

static void tick_default_passive(Simulation *sim, Entity &ent) {
    switch(ent.ai_state) {
        case AIState::kIdle: {
            default_tick_idle(sim, ent);
            break;
        }
        case AIState::kIdleMoving: {
            default_tick_idle_moving(sim, ent);
            break;
        }
        default:
            ent.ai_state = AIState::kIdle;
            break;
    }
}

static void tick_default_neutral(Simulation *sim, Entity &ent) {
    if (sim->ent_alive(ent.target)) {
        Entity &target = sim->get_ent(ent.target);
        Vector v(target.x - ent.x, target.y - ent.y);
        v.set_magnitude(PLAYER_ACCELERATION * 0.975);
        ent.acceleration = v;
        ent.set_angle(v.angle());
        return;
    } else {
        if (ent.target != NULL_ENTITY) {
            ent.target = NULL_ENTITY;
            ent.ai_state = AIState::kIdle;
            ent.ai_tick = 0;
        }
        tick_default_passive(sim, ent);;
    }
}

static void tick_default_aggro(Simulation *sim, Entity &ent) {
    if (sim->ent_alive(ent.target)) {
        Entity &target = sim->get_ent(ent.target);
        Vector v(target.x - ent.x, target.y - ent.y);
        v.set_magnitude(PLAYER_ACCELERATION * 0.975);
        ent.acceleration = v;
        ent.set_angle(v.angle());
        return;
    } else {
        if (ent.target != NULL_ENTITY) {
            ent.ai_state = AIState::kIdle;
            ent.ai_tick = 0;
        }
        ent.target = find_nearest_enemy(sim, ent, 600);
        tick_default_passive(sim, ent);;
    }
}

void tick_ai_behavior(Simulation *sim, Entity &ent) {
    if (!(ent.parent == NULL_ENTITY) && !sim->ent_alive(ent.parent)) sim->request_delete(ent.id);
    if (ent.pending_delete) return;
    switch(ent.mob_id) {
        case MobID::kBabyAnt:
        case MobID::kBee:
        case MobID::kLadybug:
        case MobID::kMassiveLadybug:
            tick_default_passive(sim, ent);
            break;
        case MobID::kWorkerAnt:
            tick_default_neutral(sim, ent);
            break;
        case MobID::kSoldierAnt:
        case MobID::kBeetle:
        case MobID::kMassiveBeetle:
            tick_default_aggro(sim, ent);
            break;
        default:
            break;
    }
    ++ent.ai_tick;
}