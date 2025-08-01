#include <Server/Process.hh>

#include <Server/EntityFunctions.hh>
#include <Server/Spawn.hh>
#include <Shared/Entity.hh>
#include <Shared/Simulation.hh>
#include <Shared/StaticData.hh>

#include <cmath>

struct PlayerBuffs {
    float extra_rot;
    float extra_range;
    float heal;
    float extra_vision;
    float extra_health;
    uint8_t yinyang_count;
    uint8_t has_antennae : 1;
    uint8_t has_observer : 1;
    uint8_t is_poisonous : 1;
    uint8_t has_cutter : 1;
};

static struct PlayerBuffs petal_passive_buffs(Simulation *sim, Entity &player) {
    struct PlayerBuffs buffs = {0};
    if (player.has_component(kMob)) return buffs;
    int count = 0;
    player.damage_reflection = 0;
    player.poison_armor = 0;
    for (uint8_t i = 0; i < player.loadout_count; ++i) {
        LoadoutSlot const &slot = player.loadout[i];
        struct PetalData const &petal_data = PETAL_DATA[slot.id];

        if (slot.id == PetalID::kAntennae) {
            buffs.has_antennae = 1;
            buffs.extra_vision = fclamp(0.4,buffs.extra_vision,1);
        } else if (slot.id == PetalID::kObserver) {
            buffs.has_observer = 1;
            buffs.extra_vision = 0.75;
        } else if (slot.id == PetalID::kThirdEye) {
            buffs.extra_range = 75;
        } else if (slot.id == PetalID::kCutter) {
            buffs.has_cutter = 1;
        } else if (slot.id == PetalID::kYinYang) {
            ++buffs.yinyang_count;
        }
        if (!player.loadout[i].already_spawned) continue;
        if (slot.id == PetalID::kLeaf) 
            buffs.heal += petal_data.attributes.constant_heal / TPS;
        else if (slot.id == PetalID::kYucca && BIT_AT(player.input, InputFlags::kDefending) && !BIT_AT(player.input, InputFlags::kAttacking)) 
            buffs.heal += petal_data.attributes.constant_heal / TPS;
        if (slot.id == PetalID::kFaster) 
            buffs.extra_rot += 1.0;
        else if (slot.id == PetalID::kCactus) 
            buffs.extra_health += 20;
        else if (slot.id == PetalID::kTricac) 
            buffs.extra_health += 60;
        else if (slot.id == PetalID::kPoisonCactus) {
            buffs.extra_health += 20;
            buffs.is_poisonous = 1;
        } else if (slot.id == PetalID::kSalt) {
            player.damage_reflection = 0.25;
        } else if (slot.id == PetalID::kLotus) {
            player.poison_armor = 2.5f / TPS;
        }
    }
    return buffs;
}

