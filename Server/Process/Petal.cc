#include <Server/Process.hh>

#include <Server/EntityFunctions.hh>
#include <Server/Spawn.hh>
#include <Shared/Entity.hh>
#include <Shared/Simulation.hh>
#include <Shared/StaticData.hh>

#include <cmath>

void tick_petal_behavior(Simulation *sim, Entity &petal) {
    if (petal.pending_delete) return;
    if (!sim->ent_alive(petal.parent)) {
        sim->request_delete(petal.id);
        return;
    }
    Entity &player = sim->get_ent(petal.parent);
    struct PetalData const &petal_data = PETAL_DATA[petal.petal_id];
    if (petal_data.attributes.rotation_style == PetalAttributes::kPassiveRot) {
        //simulate on clientside
        float rot_amt = petal.petal_id == PetalID::kWing ? 10.0 : 1.0;
        if (petal.id.id % 2) petal.set_angle(petal.angle + rot_amt / TPS);
        else petal.set_angle(petal.angle - rot_amt / TPS);
    } else if (petal_data.attributes.rotation_style == PetalAttributes::kFollowRot && !(BIT_AT(petal.flags, EntityFlags::kIsDespawning))) {
        Vector delta(petal.x - player.x, petal.y - player.y);
        petal.set_angle(delta.angle());
    }
    if (BIT_AT(petal.flags, EntityFlags::kIsDespawning)) {
        switch (petal.petal_id) {
            case PetalID::kMissile: {
                petal.acceleration.unit_normal(petal.angle).set_magnitude(4 * PLAYER_ACCELERATION);
                break;
            }
            case PetalID::kMoon: {
                Vector delta(player.x - petal.x, player.y - petal.y);
                float magnitude = 20000 * PLAYER_ACCELERATION / (delta.x * delta.x + delta.y * delta.y);
                if (magnitude > PLAYER_ACCELERATION) magnitude = PLAYER_ACCELERATION;
                delta.set_magnitude(magnitude);
                petal.acceleration.set(delta.x, delta.y);
                break;
            }
            default:
                break;
        }
    }
    else if (petal_data.attributes.secondary_reload > 0) {
        if (petal.secondary_reload > petal_data.attributes.secondary_reload * TPS) {
            if (petal_data.attributes.burst_heal > 0 && player.health < player.max_health && player.dandy_ticks == 0) {
                Vector delta(player.x - petal.x, player.y - petal.y);
                if (delta.magnitude() < petal.radius) {
                    inflict_heal(sim, player, petal_data.attributes.burst_heal);
                    sim->request_delete(petal.id);
                    return;
                }
                delta.set_magnitude(PLAYER_ACCELERATION * 4);
                petal.acceleration = delta;
            }
            switch (petal.petal_id) {
                case PetalID::kMissile:
                    if (BIT_AT(player.input, InputFlags::kAttacking)) {
                        petal.acceleration.unit_normal(petal.angle).set_magnitude(4 * PLAYER_ACCELERATION);
                        entity_set_despawn_tick(petal, 3 * TPS);
                    }
                    break;
                case PetalID::kTriweb:
                case PetalID::kWeb: {
                    if (BIT_AT(player.input, InputFlags::kAttacking)) {
                        Vector delta(petal.x - player.x, petal.y - player.y);
                        petal.friction = DEFAULT_FRICTION;
                        float angle = delta.angle();
                        if (petal.petal_id == PetalID::kTriweb) angle += frand() - 0.5;
                        petal.acceleration.unit_normal(angle).set_magnitude(30 * PLAYER_ACCELERATION);
                        entity_set_despawn_tick(petal, 0.6 * TPS);
                    } else if (BIT_AT(player.input, InputFlags::kDefending))
                        entity_set_despawn_tick(petal, 0.6 * TPS);
                    break;
                }
                case PetalID::kBubble:
                    if (BIT_AT(player.input, InputFlags::kDefending)) {
                        Vector v(player.x - petal.x, player.y - petal.y);
                        v.set_magnitude(PLAYER_ACCELERATION * 15);
                        player.acceleration += v;
                        sim->request_delete(petal.id);
                    }
                    break;
                case PetalID::kPollen:
                    if (BIT_AT(player.input, InputFlags::kAttacking) || BIT_AT(player.input, InputFlags::kDefending)) {
                        petal.friction = DEFAULT_FRICTION;
                        entity_set_despawn_tick(petal, 4.0 * TPS);
                    }
                    break;
                case PetalID::kPeas:
                case PetalID::kPoisonPeas:
                    if (BIT_AT(player.input, InputFlags::kAttacking)) {
                        Vector delta(petal.x - player.x, petal.y - player.y);
                        petal.friction = DEFAULT_FRICTION;
                        petal.acceleration.unit_normal(delta.angle()).set_magnitude(25 * PLAYER_ACCELERATION);
                        entity_set_despawn_tick(petal, 0.25 * TPS);
                    }
                    break;
                case PetalID::kMoon: {
                    if (BIT_AT(player.input, InputFlags::kAttacking)) {
                        Vector delta(petal.x - player.x, petal.y - player.y);
                        petal.friction = 0;
                        petal.acceleration.unit_normal(delta.angle() + M_PI / 3).set_magnitude(3 * PLAYER_ACCELERATION);
                        entity_set_despawn_tick(petal, 10 * TPS);
                    }
                    break;
                }
                default:
                    break;
            }
        } else petal.secondary_reload++;
    }
}