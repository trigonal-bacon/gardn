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
    uint8_t is_poisonous : 1;
    uint8_t has_cutter : 1;
    uint8_t equip_flags;
};

static struct PlayerBuffs _get_petal_passive_buffs(Simulation *sim, Entity &player) {
    struct PlayerBuffs buffs = {0};
    if (player.has_component(kMob)) return buffs;
    player.set_equip_flags(0);
    player.damage_reflection = 0;
    player.poison_armor = 0;
    for (uint32_t i = 0; i < player.get_loadout_count(); ++i) {
        LoadoutSlot const &slot = player.loadout[i];
        PetalID::T slot_petal_id = slot.get_petal_id();
        struct PetalData const &petal_data = PETAL_DATA[slot_petal_id];
        if (petal_data.attributes.equipment != EquipmentFlags::kNone)
            player.set_equip_flags(player.get_equip_flags() | (1 << petal_data.attributes.equipment));
        if (slot_petal_id == PetalID::kAntennae) {
            buffs.extra_vision = fclamp(0.4,buffs.extra_vision,1);
        } else if (slot_petal_id == PetalID::kObserver) {
            buffs.extra_vision = 0.75;
        } else if (slot_petal_id == PetalID::kThirdEye) {
            buffs.extra_range = 75;
        } else if (slot_petal_id == PetalID::kCutter) {
            buffs.has_cutter = 1;
        } else if (slot_petal_id == PetalID::kYinYang) {
            ++buffs.yinyang_count;
        }
        if (!player.loadout[i].already_spawned) continue;
        if (slot_petal_id == PetalID::kLeaf) 
            buffs.heal += petal_data.attributes.constant_heal / TPS;
        else if (slot_petal_id == PetalID::kYucca && BitMath::at(player.input, InputFlags::kDefending) && !BitMath::at(player.input, InputFlags::kAttacking)) 
            buffs.heal += petal_data.attributes.constant_heal / TPS;
        if (slot_petal_id == PetalID::kFaster) 
            buffs.extra_rot += 1.0;
        else if (slot_petal_id == PetalID::kCactus) 
            buffs.extra_health += 20;
        else if (slot_petal_id == PetalID::kTricac) 
            buffs.extra_health += 60;
        else if (slot_petal_id == PetalID::kPoisonCactus) {
            buffs.extra_health += 20;
            buffs.is_poisonous = 1;
        } else if (slot_petal_id == PetalID::kSalt) {
            player.damage_reflection = 0.25;
        } else if (slot_petal_id == PetalID::kLotus) {
            player.poison_armor = 3.5f / TPS;
        }
    }
    return buffs;
}

static uint32_t _get_petal_rotation_count(Simulation *sim, Entity &player) {
    uint32_t count = 0;
    for (uint8_t i = 0; i < player.get_loadout_count(); ++i) {
        LoadoutSlot const &slot = player.loadout[i];
        struct PetalData const &petal_data = PETAL_DATA[slot.get_petal_id()];
        if (petal_data.attributes.clump_radius > 0)
            ++count;
        else {
            for (uint32_t j = 0; j < slot.size(); ++j) {
                if (!sim->ent_alive(slot.petals[j].ent_id))
                    ++count;
                else if (!sim->get_ent(slot.petals[j].ent_id).has_component(kMob))
                    ++count;
            }
        }
    }
    return count;
}

