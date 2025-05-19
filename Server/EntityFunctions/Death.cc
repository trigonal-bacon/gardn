#include <Server/EntityFunctions.hh>

#include <Server/Spawn.hh>
#include <Shared/Entity.hh>
#include <Shared/Helpers.hh>
#include <Shared/Vector.hh>

#include <algorithm>

static void __alloc_drops(std::vector<PetalID::T> const &success_drops, float x, float y) {
    size_t count = success_drops.size();
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

void entity_on_death(Simulation *sim, Entity &ent) {
    //don't do on_death for any despawned entity
    uint8_t natural_despawn = (ent.flags & EntityFlags::IsDespawning) && ent.despawn_tick == 0;
    if (ent.has_component(kScore) && sim->ent_exists(ent.last_damaged_by) && !natural_despawn) {
        Entity *killer = &sim->get_ent(ent.last_damaged_by);
        //defer to parent
        if (sim->ent_alive(killer->owner) && sim->get_ent(killer->owner).has_component(kScore)) {
            killer = &sim->get_ent(killer->owner);
        }
        if (killer->has_component(kScore) && killer->has_component(kFlower)) {
            if (ent.has_component(kMob))
                killer->set_score(killer->score + ent.score);
            else
                killer->set_score(killer->score + ent.score / 2);
        }
    }
    if (ent.has_component(kMob) && !natural_despawn) {
        if (!(ent.team == NULL_ENTITY)) return;
        struct MobData const &mob_data = MOB_DATA[ent.mob_id];
        std::vector<struct MobDrop> const &drops = mob_data.drops;
        std::vector<PetalID::T> success_drops = {};
        for (MobDrop const &d : drops) 
            if (frand() < d.chance) success_drops.push_back(d.id);
        __alloc_drops(success_drops, ent.x, ent.y);
    } else if (ent.has_component(kPetal)) {
        if (ent.petal_id == PetalID::kWeb || ent.petal_id == PetalID::kTriweb) {
            alloc_web(100, ent);
        }
    } else if (ent.has_component(kFlower)) {
        std::vector<PetalID::T> potential = {};
        for (uint32_t i = 0; i < ent.loadout_count + MAX_SLOT_COUNT; ++i)
            if (ent.loadout_ids[i] != PetalID::kNone && 
                ent.loadout_ids[i] != PetalID::kBasic)
                potential.push_back(ent.loadout_ids[i]);
        std::sort(potential.begin(), potential.end(), 
        [](PetalID::T a, PetalID::T b) {
            return PETAL_DATA[a].rarity < PETAL_DATA[b].rarity;
        });
        //std::cout << "Player Drops\n";
        //for (auto x : potential)
            //std::cout << '{' << (int) x << ',' << (int) PETAL_DATA[x].rarity << '}';
        //std::cout << '\n';
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
        if (!sim->ent_alive(ent.parent))
            return;
        Entity &camera = sim->get_ent(ent.parent);
        //reset all reloads and stuff
        uint32_t numLeft = potential.size();
        //fill petals
        for (uint32_t i = 0; i < numLeft; ++i)
            camera.set_inventory(i, potential[numLeft - 1 - i]);
        for (uint32_t i = numLeft; i < 2 * MAX_SLOT_COUNT; ++i)
            camera.set_inventory(i, PetalID::kNone);
        //fill with basics
        for (uint32_t i = numLeft; i < ent.loadout_count; ++i)
            camera.set_inventory(i, PetalID::kBasic);
        //set respawn level
        camera.set_respawn_level(scoreToLevel(ent.score) / 2);
    }
}