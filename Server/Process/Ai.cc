#include <Server/Process.hh>

#include <Server/EntityFunctions.hh>
#include <Server/Spawn.hh>
#include <Shared/Entity.hh>
#include <Shared/Simulation.hh>
#include <Shared/StaticData.hh>

#include <cmath>

// heading angle should be used to steer. angle should follow heading in some
// way

static float get_passive_speed(Entity& ent) {
  return MOB_DATA[ent.get_mob_id()].attributes.passive_speed_mult *
         PLAYER_ACCELERATION;
}

static float get_aggro_speed(Entity& ent) {
  return MOB_DATA[ent.get_mob_id()].attributes.aggro_speed_mult *
         PLAYER_ACCELERATION;
}

static void _focus_lose_clause(Entity& ent, const Vector& v) {
  if (v.magnitude() > 1.5 * ent.detection_radius)
    ent.target = NULL_ENTITY;
}

static void set_and_follow_angle(Entity& ent, float angle) {
  ent.heading_angle = angle;
  ent.set_angle(angle);
}

static void default_tick_idle(Simulation& sim, Entity& ent) {
  ent.ai_tick = 0;
  set_and_follow_angle(ent, frand() * 2 * M_PI);
  ent.ai_state = AIState::kIdleMoving;
}

static void default_tick_idle_moving(Simulation& sim, Entity& ent,
                                     float freq = 0.002, float k = 2) {
  if (ent.ai_tick * (1000.0f / TPS) > 6.28 / freq) {
    ent.ai_tick = 0;
    ent.ai_state = AIState::kIdle;
    return;
  }
  const float t = ent.ai_tick * (1000.0f / TPS);
  ent.acceleration.unit_normal(ent.get_angle())
      .set_magnitude(get_passive_speed(ent) *
                     powf((1 - cosf(t * freq)) / 2, k));
}

static void default_tick_returning(Simulation& sim, Entity& ent,
                                   float speed = 1.0) {
  if (!sim.ent_alive(ent.get_owner())) {
    ent.ai_tick = 0;
    ent.ai_state = AIState::kIdle;
    return;
  }
  Entity& parent = sim.get_ent(ent.get_owner());
  Vector delta(parent.get_x() - ent.get_x(), parent.get_y() - ent.get_y());
  if (delta.magnitude() > 300) {
    ent.ai_tick = 0;
  } else if (ent.ai_tick > 2 * TPS || delta.magnitude() < 100) {
    ent.ai_tick = 0;
    ent.ai_state = AIState::kIdle;
    return;
  }
  delta.set_magnitude(PLAYER_ACCELERATION * speed);
  ent.acceleration = delta;
  set_and_follow_angle(ent, delta.angle());
}

