#include <Server/Process.hh>

#include <Server/EntityFunctions.hh>
#include <Server/Spawn.hh>
#include <Shared/Entity.hh>
#include <Shared/Simulation.hh>
#include <Shared/StaticData.hh>

static void default_tick_idle(Simulation *sim, Entity &ent) {
    if (ent.ai_tick >= 0.5 * TPS) {
        ent.ai_tick = 0;
        ent.set_angle(frand() * 2 * M_PI);
        ent.ai_state = AIState::kIdleMoving;
    }
}

static void default_tick_idle_moving(Simulation *sim, Entity &ent) {
    if (ent.ai_tick > 2 * TPS) {
        ent.ai_tick = 0;
        ent.ai_state = AIState::kIdle;
        return;
    }
    float r = ent.ai_tick / (2.0f * TPS);
    ent.acceleration
        .unit_normal(ent.angle)
        .set_magnitude(3 * PLAYER_ACCELERATION * (r - r * r));
}

static void default_tick_returning(Simulation *sim, Entity &ent, float speed = 1.0) {
    if (!sim->ent_alive(ent.parent)) {
        ent.ai_tick = 0;
        ent.ai_state = AIState::kIdle;
        return;
    }
    Entity &parent = sim->get_ent(ent.parent);
    Vector delta(parent.x - ent.x, parent.y - ent.y);
    if (delta.magnitude() > 300) {
        ent.ai_tick = 0;
    } else if (ent.ai_tick > 2 * TPS || delta.magnitude() < 100) {
        ent.ai_tick = 0;
        ent.ai_state = AIState::kIdle;
        return;
    } 
    delta.normalize().set_magnitude(PLAYER_ACCELERATION * speed);
    ent.acceleration = delta;
    ent.set_angle(delta.angle());
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
        case AIState::kReturning: {
            default_tick_returning(sim, ent);
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
        if (!(ent.target == NULL_ENTITY)) {
            ent.target = NULL_ENTITY;
            ent.ai_state = AIState::kIdle;
            ent.ai_tick = 0;
        }
        tick_default_passive(sim, ent);
    }
}

static void tick_default_aggro(Simulation *sim, Entity &ent, float speed) {
    if (sim->ent_alive(ent.target)) {
        Entity &target = sim->get_ent(ent.target);
        Vector v(target.x - ent.x, target.y - ent.y);
        v.set_magnitude(PLAYER_ACCELERATION * speed);
        ent.acceleration = v;
        ent.set_angle(v.angle());
        return;
    } else {
        if (!(ent.target == NULL_ENTITY)) {
            ent.ai_state = AIState::kIdle;
            ent.ai_tick = 0;
        }
        //if (ent.ai_state != AIState::kReturning) 
        ent.target = find_nearest_enemy(sim, ent, ent.detection_radius + ent.radius);
        tick_default_passive(sim, ent);
    }
}

static void tick_hornet_aggro(Simulation *sim, Entity &ent) {
    if (sim->ent_alive(ent.target)) {
        Entity &target = sim->get_ent(ent.target);
        Vector v(target.x - ent.x, target.y - ent.y);
        float dist = v.magnitude();
        if (dist > 300) {
            v.set_magnitude(PLAYER_ACCELERATION * 0.975);
            ent.acceleration = v;
        }
        ent.set_angle(v.angle());
        if (ent.ai_tick >= 1.5 * TPS && dist < 800) {
            ent.ai_tick = 0;
            //spawn missile;
            Entity &missile = alloc_petal(PetalID::kMissile, ent);
            missile.damage = 10;
            missile.health = missile.max_health = 25;
            //missile.despawn_tick = 1;
            missile.set_despawn_tick(3 * TPS);
            missile.set_radius(20);
            missile.set_angle(ent.angle);
            missile.acceleration.unit_normal(ent.angle).set_magnitude(25 * PLAYER_ACCELERATION);
            Vector kb;
            kb.unit_normal(ent.angle - M_PI).set_magnitude(2.5 * PLAYER_ACCELERATION);
            ent.acceleration += kb;            
        }
        return;
    } else {
        if (!(ent.target == NULL_ENTITY)) {
            ent.ai_state = AIState::kIdle;
            ent.ai_tick = 0;
            ent.target = NULL_ENTITY;
        }
        ent.target = find_nearest_enemy(sim, ent, 1200);
        tick_default_passive(sim, ent);;
    }
}

