#include <Server/Process/Process.hh>

#include <Server/EntityFunctions.hh>
#include <Server/Spawn.hh>
#include <Shared/Entity.hh>
#include <Shared/Simulation.hh>
#include <Shared/StaticData.hh>

#include <cmath>

struct _PlayerBuffs {
    float extra_rot;
    float extra_range;
    float heal;
    float extra_vision;
    float extra_health;
    uint8_t has_antennae : 1;
    uint8_t has_observer : 1;
};

//maybe reconsider
static struct _PlayerBuffs petal_passive_buffs(Simulation *sim, Entity &player) {
    struct _PlayerBuffs buffs = {};
    int count = 0;
    for (uint8_t i = 0; i < player.loadout_count; ++i) {
        LoadoutSlot const &slot = player.loadout[i];
        struct PetalData const &petal_data = PETAL_DATA[slot.id];
        if (!player.loadout[i].already_spawned) continue;
        if (petal_data.attributes.constant_heal > 0) {
            buffs.heal += petal_data.attributes.constant_heal / TPS;
            //inflict_heal(sim, player, petal_data.attributes.constant_heal / TPS);
        }
        if (slot.id == PetalID::kFaster) {
            buffs.extra_rot += 1.0;
        } else if (slot.id == PetalID::kThirdEye) {
            buffs.extra_range = 75;
        } else if (slot.id == PetalID::kAntennae) {
            buffs.has_antennae = 1;
            buffs.extra_vision = fclamp(0.25,buffs.extra_vision,1);
        } else if (slot.id == PetalID::kObserver) {
            buffs.has_observer = 1;
            buffs.extra_vision = 0.75;
        } else if (slot.id == PetalID::kCactus) {
            buffs.extra_health += 20;
        } else if (slot.id == PetalID::kTricac) {
            buffs.extra_health += 60;
        }
    }
    return buffs;
}

void tick_player_behavior(Simulation *sim, Entity &player) {
    if (player.pending_delete) return;
    
    struct _PlayerBuffs buffs = petal_passive_buffs(sim, player);

    if (buffs.heal > 0)
        inflict_heal(sim, player, buffs.heal);
    float health_ratio = player.health / player.max_health;
    player.max_health = hpAtLevel(scoreToLevel(player.score)) + buffs.extra_health;
    player.health = health_ratio * player.max_health;
    float rot_pos = 0;
    player.set_face_flags(0);

    if (sim->ent_alive(player.parent)) {
        Entity &camera = sim->get_ent(player.parent);
        camera.set_fov(BASE_FOV * (1 - buffs.extra_vision));
    }

    assert(player.loadout_count <= MAX_SLOT_COUNT);
    for (uint8_t i = 0; i < player.loadout_count; ++i) {
        LoadoutSlot &slot = player.loadout[i];
        struct PetalData const &petal_data = PETAL_DATA[slot.id];
        player.set_loadout_ids(i, slot.id);
        if (slot.id == PetalID::kNone) continue;
        if (petal_data.count == 0) {
            slot.already_spawned = 1;
            continue;
        }
        for (uint32_t j = 0; j < petal_data.count; ++j) {
            LoadoutPetal &petal_slot = slot.petals[j];
            if (!sim->ent_alive(petal_slot.ent_id)) {
                petal_slot.ent_id = NULL_ENTITY;
                if (petal_slot.reload >= petal_data.reload * TPS) {
                    Entity &petal = alloc_petal(slot.id, player);
                    petal_slot.ent_id = petal.id;
                    petal_slot.reload = 0;
                    slot.already_spawned = 1;
                } 
                else 
                    ++petal_slot.reload;
            } else {
                Entity &petal = sim->get_ent(petal_slot.ent_id);
                //only do this if petal not despawning (detached <-> despawn_tick != 0)
                if (petal.has_component(kPetal) && !(petal.flags & EntityFlags::IsDespawning)) {
                    //petal rotation behavior
                    Vector wanting;
                    Vector delta(player.x - petal.x, player.y - petal.y);
                    if (player.rotation_count > 0) {
                        wanting.unit_normal(2 * M_PI * rot_pos / player.rotation_count + player.heading_angle);
                    }
                    float range = 60;
                    if (BIT_AT(player.input, 0)) { 
                        if (petal_data.attributes.defend_only == 0) 
                            range = 120 + buffs.extra_range; 
                        if (petal.petal_id == PetalID::kWing) {
                            float wave = sinf((float) petal.lifetime / (0.4 * TPS));
                            wave = wave * wave;
                            range += wave * 120;
                        }
                    }
                    else if (BIT_AT(player.input, 1)) range = 40;
                    wanting *= range;
                    if (petal_data.attributes.clump_radius > 0) {
                        Vector secondary;
                        secondary.unit_normal(2 * M_PI * j / petal_data.count + player.heading_angle * 0.2)
                        .set_magnitude(petal_data.attributes.clump_radius);
                        wanting += secondary;
                    }
                    wanting += delta;
                    wanting *= 0.5;
                    petal.acceleration = wanting;
                    if (petal_data.attributes.spawns != MobID::kNumMobs &&
                        petal.secondary_reload > petal_data.attributes.secondary_reload * TPS) {
                        uint8_t spawn_id = petal_data.attributes.spawns;
                        Entity &mob = alloc_mob(spawn_id, petal.x, petal.y, petal.team);
                        entity_set_owner(mob, petal.parent);
                        mob.flags |= EntityFlags::DieOnParentDeath;
                        if (petal_data.attributes.spawn_count == 0) {
                            petal_slot.ent_id = mob.id;
                            sim->request_delete(petal.id);
                            break;
                        } else {
                            mob.set_despawn_tick(petal_data.attributes.secondary_reload * TPS * petal_data.attributes.spawn_count);
                            petal.secondary_reload = 0;
                            //needed
                            mob.set_parent(petal.id);
                        }
                    }
                } else 
                    if (petal.has_component(kMob)) --rot_pos;
            }
            if (petal_data.attributes.clump_radius == 0) ++rot_pos;
        }
        if (petal_data.attributes.clump_radius > 0) ++rot_pos;
    };
    player.heading_angle += (2.5 + buffs.extra_rot) / TPS;
    if (BIT_AT(player.input, 0)) player.set_face_flags(player.face_flags | 1);
    else if (BIT_AT(player.input, 1)) player.set_face_flags(player.face_flags | 2);
    if (player.poison.time > 0) player.set_face_flags(player.face_flags | 4);
    if (player.dandy_ticks > 0) player.set_face_flags(player.face_flags | 8);
    if (buffs.extra_range > 0) player.set_face_flags(player.face_flags | 16);
    if (buffs.has_antennae) player.set_face_flags(player.face_flags | 32);
    if (buffs.has_observer) player.set_face_flags(player.face_flags | 64);
    player.rotation_count = rot_pos;
}