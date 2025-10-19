#include <Server/Process.hh>

#include <Server/EntityFunctions.hh>
#include <Server/Spawn.hh>
#include <Shared/Entity.hh>
#include <Shared/Simulation.hh>
#include <Shared/StaticData.hh>

#include <cmath>

void tick_petal_behavior(Simulation *sim, Entity &petal) {
    if (petal.pending_delete) return;
    if (!sim->ent_alive(petal.get_parent())) {
        sim->request_delete(petal.id);
        return;
    }
    Entity &player = sim->get_ent(petal.get_parent());
    struct PetalData const &petal_data = PETAL_DATA[petal.get_petal_id()];
    if (petal_data.attributes.rotation_style == PetalAttributes::kPassiveRot) {
        //simulate on clientside
        float rot_amt = petal.get_petal_id() == PetalID::kWing ? 10.0 : 1.0;
        if (petal.id.id % 2) petal.set_angle(petal.get_angle() + rot_amt / TPS);
        else petal.set_angle(petal.get_angle() - rot_amt / TPS);
    } else if (petal_data.attributes.rotation_style == PetalAttributes::kFollowRot && !(BitMath::at(petal.flags, EntityFlags::kIsDespawning))) {
        Vector delta(petal.get_x() - player.get_x(), petal.get_y() - player.get_y());
        petal.set_angle(delta.angle());
    }
    if (BitMath::at(petal.flags, EntityFlags::kIsDespawning)) {
        switch (petal.get_petal_id()) {
<<<<<<< HEAD
            case PetalID::kMissile:
            case PetalID::kDandelion: {
                petal.acceleration.unit_normal(petal.get_angle()).set_magnitude(4 * PLAYER_ACCELERATION);
=======
            case PetalID::kMissile: {
                petal.acceleration.unit_normal(petal.get_angle()).set_magnitude(1.5 * PLAYER_ACCELERATION);
>>>>>>> upstream/master
                break;
            }
            case PetalID::kPoisonPeas:
            case PetalID::kPeas: {
                petal.acceleration.unit_normal(petal.heading_angle).set_magnitude(2.0 * PLAYER_ACCELERATION);
                break;
            }
            default:
                petal.acceleration.set(0,0);
                break;
        }
    }
    else if (petal_data.attributes.secondary_reload > 0) {
        if (petal.secondary_reload > petal_data.attributes.secondary_reload * TPS) {
            if (petal_data.attributes.burst_heal > 0) {
                EntityID potential = NULL_ENTITY;
                float min_health_ratio = 1;
                if (player.health < player.max_health &&
                    player.dandy_ticks == 0 &&
                    !BitMath::at(player.flags, EntityFlags::kZombie))
                    potential = player.id;
                else
                    sim->spatial_hash.query(player.get_x(), player.get_y(),
                        TEAMMATE_HEAL_RADIUS, TEAMMATE_HEAL_RADIUS, [&](Simulation *sim, Entity &ent){
                            if (!sim->ent_alive(ent.id)) return;
                            if (!ent.has_component(kFlower)) return;
                            if (ent.get_team() != player.get_team()) return;
                            if (ent.dandy_ticks > 0) return;
                            if (BitMath::at(ent.flags, EntityFlags::kZombie)) return;
                            float health_ratio = ent.health / ent.max_health;
                            if (health_ratio >= min_health_ratio) return;
                            float dist = Vector(ent.get_x() - player.get_x(), ent.get_y() - player.get_y()).magnitude();
                            if (dist > TEAMMATE_HEAL_RADIUS) return;
                            potential = ent.id;
                            min_health_ratio = health_ratio;
                        });
                if (potential != NULL_ENTITY) {
                    Entity &ent = sim->get_ent(potential);
                    Vector delta(ent.get_x() - petal.get_x(), ent.get_y() - petal.get_y());
                    if (delta.magnitude() < petal.get_radius()) {
                        inflict_heal(sim, ent, petal_data.attributes.burst_heal);
                        sim->request_delete(petal.id);
                        return;
                    }
                    delta.set_magnitude(PLAYER_ACCELERATION * 4);
                    petal.acceleration = delta;
                }
            }
            switch (petal.get_petal_id()) {
                case PetalID::kMissile:
                case PetalID::kDandelion:
                    if (BitMath::at(player.input, InputFlags::kAttacking)) {
                        petal.velocity.unit_normal(petal.get_angle()).set_magnitude(20 * PLAYER_ACCELERATION);
                        petal.friction = DEFAULT_FRICTION;
                        entity_set_despawn_tick(petal, 2.5 * TPS);
                    }
                    break;
                case PetalID::kTriweb:
                case PetalID::kWeb: {
                    if (BitMath::at(player.input, InputFlags::kAttacking)) {
                        Vector delta(petal.get_x() - player.get_x(), petal.get_y() - player.get_y());
                        petal.friction = DEFAULT_FRICTION;
                        float angle = delta.angle();
                        if (petal.get_petal_id() == PetalID::kTriweb) angle += frand() - 0.5;
                        petal.acceleration.unit_normal(angle).set_magnitude(30 * PLAYER_ACCELERATION);
                        entity_set_despawn_tick(petal, 0.6 * TPS);
                    } else if (BitMath::at(player.input, InputFlags::kDefending))
                        entity_set_despawn_tick(petal, 0.6 * TPS);
                    break;
                }
                case PetalID::kBubble:
                    if (BitMath::at(player.input, InputFlags::kDefending)) {
                        Vector v(player.get_x() - petal.get_x(), player.get_y() - petal.get_y());
                        v.set_magnitude(PLAYER_ACCELERATION * 20);
                        player.velocity += v;
                        sim->request_delete(petal.id);
                    }
                    break;
                case PetalID::kPollen:
                    if (BitMath::at(player.input, InputFlags::kAttacking)) {
                        petal.friction = DEFAULT_FRICTION;
                        // give a gentle outward push
                        Vector delta(petal.get_x() - player.get_x(), petal.get_y() - player.get_y());
                        float out_angle = delta.angle();
                        petal.acceleration.unit_normal(out_angle).set_magnitude(4 * PLAYER_ACCELERATION);
                        entity_set_despawn_tick(petal, 4.0 * TPS);
                    }
                    else if (BitMath::at(player.input, InputFlags::kDefending)) {
                        petal.friction = DEFAULT_FRICTION;
                        Vector delta(petal.get_x() - player.get_x(), petal.get_y() - player.get_y());
                        float out_angle = delta.angle();
                        petal.acceleration.unit_normal(out_angle).set_magnitude(-.25 * PLAYER_ACCELERATION);
                        entity_set_despawn_tick(petal, 4.0 * TPS);
                    }
                    break;
                case PetalID::kPeas:
                case PetalID::kPoisonPeas:
                case PetalID::kFatPeas:
                    if (BitMath::at(player.input, InputFlags::kAttacking)) {
<<<<<<< HEAD
                        Vector delta(petal.get_x() - player.get_x(), petal.get_y() - player.get_y());
                        float base = delta.angle();
                        // spread around full circle
                        float spread = (frand() * 2.0f * (float)M_PI) - (float)M_PI;
                        float shoot_angle = base + spread;
                        petal.friction = DEFAULT_FRICTION;
                        petal.acceleration.unit_normal(shoot_angle).set_magnitude(30 * PLAYER_ACCELERATION);
                        entity_set_despawn_tick(petal, 0.32 * TPS);
                    }
                    break;
                case PetalID::kWax:
                    if (BitMath::at(player.input, InputFlags::kAttacking) || BitMath::at(player.input, InputFlags::kDefending)) {
                        petal.friction = 1.0f;
                        entity_set_despawn_tick(petal, 15.0f * TPS);
                    } else {
                        petal.friction = 1.0f;
                        entity_set_despawn_tick(petal, 15.0 * TPS);
=======
                        float spray_angle = frand() * 2 * M_PI;
                        petal.friction = DEFAULT_FRICTION;
                        petal.velocity.unit_normal(spray_angle).set_magnitude(20 * PLAYER_ACCELERATION);
                        petal.set_split_projectile(0);
                        petal.heading_angle = spray_angle;
                        entity_set_despawn_tick(petal, 1.0 * TPS);
                        for (uint32_t i = 1; i < petal_data.count; ++i) {
                            Entity &new_petal = alloc_petal(sim, petal.get_petal_id(), player);
                            new_petal.friction = DEFAULT_FRICTION;
                            new_petal.set_x(petal.get_x());
                            new_petal.set_y(petal.get_y());
                            new_petal.set_split_projectile(0);
                            new_petal.heading_angle = spray_angle + (2 * M_PI * i) / petal_data.count;
                            new_petal.velocity
                                .unit_normal(new_petal.heading_angle)
                                .set_magnitude(20 * PLAYER_ACCELERATION);
                            entity_set_despawn_tick(new_petal, 1.0 * TPS);
                        }
>>>>>>> upstream/master
                    }
                    break;
                default:
                    break;
            }
        } else petal.secondary_reload++;
    }
}