static void tick_default_passive(Simulation& sim, Entity& ent) {
  switch (ent.ai_state) {
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

static void tick_default_neutral(Simulation& sim, Entity& ent) {
  if (sim.ent_alive(ent.target)) {
    Entity& target = sim.get_ent(ent.target);
    Vector v(target.get_x() - ent.get_x(), target.get_y() - ent.get_y());
    v.set_magnitude(get_aggro_speed(ent));
    ent.acceleration = v;
    set_and_follow_angle(ent, v.angle());
    return;
  } else {
    if (ent.target != NULL_ENTITY) {
      ent.target = NULL_ENTITY;
      ent.ai_state = AIState::kIdle;
      ent.ai_tick = 0;
    }
    tick_default_passive(sim, ent);
  }
}

static void tick_default_aggro(Simulation& sim, Entity& ent) {
  if (sim.ent_alive(ent.target)) {
    Entity& target = sim.get_ent(ent.target);
    Vector v(target.get_x() - ent.get_x(), target.get_y() - ent.get_y());
    _focus_lose_clause(ent, v);
    v.set_magnitude(get_aggro_speed(ent));
    ent.acceleration = v;
    set_and_follow_angle(ent, v.angle());
    return;
  } else {
    if (ent.target != NULL_ENTITY) {
      ent.ai_state = AIState::kIdle;
      ent.ai_tick = 0;
    }
    // if (ent.ai_state != AIState::kReturning)
    ent.target =
        find_nearest_enemy(sim, ent, ent.detection_radius + ent.get_radius());
    tick_default_passive(sim, ent);
  }
}

static void tick_bee_passive(Simulation& sim, Entity& ent, float freq = 0.002,
                             float k = 0.5) {
  switch (ent.ai_state) {
  case AIState::kIdle: {
    if (ent.ai_tick >= 5 * TPS) {
      ent.ai_tick = 0;
      ent.heading_angle = frand() * 2 * M_PI;
      ent.ai_state = AIState::kIdle;
    }
    const float t = ent.lifetime * (1000.0f / TPS);
    ent.set_angle(ent.heading_angle + 0.5 * sinf(t * freq));
    Vector v(cosf(ent.get_angle()), sinf(ent.get_angle()));
    v *= get_passive_speed(ent) * powf((1 - cosf(t * freq)) / 2, k);
    ent.acceleration = v;
    break;
  }
  case AIState::kIdleMoving: {
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

static void tick_hornet_aggro(Simulation& sim, Entity& ent) {
  if (sim.ent_alive(ent.target)) {
    Entity& target = sim.get_ent(ent.target);
    Vector v(target.get_x() - ent.get_x(), target.get_y() - ent.get_y());
    _focus_lose_clause(ent, v);
    set_and_follow_angle(ent, v.angle());
    float dist = v.magnitude();
    if (dist > 300) {
      v.set_magnitude(get_aggro_speed(ent));
    } else {
      // ent.acceleration.set(0,0);
      v = {-v.y, v.x};
      v.set_magnitude(get_aggro_speed(ent) * 0.2);
    }
    ent.acceleration = v;
    if (ent.ai_tick >= 1.5 * TPS && dist < 800) {
      ent.ai_tick = 0;

      Entity& missile = alloc_petal(sim, PetalID::kMissile, ent);
      missile.damage = 10;
      missile.health = missile.max_health = 10;

      entity_set_despawn_tick(missile, 3 * TPS);
      missile.set_angle(ent.get_angle());
      missile.set_x(missile.get_x() - cosf(missile.get_angle()) * 20);
      missile.set_y(missile.get_y() - sinf(missile.get_angle()) * 20);
      missile.velocity.unit_normal(ent.get_angle()).set_magnitude(1.4);
      missile.constant_velocity.unit_normal(ent.get_angle()).set_magnitude(0.6);
      Vector kb;
      kb.unit_normal(ent.get_angle() - M_PI)
          .set_magnitude(PLAYER_ACCELERATION * 0.2);
      ent.velocity += kb;
    }
    return;
  } else {
    if (ent.target != NULL_ENTITY) {
      ent.ai_state = AIState::kIdle;
      ent.ai_tick = 0;
      ent.target = NULL_ENTITY;
    }
    ent.target = find_nearest_enemy(sim, ent, ent.detection_radius);
    tick_bee_passive(sim, ent);
  }
}

static void tick_centipede_passive(Simulation& sim, Entity& ent) {
  switch (ent.ai_state) {
  case AIState::kIdle: {
    set_and_follow_angle(ent, ent.get_angle() + 0.2 / TPS);
    if (frand() < 1 / (5.0 * TPS))
      ent.ai_state = AIState::kIdleMoving;
    break;
  }
  case AIState::kIdleMoving: {
    set_and_follow_angle(ent, ent.get_angle() - 0.2 / TPS);
    if (frand() < 1 / (5.0 * TPS))
      ent.ai_state = AIState::kIdle;
    break;
  }
  case AIState::kReturning: {
    default_tick_returning(sim, ent);
    break;
  }
  }
  ent.acceleration.unit_normal(ent.get_angle())
      .set_magnitude(get_passive_speed(ent));
}

static void tick_centipede_neutral(Simulation& sim, Entity& ent) {
  if (sim.ent_alive(ent.target)) {
    Entity& target = sim.get_ent(ent.target);
    Vector v(target.get_x() - ent.get_x(), target.get_y() - ent.get_y());
    v.set_magnitude(get_passive_speed(ent));
    ent.acceleration = v;
    set_and_follow_angle(ent, v.angle());
    return;
  } else {
    if (ent.target != NULL_ENTITY) {
      ent.ai_state = AIState::kIdle;
      ent.ai_tick = 0;
    }
    switch (ent.ai_state) {
    case AIState::kIdle:
    case AIState::kIdleMoving:
      tick_centipede_passive(sim, ent);
      break;
    case AIState::kReturning: {
      default_tick_returning(sim, ent);
      break;
    }
    }
  }
}

static void tick_centipede_aggro(Simulation& sim, Entity& ent) {
  if (sim.ent_alive(ent.target)) {
    Entity& target = sim.get_ent(ent.target);
    Vector v(target.get_x() - ent.get_x(), target.get_y() - ent.get_y());
    _focus_lose_clause(ent, v);
    v.set_magnitude(get_aggro_speed(ent));
    ent.acceleration = v;
    set_and_follow_angle(ent, v.angle());
    return;
  } else {
    if (ent.target != NULL_ENTITY) {
      ent.ai_state = AIState::kIdle;
      ent.ai_tick = 0;
    }
    ent.target =
        find_nearest_enemy(sim, ent, ent.detection_radius + ent.get_radius());
    switch (ent.ai_state) {
    case AIState::kIdle:
    case AIState::kIdleMoving:
      tick_centipede_passive(sim, ent);
      break;
    case AIState::kReturning: {
      default_tick_returning(sim, ent);
      break;
    }
    }
  }
}

static void tick_sandstorm(Simulation& sim, Entity& ent) {
  switch (ent.ai_state) {
  case AIState::kIdle: {
    if (frand() > 1.0f / TPS) {
      ent.ai_tick = 0;
      ent.heading_angle = frand() * 2 * M_PI;
      ent.ai_state = AIState::kIdleMoving;
    }
    Vector rand = Vector::rand(PLAYER_ACCELERATION * 0.5);
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
  if (sim.ent_alive(ent.get_owner())) {
    Entity& parent = sim.get_ent(ent.get_owner());
    ent.acceleration = (ent.acceleration + parent.acceleration) * 0.75;
  }
}

static void tick_digger(Simulation& sim, Entity& ent) {
  ent.input = 0;
  if (sim.ent_alive(ent.target)) {
    Entity& target = sim.get_ent(ent.target);
    Vector v(target.get_x() - ent.get_x(), target.get_y() - ent.get_y());
    _focus_lose_clause(ent, v);
    v.set_magnitude(get_aggro_speed(ent));
    if (ent.health / ent.max_health > 0.1) {
      BitMath::set(ent.input, InputFlags::kAttacking);
    } else {
      BitMath::set(ent.input, InputFlags::kDefending);
      v *= -1;
    }
    ent.acceleration = v;
    set_and_follow_angle(ent, v.angle());
    return;
  } else {
    if (ent.target != NULL_ENTITY) {
      ent.ai_state = AIState::kIdle;
      ent.ai_tick = 0;
      ent.target = NULL_ENTITY;
    }
    ent.target =
        find_nearest_enemy(sim, ent, ent.detection_radius + ent.get_radius());
    switch (ent.ai_state) {
    case AIState::kIdle: {
      set_and_follow_angle(ent, frand() * M_PI * 2);
      ent.ai_state = AIState::kIdleMoving;
      ent.ai_tick = 0;
      break;
    }
    case AIState::kIdleMoving: {
      if (ent.ai_tick > 5 * TPS)
        ent.ai_state = AIState::kIdle;
      ent.acceleration.unit_normal(ent.get_angle())
          .set_magnitude(get_passive_speed(ent));
      break;
    }
    case AIState::kReturning: {
      default_tick_returning(sim, ent);
      break;
    }
    }
  }
}

void tick_ai_behavior(Simulation& sim, Entity& ent) {
  if (ent.pending_delete)
    return;
  if (sim.ent_alive(ent.get_parent()))
    return;
  ent.acceleration.set(0, 0);
  if (!sim.ent_alive(ent.target) && sim.ent_alive(ent.last_damaged_by))
    ent.target = ent.last_damaged_by;
  if (ent.get_owner() != NULL_ENTITY) {
    if (!sim.ent_alive(ent.get_owner())) {
      if (BitMath::at(ent.flags, EntityFlags::kDieOnParentDeath))
        sim.request_delete(ent.id);
      ent.set_owner(NULL_ENTITY);
    } else {
      const Entity& parent = sim.get_ent(ent.get_owner());
      Vector delta(parent.get_x() - ent.get_x(), parent.get_y() - ent.get_y());
      if (delta.magnitude() > SUMMON_RETREAT_RADIUS) {
        ent.target = NULL_ENTITY;
        ent.ai_state = AIState::kReturning;
      }
      if (sim.ent_alive(ent.target)) {
        const Entity& target = sim.get_ent(ent.target);
        delta = Vector(parent.get_x() - target.get_x(),
                       parent.get_y() - target.get_y());
        if (delta.magnitude() > SUMMON_RETREAT_RADIUS)
          ent.target = NULL_ENTITY;
      }
    }
  }
  if (BitMath::at(ent.flags, EntityFlags::kIsCulled)) {
    ent.target = NULL_ENTITY;
    ent.ai_tick = 0;
    return;
  }
  switch (ent.get_mob_id()) {
  case MobID::kBabyAnt:
  case MobID::kLadybug:
  case MobID::kMassiveLadybug:
    tick_default_passive(sim, ent);
    break;
  case MobID::kBee:
    tick_bee_passive(sim, ent);
    break;
  case MobID::kCentipede:
    tick_centipede_passive(sim, ent);
    break;
  case MobID::kEvilCentipede:
    tick_centipede_aggro(sim, ent);
    break;
  case MobID::kDesertCentipede:
    tick_centipede_neutral(sim, ent);
    break;
  case MobID::kWorkerAnt:
  case MobID::kDarkLadybug:
  case MobID::kShinyLadybug:
    tick_default_neutral(sim, ent);
    break;
  case MobID::kSpider:
    if (ent.lifetime % TPS == 0)
      alloc_web(sim, 25, ent);
  case MobID::kSoldierAnt:
  case MobID::kBeetle:
  case MobID::kMassiveBeetle:
  case MobID::kScorpion:
    tick_default_aggro(sim, ent);
    break;
  case MobID::kQueenAnt:
    if (ent.lifetime % (2 * TPS) == 0) {
      Vector behind;
      behind.unit_normal(ent.get_angle() + M_PI);
      behind *= ent.get_radius();
      Entity& spawned =
          alloc_mob(sim, MobID::kSoldierAnt, ent.get_x() + behind.x,
                    ent.get_y() + behind.y, ent.get_team(), [](Entity& mob) {
                      mob.score_reward = MOB_DATA[mob.get_mob_id()].xp;
                      BitMath::set(mob.flags, EntityFlags::kHasCulling);
                    });
      entity_set_despawn_tick(spawned, 10 * TPS);
      spawned.set_owner(ent.get_owner());
    }
    tick_default_aggro(sim, ent);
    break;
  case MobID::kHornet:
    tick_hornet_aggro(sim, ent);
    break;
  case MobID::kBoulder:
  case MobID::kRock:
  case MobID::kCactus:
  case MobID::kSquare:
    break;
  case MobID::kSandstorm:
    tick_sandstorm(sim, ent);
    break;
  case MobID::kDigger:
    tick_digger(sim, ent);
    break;
  default:
    break;
  }
  // wall avoidance, reflect both angles
  if (!sim.ent_alive(ent.target)) {
    if (ent.get_x() - ent.get_radius() <= 0 &&
        angle_within(ent.get_angle(), M_PI, M_PI / 2))
      set_and_follow_angle(ent, M_PI - ent.get_angle());
    if (ent.get_x() + ent.get_radius() >= ARENA_WIDTH &&
        angle_within(ent.get_angle(), 0, M_PI / 2))
      set_and_follow_angle(ent, M_PI - ent.get_angle());
    if (ent.get_y() - ent.get_radius() <= 0 &&
        angle_within(ent.get_angle(), 3 * M_PI / 2, M_PI / 2))
      set_and_follow_angle(ent, 0 - ent.get_angle());
    if (ent.get_y() + ent.get_radius() >= ARENA_HEIGHT &&
        angle_within(ent.get_angle(), M_PI / 2, M_PI / 2))
      set_and_follow_angle(ent, 0 - ent.get_angle());
  }
  ++ent.ai_tick;
}