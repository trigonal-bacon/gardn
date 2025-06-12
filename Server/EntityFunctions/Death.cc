#include <Server/EntityFunctions.hh>

#include <Server/PetalTracker.hh>
#include <Server/Spawn.hh>

#include <Shared/Entity.hh>
#include <Shared/Helpers.hh>
#include <Shared/Map.hh>
#include <Shared/Vector.hh>

#include <algorithm>

static void __alloc_drops(std::vector<PetalID::T> &success_drops, float x, float y) {
    #ifdef DEBUG
    for (PetalID::T id : success_drops)
        assert(id != PetalID::kNone && id < PetalID::kNumPetals);
    #endif
    size_t count = success_drops.size();
    for (size_t i = count; i > 0; --i) {
        PetalID::T drop_id = success_drops[i - 1];
        if (PETAL_DATA[drop_id].rarity == RarityID::kUnique && PetalTracker::get_count(drop_id) > 0) {
            success_drops[i] = success_drops[count - 1];
            --count;
            success_drops.pop_back();
        }
    }
    DEBUG_ONLY(assert(success_drops.size() == count);)
    if (count > 1) {
        for (size_t i = 0; i < count; ++i) {
            Entity &drop = alloc_drop(success_drops[i]);
            drop.set_x(x);
            drop.set_y(y);
            drop.velocity.unit_normal(i * 2 * M_PI / count).set_magnitude(PLAYER_ACCELERATION * 8.5);
        }
    } else if (count == 1) {
        Entity &drop = alloc_drop(success_drops[0]);
        drop.set_x(x);
        drop.set_y(y);
    }
}

static void __add_score(Simulation *sim, EntityID const killer_id, Entity const &target) {
    if (!sim->ent_alive(killer_id)) return;
    Entity &killer = sim->get_ent(killer_id);
    if (killer.has_component(kFlower)) {
        if (target.has_component(kMob))
            killer.set_score(killer.score + target.score);
        else
            killer.set_score(killer.score + target.score / 2);
    }
    if (target.has_component(kFlower) && sim->ent_alive(target.parent)) {
        Entity &camera = sim->get_ent(target.parent);
        if (!killer.has_component(kName)) camera.set_killed_by("");
        else camera.set_killed_by(killer.name);
    }
}

void entity_on_death(Simulation *sim, Entity const &ent) {
    //don't do on_death for any despawned entity
    uint8_t natural_despawn = (ent.flags & EntityFlags::IsDespawning) && ent.despawn_tick == 0;
    if (ent.has_component(kScore) && sim->ent_exists(ent.last_damaged_by) && !natural_despawn) {
        EntityID killer_id = sim->get_ent(ent.last_damaged_by).base_entity;
        __add_score(sim, killer_id, ent);
    }
    if (ent.has_component(kMob) && !natural_despawn) {
        if (!(ent.team == NULL_ENTITY)) return;
        if (ent.flags & EntityFlags::SpawnedFromZone)
            Map::remove_mob(ent.zone);
        struct MobData const &mob_data = MOB_DATA[ent.mob_id];
        std::vector<struct MobDrop> const &drops = mob_data.drops;
        std::vector<PetalID::T> success_drops = {};
        for (MobDrop const &d : drops) 
            if (frand() < d.chance) success_drops.push_back(d.id);
        __alloc_drops(success_drops, ent.x, ent.y);
        if (ent.mob_id == MobID::kAntHole && frand() < 0.10) { 
            EntityID team = NULL_ENTITY;
            if (sim->ent_exists(ent.last_damaged_by))
                team = sim->get_ent(ent.last_damaged_by).team;
            alloc_mob(MobID::kDigger, ent.x, ent.y, team);
        }

    } else if (ent.has_component(kPetal)) {
        if (ent.petal_id == PetalID::kWeb || ent.petal_id == PetalID::kTriweb) {
            alloc_web(100, ent);
        }
    } else if (ent.has_component(kFlower)) {
        std::vector<PetalID::T> potential = {};
        for (uint32_t i = 0; i < ent.loadout_count + MAX_SLOT_COUNT; ++i) {
            DEBUG_ONLY(assert(ent.loadout_ids[i] < PetalID::kNumPetals));
            PetalTracker::remove_petal(ent.loadout_ids[i]);
            if (ent.loadout_ids[i] != PetalID::kNone && ent.loadout_ids[i] != PetalID::kBasic && frand() < 0.95)
                potential.push_back(ent.loadout_ids[i]);
        }
        for (uint32_t i = 0; i < ent.deleted_petals.size(); ++i) {
            DEBUG_ONLY(assert(ent.deleted_petals[i] < PetalID::kNumPetals));
            PetalTracker::remove_petal(ent.deleted_petals[i]);
            if (ent.deleted_petals[i] != PetalID::kNone && ent.deleted_petals[i] != PetalID::kBasic && frand() < 0.95)
                potential.push_back(ent.deleted_petals[i]);
        }
        //no need to deleted_petals.clear, the player dies
        std::sort(potential.begin(), potential.end(), [](PetalID::T a, PetalID::T b) {
            return PETAL_DATA[a].rarity < PETAL_DATA[b].rarity;
        });

        std::vector<PetalID::T> success_drops = {};
        uint32_t numDrops = potential.size();
        if (numDrops > 3)
            numDrops = 3;
        for (uint32_t i = 0; i < numDrops; ++i) {
            success_drops.push_back(potential.back());
            potential.pop_back();
        }
        __alloc_drops(success_drops, ent.x, ent.y);
        //if the camera is the one that disconnects
        //no need to re-add the petals to the petal tracker
        if (!sim->ent_alive(ent.parent))
            return;
        Entity &camera = sim->get_ent(ent.parent);
        //reset all reloads and stuff
        uint32_t numLeft = potential.size();
        //set respawn level
        camera.set_respawn_level(div_round_up(3 * score_to_level(ent.score), 4));
        uint32_t max_possible = MAX_SLOT_COUNT + loadout_slots_at_level(camera.respawn_level);
        if (numLeft > max_possible) numLeft = max_possible;
        //fill petals
        for (uint32_t i = 0; i < 2 * MAX_SLOT_COUNT; ++i)
            camera.set_inventory(i, PetalID::kNone); //force reset
        for (uint32_t i = 0; i < numLeft; ++i) {
            DEBUG_ONLY(assert(potential.back() < PetalID::kNumPetals));
            PetalTracker::add_petal(potential.back());
            camera.set_inventory(i, potential.back());
            potential.pop_back();
        }
        //only track up to max_possible
        for (uint32_t i = numLeft; i < max_possible; ++i)
            camera.set_inventory(i, PetalID::kNone); //don't track kNone
        //fill with basics
        for (uint32_t i = numLeft; i < ent.loadout_count; ++i) {
            PetalTracker::add_petal(PetalID::kBasic);
            camera.set_inventory(i, PetalID::kBasic);
        }
    } else if (ent.has_component(kDrop)) {
        if (ent.flags & EntityFlags::IsDespawning)
            PetalTracker::remove_petal(ent.drop_id);
    }
}