void tick_player_behavior(Simulation *sim, Entity &player) {
    if (player.pending_delete) return;
    DEBUG_ONLY(assert(player.max_health > 0);)
    PlayerBuffs const buffs = petal_passive_buffs(sim, player);
    float health_ratio = player.health / player.max_health;
    if (!player.has_component(kMob)) {
        player.max_health = hp_at_level(score_to_level(player.score)) + buffs.extra_health;
        if (buffs.has_cutter) player.damage = BASE_BODY_DAMAGE + 20;
        else player.damage = BASE_BODY_DAMAGE;
    }
    player.health = health_ratio * player.max_health;
    if (buffs.heal > 0)
        inflict_heal(sim, player, buffs.heal);
    if (buffs.is_poisonous)
        player.poison_damage = {10.0, 2};
    else
        player.poison_damage = {0, 0};
    
    float rot_pos = 0;
    player.set_face_flags(0);

    if (sim->ent_alive(player.parent)) {
        Entity &camera = sim->get_ent(player.parent);
        camera.set_fov(BASE_FOV * (1 - buffs.extra_vision));
    }

    DEBUG_ONLY(assert(player.loadout_count <= MAX_SLOT_COUNT);)
    for (uint8_t i = 0; i < player.loadout_count; ++i) {
        LoadoutSlot &slot = player.loadout[i];
        struct PetalData const &petal_data = PETAL_DATA[slot.id];
        DEBUG_ONLY(assert(petal_data.count <= MAX_PETALS_IN_CLUMP);)
        //player.set_loadout_ids(i, slot.id);
        //other way around. loadout_ids should dictate loadout
        if (slot.id != player.loadout_ids[i] || player.overlevel_timer >= PETAL_DISABLE_DELAY * TPS) {
            //delete all old petals
            for (uint32_t j = 0; j < petal_data.count; ++j) {
                LoadoutPetal &petal_slot = slot.petals[j];
                if (sim->ent_alive(petal_slot.ent_id))
                    sim->request_delete(petal_slot.ent_id);
            }
            slot.reset();
            slot.id = player.loadout_ids[i];
        }

        if (slot.id == PetalID::kNone || petal_data.count == 0)
            continue;
        //if overleveled timer too large
        if (player.overlevel_timer >= PETAL_DISABLE_DELAY * TPS) {
            player.set_loadout_reloads(i, 0);
            continue;
        }
        float min_reload = 1;
        for (uint32_t j = 0; j < petal_data.count; ++j) {
            LoadoutPetal &petal_slot = slot.petals[j];
            if (!sim->ent_alive(petal_slot.ent_id)) {
                petal_slot.ent_id = NULL_ENTITY;
                uint32_t reload_time = (petal_data.reload * TPS);
                if (!slot.already_spawned) reload_time += TPS;
                float this_reload = reload_time == 0 ? 1 : (float) petal_slot.reload / reload_time;
                if (this_reload < min_reload) min_reload = this_reload;
                if (petal_slot.reload >= reload_time) {
                    Entity &petal = alloc_petal(sim, slot.id, player);
                    petal_slot.ent_id = petal.id;
                    petal_slot.reload = 0;
                    slot.already_spawned = 1;
                } 
                else
                    ++petal_slot.reload;
            }
            if (sim->ent_alive(petal_slot.ent_id)) {
                Entity &petal = sim->get_ent(petal_slot.ent_id);
                //only do this if petal not despawning
                if (petal.has_component(kPetal) && !(BIT_AT(petal.flags, EntityFlags::kIsDespawning))) {
                    //petal rotation behavior
                    Vector wanting;
                    Vector delta(player.x - petal.x, player.y - petal.y);
                    if (player.rotation_count > 0)
                        wanting.unit_normal(2 * M_PI * rot_pos / player.rotation_count + player.heading_angle);

                    float range = 60;
                    if (BIT_AT(player.input, InputFlags::kAttacking)) { 
                        if (petal_data.attributes.defend_only == 0) 
                            range = 120 + buffs.extra_range; 
                        if (petal.petal_id == PetalID::kWing) {
                            float wave = sinf((float) petal.lifetime / (0.4 * TPS));
                            wave = wave * wave;
                            range += wave * 120;
                        }
                    }
                    else if (BIT_AT(player.input, InputFlags::kDefending)) range = 40;
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
                    game_tick_t sec_reload_ticks = petal_data.attributes.secondary_reload * TPS;
                    if (petal_data.attributes.spawns != MobID::kNumMobs &&
                        petal.secondary_reload > sec_reload_ticks) {
                        uint8_t spawn_id = petal_data.attributes.spawns;
                        Entity &mob = alloc_mob(sim, spawn_id, petal.x, petal.y, petal.team);
                        mob.set_parent(player.id);
                        mob.base_entity = player.id;
                        mob.set_score(0);
                        BIT_SET(mob.flags, EntityFlags::kDieOnParentDeath)
                        BIT_SET(mob.flags, EntityFlags::kNoDrops)
                        if (petal_data.attributes.spawn_count == 0) {
                            petal_slot.ent_id = mob.id;
                            sim->request_delete(petal.id);
                            break;
                        } else {
                            entity_set_despawn_tick(mob, sec_reload_ticks * petal_data.attributes.spawn_count);
                            petal.secondary_reload = 0;
                            //needed
                            mob.set_parent(petal.id);
                            mob.base_entity = player.id;
                        }
                    }
                } else {
                    //if petal is a mob, or detached (IsDespawning)
                    if (petal.has_component(kMob)) --rot_pos;
                    else petal_slot.ent_id = NULL_ENTITY;
                }
            }
            //spread out
            if (petal_data.attributes.clump_radius == 0) ++rot_pos;
        }
        //clump
        if (petal_data.attributes.clump_radius > 0) ++rot_pos;
        player.set_loadout_reloads(i, min_reload * 255);
    };
    if (BIT_AT(player.input, InputFlags::kAttacking)) 
        player.set_face_flags(player.face_flags | (1 << FaceFlags::kAttacking));
    else if (BIT_AT(player.input, InputFlags::kDefending))
        player.set_face_flags(player.face_flags | (1 << FaceFlags::kDefending));
    if (player.poison_ticks > 0) player.set_face_flags(player.face_flags | (1 << FaceFlags::kPoisoned));
    if (player.dandy_ticks > 0) player.set_face_flags(player.face_flags | (1 << FaceFlags::kDandelioned));
    if (buffs.extra_range > 0) player.set_face_flags(player.face_flags | (1 << FaceFlags::kThirdEye));
    if (buffs.has_antennae) player.set_face_flags(player.face_flags | (1 << FaceFlags::kAntennae));
    if (buffs.has_observer) player.set_face_flags(player.face_flags | (1 << FaceFlags::kObserver));
    if (buffs.has_cutter) player.set_face_flags(player.face_flags | (1 << FaceFlags::kCutter));
    if (buffs.yinyang_count != MAX_SLOT_COUNT) {
        switch (buffs.yinyang_count % 3) {
            case 0:
                player.heading_angle += (2.5 + buffs.extra_rot) / TPS;
                break;
            case 1:
                player.heading_angle -= (2.5 + buffs.extra_rot) / TPS;
                break;
            default:
                break;
        }
    } else 
        player.heading_angle += 10 * (2.5 + buffs.extra_rot) / TPS;

    player.rotation_count = rot_pos;
}