void tick_player_behavior(Simulation *sim, Entity &player) {
    if (player.pending_delete) return;
    DEBUG_ONLY(assert(player.max_health > 0);)
    PlayerBuffs const buffs = _get_petal_passive_buffs(sim, player);
    float health_ratio = player.health / player.max_health;
    if (!player.has_component(kMob)) {
        player.max_health = hp_at_level(score_to_level(player.get_score())) + buffs.extra_health;
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
    uint32_t rotation_count = _get_petal_rotation_count(sim, player);
    //maybe use delta mode for face flags?
    player.set_face_flags(0);

    if (sim->ent_alive(player.get_parent())) {
        Entity &camera = sim->get_ent(player.get_parent());
        camera.set_fov(BASE_FOV * (1 - buffs.extra_vision));
    }

    DEBUG_ONLY(assert(player.get_loadout_count() <= MAX_SLOT_COUNT);)
    for (uint32_t i = 0; i < player.get_loadout_count(); ++i) {
        LoadoutSlot &slot = player.loadout[i];
        //player.set_loadout_ids(i, slot.id);
        //other way around. loadout_ids should dictate loadout
        if (slot.get_petal_id() != player.get_loadout_ids(i) || player.get_overlevel_timer() >= PETAL_DISABLE_DELAY * TPS)
            slot.update_id(sim, player.get_loadout_ids(i));
        PetalID::T slot_petal_id = slot.get_petal_id();
        struct PetalData const &petal_data = PETAL_DATA[slot_petal_id];
        DEBUG_ONLY(assert(petal_data.count <= MAX_PETALS_IN_CLUMP);)

        if (slot_petal_id == PetalID::kNone || petal_data.count == 0)
            continue;
        //if overleveled timer too large
        if (player.get_overlevel_timer() >= PETAL_DISABLE_DELAY * TPS) {
            player.set_loadout_reloads(i, 0);
            continue;
        }
        float min_reload = 1;
        for (uint32_t j = 0; j < slot.size(); ++j) {
            LoadoutPetal &petal_slot = slot.petals[j];
            if (!sim->ent_alive(petal_slot.ent_id)) {
                petal_slot.ent_id = NULL_ENTITY;
                game_tick_t reload_time = (petal_data.reload * TPS);
                if (!slot.already_spawned) reload_time += TPS;
                float this_reload = reload_time == 0 ? 1 : (float) petal_slot.reload / reload_time;
                min_reload = std::min(min_reload, this_reload);
                if (petal_slot.reload >= reload_time) {
                    petal_slot.ent_id = alloc_petal(sim, slot_petal_id, player).id;
                    petal_slot.reload = 0;
                    slot.already_spawned = 1;
                } 
                else
                    ++petal_slot.reload;
            }
            if (sim->ent_alive(petal_slot.ent_id)) {
                Entity &petal = sim->get_ent(petal_slot.ent_id);
                //only do this if petal not despawning
                if (petal.has_component(kPetal) && !(BitMath::at(petal.flags, EntityFlags::kIsDespawning))) {
                    //petal rotation behavior
                    Vector wanting;
                    Vector delta(player.get_x() - petal.get_x(), player.get_y() - petal.get_y());
                    if (rotation_count > 0)
                        wanting.unit_normal(2 * M_PI * rot_pos / rotation_count + player.heading_angle);

                    float range = player.get_radius() + 40;
                    if (BitMath::at(player.input, InputFlags::kAttacking)) { 
                        if (petal_data.attributes.defend_only == 0) 
                            range = player.get_radius() + 100 + buffs.extra_range; 
                        if (petal.get_petal_id() == PetalID::kWing) {
                            float wave = sinf((float) petal.lifetime / (0.4 * TPS));
                            wave = wave * wave;
                            range += wave * 120;
                        }
                    }
                    else if (BitMath::at(player.input, InputFlags::kDefending)) range = player.get_radius() + 15;
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
                        Entity &mob = alloc_mob(sim, spawn_id, petal.get_x(), petal.get_y(), petal.get_team());
                        mob.set_parent(player.id);
                        mob.set_color(player.get_color());
                        mob.base_entity = player.id;
                        BitMath::set(mob.flags, EntityFlags::kDieOnParentDeath);
                        BitMath::set(mob.flags, EntityFlags::kNoDrops);
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
                    if (BitMath::at(petal.flags, EntityFlags::kIsDespawning))
                        petal_slot.ent_id = NULL_ENTITY;
                    if (petal.has_component(kMob))
                        --rot_pos;
                }
            }
            //spread out
            if (petal_data.attributes.clump_radius == 0) ++rot_pos;
        }
        //clump
        if (petal_data.attributes.clump_radius > 0) ++rot_pos;
        player.set_loadout_reloads(i, min_reload * 255);
    };
    if (BitMath::at(player.input, InputFlags::kAttacking)) 
        player.set_face_flags(player.get_face_flags() | (1 << FaceFlags::kAttacking));
    else if (BitMath::at(player.input, InputFlags::kDefending))
        player.set_face_flags(player.get_face_flags() | (1 << FaceFlags::kDefending));
    if (player.poison_ticks > 0)
        player.set_face_flags(player.get_face_flags() | (1 << FaceFlags::kPoisoned));
    if (player.dandy_ticks > 0)
        player.set_face_flags(player.get_face_flags() | (1 << FaceFlags::kDandelioned));
    if (player.get_dev())
        player.set_face_flags(player.get_face_flags() | (1 << FaceFlags::kDeveloper));
    if (buffs.yinyang_count < 8) {
        switch (buffs.yinyang_count % 3) {
            case 0:
                player.heading_angle += (BASE_PETAL_ROTATION_SPEED + buffs.extra_rot) / TPS;
                break;
            case 1:
                player.heading_angle -= (BASE_PETAL_ROTATION_SPEED + buffs.extra_rot) / TPS;
                break;
            default:
                break;
        }
    } else 
        player.heading_angle += 10 * (BASE_PETAL_ROTATION_SPEED + buffs.extra_rot) / TPS;
}