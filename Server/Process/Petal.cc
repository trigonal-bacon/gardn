#include <Server/Process/Process.hh>

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
        float rot_amt = petal.petal_id == PetalID::kWing ? 10.0 : 1.0;
        if (petal.id.id % 2) petal.set_angle(petal.angle + rot_amt / TPS);
        else petal.set_angle(petal.angle - rot_amt / TPS);
    } else if (petal_data.attributes.rotation_style == PetalAttributes::kFollowRot && !(petal.flags & EntityFlags::IsDespawning)) {
        Vector delta(petal.x - player.x, petal.y - player.y);
        petal.set_angle(delta.angle());
    }
    if (petal.flags & EntityFlags::IsDespawning) {
        switch (petal.petal_id) {
            case PetalID::kRose:
            case PetalID::kDahlia:
            case PetalID::kAzalea: {
                if (player.health == player.max_health) {
                    petal.flags &= ~EntityFlags::IsDespawning;
                    return;
                }
                petal.despawn_tick = 10; //prevent despawn
                Vector delta(player.x - petal.x, player.y - petal.y);
                if (delta.magnitude() < petal.radius) {
                    inflict_heal(sim, player, petal_data.attributes.burst_heal);
                    sim->request_delete(petal.id);
                    return;
                }
                //if (player.has_component(kFlower)) 
                delta.normalize().set_magnitude(PLAYER_ACCELERATION * 5);
                //else delta.normalize().set_magnitude(PLAYER_ACCELERATION * 5);
                petal.acceleration = delta;
                break;
            }
            case PetalID::kMissile: {
                petal.acceleration.unit_normal(petal.angle).set_magnitude(4.5 * PLAYER_ACCELERATION);
                break;
            }
            default:
                break;
        }
    }
    else if (petal_data.attributes.secondary_reload > 0) {
        if (petal.secondary_reload > petal_data.attributes.secondary_reload * TPS) {
            switch (petal.petal_id) {
                case PetalID::kDahlia:
                case PetalID::kRose:
                case PetalID::kAzalea:
                    if (player.health != player.max_health && player.dandy_ticks == 0)
                        petal.set_despawn_tick(10); //doesn't matter what it is
                    break;
                case PetalID::kMissile:
                    if (BIT_AT(player.input, 0)) {
                        petal.acceleration.unit_normal(petal.angle).set_magnitude(10 * PLAYER_ACCELERATION);
                        petal.set_despawn_tick(3 * TPS);
                    }
                    break;
                case PetalID::kTriweb:
                case PetalID::kWeb: {
                    if (BIT_AT(player.input, 0)) {
                        Vector delta(petal.x - player.x, petal.y - player.y);
                        petal.friction = DEFAULT_FRICTION;
                        float angle = delta.angle();
                        if (petal.petal_id == PetalID::kTriweb) angle += frand() - 0.5;
                        petal.acceleration.unit_normal(angle).set_magnitude(30 * PLAYER_ACCELERATION);
                        petal.set_despawn_tick(0.6 * TPS);
                    } else if (BIT_AT(player.input, 1))
                        petal.set_despawn_tick(0.6 * TPS);
                    break;
                }
                case PetalID::kBubble:
                    if (BIT_AT(player.input, 1)) {
                        Vector v(player.x - petal.x, player.y - petal.y);
                        v.set_magnitude(PLAYER_ACCELERATION * 10);
                        player.acceleration += v;
                        sim->request_delete(petal.id);
                    }
                    break;
                case PetalID::kPollen:
                    if (BIT_AT(player.input, 0) || BIT_AT(player.input, 1)) {
                        petal.friction = DEFAULT_FRICTION;
                        petal.set_despawn_tick(4.0 * TPS);
                    }
                    break;
                case PetalID::kPeas:
                case PetalID::kPoisonPeas:
                    if (BIT_AT(player.input, 0)) {
                        Vector delta(petal.x - player.x, petal.y - player.y);
                        petal.friction = DEFAULT_FRICTION;
                        petal.acceleration.unit_normal(delta.angle()).set_magnitude(25 * PLAYER_ACCELERATION);
                        petal.set_despawn_tick(0.25 * TPS);
                    }
                    break;
                default:
                    //petal.despawn_tick++;
                    break;
            }
        } else petal.secondary_reload++;
    }
}