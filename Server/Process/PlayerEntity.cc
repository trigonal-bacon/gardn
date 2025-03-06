#include <Server/Process/Process.hh>

#include <Server/EntityFunctions.hh>
#include <Server/Spawn.hh>
#include <Shared/Entity.hh>
#include <Shared/Simulation.hh>
#include <Shared/StaticData.hh>

static int petal_passive_buffs(Simulation *sim, Entity &ent, Entity &player) {
    int count = 0;
    for (uint8_t i = 0; i < ent.loadout_count; ++i) {
        struct PetalData const &petal_data = PETAL_DATA[ent.loadout[i].id];
        if (!ent.loadout[i].already_spawned) continue;
        if (petal_data.attributes.constant_heal > 0) {
            inflict_heal(sim, player, petal_data.attributes.constant_heal / TPS);
        }
    }
    return count;
}

void tick_player_behavior(Simulation *sim, Entity &player) {
    if (player.pending_delete) return;
    if (!sim->ent_alive(player.parent)) {
        sim->request_delete(player.id);
        return;
    }
    Entity &camera = sim->get_ent(player.parent);
    float count = (float) petal_passive_buffs(sim, camera, player);
    assert(player.loadout_count < MAX_SLOT_COUNT);
    float rot_pos = 0;
    player.set_face_flags(0);
    for (uint8_t i = 0; i < camera.loadout_count; ++i) {
        LoadoutSlot &slot = camera.loadout[i];
        struct PetalData const &petal_data = PETAL_DATA[slot.id];
        camera.set_loadout_ids(i, slot.id);
        if (slot.id == PetalID::kNone) continue;
        for (uint32_t j = 0; j < petal_data.count; ++j) {
            LoadoutPetal &petal_slot = slot.petals[j];
            if (!sim->ent_alive(petal_slot.ent_id)) {
                petal_slot.ent_id = NULL_ENTITY;
                if (petal_slot.reload >= petal_data.reload * TPS) {
                    Entity &petal = alloc_petal(slot.id, player);
                    petal.set_x(player.x);
                    petal.set_y(player.y);
                    petal.set_parent(player.id);
                    petal.set_team(player.parent);
                    petal_slot.ent_id = petal.id;
                    petal_slot.reload = 0;
                    petal.owner_slot = &petal_slot;
                    slot.already_spawned = 1;
                } else {
                    ++petal_slot.reload;
                }
            } else {
                Entity &petal = sim->get_ent(petal_slot.ent_id);
                if (petal.has_component(kPetal) && !petal.despawn_tick) {
                    //petal rotation behavior
                    Vector wanting;
                    Vector delta(player.x - petal.x, player.y - petal.y);
                    if (camera.rotation_count > 0) {
                        wanting.unit_normal(2 * M_PI * rot_pos / camera.rotation_count + player.petal_rotation);
                    }
                    if (BIT_AT(player.input, 0)) { 
                        if (petal_data.attributes.defend_only == 0) wanting *= 120; 
                        else wanting *= 60;
                    }
                    else if (BIT_AT(player.input, 1)) wanting *= 40;
                    else wanting *= 60;
                    if (petal_data.attributes.clump_radius > 0) {
                        Vector secondary;
                        secondary.unit_normal(2 * M_PI * j / petal_data.count + player.petal_rotation * 0.2)
                        .set_magnitude(petal_data.attributes.clump_radius);
                        wanting += secondary;
                    }
                    wanting += delta;
                    wanting *= 0.5;
                    petal.acceleration = wanting;
                } else {
                    if (petal.has_component(kMob)) --rot_pos;
                    //nothing tbh
                }
            }
            if (petal_data.attributes.clump_radius == 0) ++rot_pos;
        }
        if (petal_data.attributes.clump_radius > 0) ++rot_pos;
    };
    player.petal_rotation += 0.1;
    if (BIT_AT(player.input, 0)) player.set_face_flags(player.face_flags | 1);
    else if (BIT_AT(player.input, 1)) player.set_face_flags(player.face_flags | 2);
    camera.rotation_count = rot_pos;
}

void tick_petal_behavior(Simulation *sim, Entity &petal) {
    if (petal.pending_delete) return;
    if (!sim->ent_alive(petal.parent)) {
        sim->request_delete(petal.id);
        return;
    }
    Entity &player = sim->get_ent(petal.parent);
    struct PetalData const &petal_data = PETAL_DATA[petal.petal_id];
    if (petal_data.attributes.rotation_style == PetalAttributes::kPassiveRot) {
        if (petal.id.id % 2) petal.set_angle(petal.angle + 1.0 / TPS);
        else petal.set_angle(petal.angle - 1.0 / TPS);
    } else if (petal_data.attributes.rotation_style == PetalAttributes::kFollowRot && petal.despawn_tick == 0) {
        Vector delta(petal.x - player.x, petal.y - player.y);
        petal.set_angle(delta.angle());
    }
    if (petal.despawn_tick > 0) {
        switch (petal.petal_id) {
            case PetalID::kRose: {
                if (player.health == player.max_health) {
                    petal.despawn_tick = 0;
                    return;
                }
                Vector delta(player.x - petal.x, player.y - petal.y);
                if (delta.magnitude() < player.radius + petal.radius) {
                    inflict_heal(sim, player, petal_data.attributes.burst_heal);
                    sim->request_delete(petal.id);
                    return;
                }
                delta.normalize().set_magnitude(PLAYER_ACCELERATION * 8);
                petal.acceleration = delta;
                break;
            }
            case PetalID::kBeetleEgg:
            case PetalID::kAntEgg: {
                uint8_t spawn_id = (petal.petal_id == PetalID::kAntEgg) ? MobID::kSoldierAnt : MobID::kMassiveBeetle;
                Entity &mob = alloc_mob(spawn_id);
                mob.set_team(petal.team);
                mob.set_parent(petal.parent);
                mob.set_x(petal.x);
                mob.set_y(petal.y);
                if (petal.owner_slot != nullptr) petal.owner_slot->ent_id = mob.id;
                sim->request_delete(petal.id);
                break;
            }
            case PetalID::kMissile: {
                if (petal.despawn_tick > 3 * TPS) {
                    sim->request_delete(petal.id);
                    return;
                }
                petal.acceleration.unit_normal(petal.angle).set_magnitude(4.5 * PLAYER_ACCELERATION);
                break;
            }
            default: {
                sim->request_delete(petal.id);
                break;
            }
        }
        ++petal.despawn_tick;
    }
    else if (petal_data.attributes.secondary_reload > 0) {
        if (petal.secondary_reload > petal_data.attributes.secondary_reload * TPS) {
            switch (petal.petal_id) {
                case PetalID::kRose:
                    if (player.health != player.max_health) petal.despawn_tick++;
                    break;
                case PetalID::kMissile:
                    if (BIT_AT(player.input, 0)) {
                        petal.acceleration.unit_normal(petal.angle).set_magnitude(10 * PLAYER_ACCELERATION);
                        petal.despawn_tick++;
                        if (petal.owner_slot != nullptr) petal.owner_slot->ent_id = NULL_ENTITY;
                    }
                    break;
                default:
                    petal.despawn_tick++;
                    break;
            }
        } else petal.secondary_reload++;
    }
}