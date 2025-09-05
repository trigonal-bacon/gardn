#include <Server/EntityFunctions.hh>

#include <Server/PetalTracker.hh>
#include <Server/Spawn.hh>

#include <Shared/Entity.hh>
#include <Shared/Map.hh>
#include <Shared/Simulation.hh>

#include <algorithm>
#include <iostream>

static void _alloc_drops(Simulation *sim, std::vector<PetalID::T> &success_drops, float x, float y) {
    #ifdef DEBUG
    for (PetalID::T id : success_drops)
        assert(id != PetalID::kNone && id < PetalID::kNumPetals);
    #endif
    size_t count = success_drops.size();
    for (size_t i = count; i > 0; --i) {
        PetalID::T drop_id = success_drops[i - 1];
        if (PETAL_DATA[drop_id].rarity == RarityID::kUnique && PetalTracker::get_count(sim, drop_id) > 0) {
            success_drops[i] = success_drops[count - 1];
            --count;
            success_drops.pop_back();
            PetalTracker::remove_petal(sim, drop_id);
        }
    }
    DEBUG_ONLY(assert(success_drops.size() == count);)
    if (count > 1) {
        for (size_t i = 0; i < count; ++i) {
            Entity &drop = alloc_drop(sim, success_drops[i]);
            drop.set_x(x);
            drop.set_y(y);
            drop.velocity.unit_normal(i * 2 * M_PI / count).set_magnitude(25);
        }
    } else if (count == 1) {
        Entity &drop = alloc_drop(sim, success_drops[0]);
        drop.set_x(x);
        drop.set_y(y);
    }
}

static void _add_score(Simulation *sim, EntityID const killer_id, Entity const &target) {
    if (!sim->ent_exists(killer_id)) return;
    Entity &killer = sim->get_ent(killer_id);
    if (killer.has_component(kScore))
        killer.set_score(killer.get_score() + target.score_reward);
}

void entity_on_death(Simulation *sim, Entity const &ent) {
    //don't do on_death for any despawned entity
    uint8_t natural_despawn = BitMath::at(ent.flags, EntityFlags::kIsDespawning) && ent.despawn_tick == 0;
    if (ent.score_reward > 0 && !natural_despawn)
        _add_score(sim, ent.last_damaged_by, ent);
    if (ent.has_component(kFlower) && sim->ent_alive(ent.get_parent())) {
        Entity &camera = sim->get_ent(ent.get_parent());
        if (sim->ent_exists(ent.last_damaged_by)) {
            Entity const &killer = sim->get_ent(ent.last_damaged_by);
            if (killer.has_component(kName)) camera.set_killed_by(killer.get_name());
            else camera.set_killed_by("");
        } else if (ent.poison_ticks > 0) camera.set_killed_by("Poison");
        else camera.set_killed_by("");
    }
    if (ent.has_component(kMob)) {
        if (BitMath::at(ent.flags, EntityFlags::kSpawnedFromZone))
            Map::remove_mob(sim, ent.zone);
        if (!natural_despawn && !(BitMath::at(ent.flags, EntityFlags::kNoDrops))) {
            struct MobData const &mob_data = MOB_DATA[ent.get_mob_id()];
            std::vector<PetalID::T> success_drops = {};
            StaticArray<float, MAX_DROPS_PER_MOB> const &drop_chances = MOB_DROP_CHANCES[ent.get_mob_id()];
            for (uint32_t i = 0; i < mob_data.drops.size(); ++i) 
                if (frand() < drop_chances[i]) success_drops.push_back(mob_data.drops[i]);
            _alloc_drops(sim, success_drops, ent.get_x(), ent.get_y());
        }
        if (ent.get_mob_id() == MobID::kAntHole && ent.get_team() == NULL_ENTITY && frand() < DIGGER_SPAWN_CHANCE) { 
            EntityID team = NULL_ENTITY;
            if (sim->ent_exists(ent.last_damaged_by))
                team = sim->get_ent(ent.last_damaged_by).get_team();
            alloc_mob(sim, MobID::kDigger, ent.get_x(), ent.get_y(), team);
        }

    } else if (ent.has_component(kPetal)) {
        if (ent.get_petal_id() == PetalID::kWeb || ent.get_petal_id() == PetalID::kTriweb)
            alloc_web(sim, 100, ent);
    } else if (ent.has_component(kFlower)) {
        std::vector<PetalID::T> potential = {};
        for (uint32_t i = 0; i < ent.get_loadout_count() + MAX_SLOT_COUNT; ++i) {
            DEBUG_ONLY(assert(ent.get_loadout_ids(i) < PetalID::kNumPetals));
            PetalTracker::remove_petal(sim, ent.get_loadout_ids(i));
            if (ent.get_loadout_ids(i) != PetalID::kNone && ent.get_loadout_ids(i) != PetalID::kBasic && frand() < 0.95)
                potential.push_back(ent.get_loadout_ids(i));
        }
        for (uint32_t i = 0; i < ent.deleted_petals.size(); ++i) {
            DEBUG_ONLY(assert(ent.deleted_petals[i] < PetalID::kNumPetals));
            PetalTracker::remove_petal(sim, ent.deleted_petals[i]);
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
            PetalID::T p_id = potential.back();
            if (PETAL_DATA[p_id].rarity >= RarityID::kRare && frand() < 0.05) p_id = PetalID::kPollen;
            success_drops.push_back(p_id);
            potential.pop_back();
        }
        _alloc_drops(sim, success_drops, ent.get_x(), ent.get_y());
        //if the camera is the one that disconnects
        //no need to re-add the petals to the petal tracker
        if (!sim->ent_alive(ent.get_parent()))
            return;
        Entity &camera = sim->get_ent(ent.get_parent());
        //reset all reloads and stuff
        uint32_t num_left = potential.size();
        //set respawn level
        uint32_t respawn_level = div_round_up(3 * score_to_level(ent.get_score()), 4);
        if (respawn_level > MAX_LEVEL) respawn_level = MAX_LEVEL;
        camera.set_respawn_level(respawn_level);
        uint32_t max_possible = MAX_SLOT_COUNT + loadout_slots_at_level(respawn_level);
        if (num_left > max_possible) num_left = max_possible;
        //fill petals
        for (uint32_t i = 0; i < 2 * MAX_SLOT_COUNT; ++i)
            camera.set_inventory(i, PetalID::kNone); //force reset
        for (uint32_t i = 0; i < num_left; ++i) {
            DEBUG_ONLY(assert(potential.back() < PetalID::kNumPetals));
            PetalTracker::add_petal(sim, potential.back());
            camera.set_inventory(i, potential.back());
            potential.pop_back();
        }
        //only track up to max_possible
        for (uint32_t i = num_left; i < max_possible; ++i)
            camera.set_inventory(i, PetalID::kNone); //don't track kNone
        //fill with basics
        for (uint32_t i = num_left; i < loadout_slots_at_level(respawn_level); ++i) {
            PetalTracker::add_petal(sim, PetalID::kBasic);
            camera.set_inventory(i, PetalID::kBasic);
        }
    } else if (ent.has_component(kDrop)) {
        if (BitMath::at(ent.flags, EntityFlags::kIsDespawning))
            PetalTracker::remove_petal(sim, ent.get_drop_id());
    }
}