static void tick_centipede_passive(Simulation *sim, Entity &ent) {
    switch(ent.ai_state) {
        case AIState::kIdle: {
            ent.set_angle(ent.angle + 0.005);
            if (frand() < 0.0025) ent.ai_state = AIState::kIdleMoving;
            break;
        }
        case AIState::kIdleMoving: {
            ent.set_angle(ent.angle - 0.005);
            if (frand() < 0.0025) ent.ai_state = AIState::kIdle;
            break;
        }
        case AIState::kReturning: {
            default_tick_returning(sim, ent);
            break;
        }
    }
    ent.acceleration.unit_normal(ent.angle).set_magnitude(PLAYER_ACCELERATION / 10);
}

static void tick_centipede_neutral(Simulation *sim, Entity &ent, float speed) {
    if (sim->ent_alive(ent.target)) {
        Entity &target = sim->get_ent(ent.target);
        Vector v(target.x - ent.x, target.y - ent.y);
        v.set_magnitude(PLAYER_ACCELERATION * speed);
        ent.acceleration = v;
        ent.set_angle(v.angle());
        return;
    } else {
        if (!(ent.target == NULL_ENTITY)) {
            ent.ai_state = AIState::kIdle;
            ent.ai_tick = 0;
        }
        //ent.target = find_nearest_enemy(sim, ent, ent.detection_radius + ent.radius);
        switch(ent.ai_state) {
            case AIState::kIdle: {
                ent.set_angle(ent.angle + 0.005);
                if (frand() < 0.0025) ent.ai_state = AIState::kIdleMoving;
                ent.acceleration.unit_normal(ent.angle).set_magnitude(PLAYER_ACCELERATION * speed);
                break;
            }
            case AIState::kIdleMoving: {
                ent.set_angle(ent.angle - 0.005);
                if (frand() < 0.0025) ent.ai_state = AIState::kIdle;
                ent.acceleration.unit_normal(ent.angle).set_magnitude(PLAYER_ACCELERATION * speed);
                break;
            }
            case AIState::kReturning: {
                default_tick_returning(sim, ent);
                break;
            }
        }
    }
}

static void tick_centipede_aggro(Simulation *sim, Entity &ent) {
    if (sim->ent_alive(ent.target)) {
        Entity &target = sim->get_ent(ent.target);
        Vector v(target.x - ent.x, target.y - ent.y);
        v.set_magnitude(PLAYER_ACCELERATION * 0.95);
        ent.acceleration = v;
        ent.set_angle(v.angle());
        return;
    } else {
        if (!(ent.target == NULL_ENTITY)) {
            ent.ai_state = AIState::kIdle;
            ent.ai_tick = 0;
        }
        ent.target = find_nearest_enemy(sim, ent, ent.detection_radius + ent.radius);
        switch(ent.ai_state) {
            case AIState::kIdle: {
                ent.set_angle(ent.angle + 0.005);
                if (frand() < 0.0025) ent.ai_state = AIState::kIdleMoving;
                ent.acceleration.unit_normal(ent.angle).set_magnitude(PLAYER_ACCELERATION / 10);
                break;
            }
            case AIState::kIdleMoving: {
                ent.set_angle(ent.angle - 0.005);
                if (frand() < 0.0025) ent.ai_state = AIState::kIdle;
                ent.acceleration.unit_normal(ent.angle).set_magnitude(PLAYER_ACCELERATION / 10);
                break;
            }
            case AIState::kReturning: {
                default_tick_returning(sim, ent);
                break;
            }
        }
    }
}

static void tick_sandstorm(Simulation *sim, Entity &ent) {
    switch(ent.ai_state) {
        case AIState::kIdle: {
            if (frand() > 1.0f / TPS) {
                ent.ai_tick = 0;
                ent.heading_angle = frand() * 2 * M_PI;
                ent.ai_state = AIState::kIdleMoving;
            }
            Vector rand;
            rand.unit_normal(frand() * 2 * M_PI);
            rand *= (PLAYER_ACCELERATION);
            ent.acceleration.set(rand.x, rand.y);
            break;
        }
        case AIState::kIdleMoving: {
            if (ent.ai_tick >= 2.5 * TPS) {
                ent.ai_tick = 0;
                ent.ai_state = AIState::kIdle;
            }
            if (frand() > 2.5f / TPS)
                ent.heading_angle += frand() * M_PI - M_PI / 2;
            Vector head;
            head.unit_normal(ent.heading_angle);
            head.set_magnitude(PLAYER_ACCELERATION);
            Vector rand;
            rand.unit_normal(ent.heading_angle + frand() * M_PI - M_PI / 2);
            rand.set_magnitude(PLAYER_ACCELERATION * 0.5);
            head += rand;
            ent.acceleration.set(head.x, head.y);
            break;
        }
        case AIState::kReturning: {
            default_tick_returning(sim, ent, 1.5);
            break;
        }
        default:
            ent.ai_state = AIState::kIdle;
            break;
    }
    if (sim->ent_alive(ent.owner)) {
        Entity &parent = sim->get_ent(ent.owner);
        ent.acceleration.x = (ent.acceleration.x + parent.acceleration.x);
        ent.acceleration.y = (ent.acceleration.y + parent.acceleration.y);
    }
}

void tick_ai_behavior(Simulation *sim, Entity &ent) {
    if (ent.pending_delete) return;
    if (sim->ent_alive(ent.seg_head)) return;
    if (!(ent.parent == NULL_ENTITY)) { 
        if (!sim->ent_alive(ent.parent)) {
            if (ent.flags & EntityFlags::DieOnParentDeath)
                sim->request_delete(ent.id);
            ent.parent = NULL_ENTITY;
        } else {
            Entity &parent = sim->get_ent(ent.parent);
            Vector delta(parent.x - ent.x, parent.y - ent.y);
            if (delta.magnitude() > SUMMON_RETREAT_RADIUS) {
                ent.target = NULL_ENTITY;
                ent.ai_state = AIState::kReturning;
                ent.ai_tick = 3; //prevent subtraction errors
            }
        }
    }
    switch(ent.mob_id) {
        case MobID::kBabyAnt:
        case MobID::kBee:
        case MobID::kLadybug:
        case MobID::kMassiveLadybug:
            tick_default_passive(sim, ent);
            break;
        case MobID::kCentipede:
            tick_centipede_passive(sim, ent);
            break;
        case MobID::kEvilCentipede:
            tick_centipede_aggro(sim, ent);
            break;
        case MobID::kDesertCentipede:
            tick_centipede_neutral(sim, ent, 1.5);
            break;
        case MobID::kWorkerAnt:
        case MobID::kDarkLadybug:
            tick_default_neutral(sim, ent);
            break;
        case MobID::kSoldierAnt:
        case MobID::kBeetle:
        case MobID::kMassiveBeetle:
            tick_default_aggro(sim, ent, 0.95);
            break;
        case MobID::kScorpion:
            tick_default_aggro(sim, ent, 1.25);
            break;
        case MobID::kSpider:
            if (ent.lifetime % (TPS) == 0) 
                alloc_web(25, ent);
            tick_default_aggro(sim, ent, 1.25);
            break;
        case MobID::kQueenAnt:
            if (ent.lifetime % (2 * TPS) == 0) {
                Vector behind;
                behind.unit_normal(ent.angle + M_PI);
                behind *= ent.radius;
                Entity &spawned = alloc_mob(MobID::kSoldierAnt, ent.x + behind.x, ent.y + behind.y, ent.team);
                spawned.flags |= EntityFlags::IsDespawning;
                spawned.despawn_tick = 10 * TPS;
                entity_set_owner(spawned, ent.parent);
            }
            tick_default_aggro(sim, ent, 0.95);
            break;
        case MobID::kHornet:
            tick_hornet_aggro(sim, ent);
            break;
        case MobID::kRock:
        case MobID::kCactus:
            break;
        case MobID::kSandstorm:
            tick_sandstorm(sim, ent);
            break;
        default:
            break;
    }
    ++ent.ai_tick;
}