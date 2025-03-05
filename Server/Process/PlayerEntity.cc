#include <Server/Process/Process.hh>

#include <Server/Spawn.hh>
#include <Shared/Entity.hh>
#include <Shared/Simulation.hh>
#include <Shared/StaticData.hh>

static int count_petals(Entity &ent) {
    int count = 0;
    for (uint8_t i = 0; i < ent.loadout_count; ++i) {
        count += PETAL_DATA[ent.loadout[i].id].count;
    }
    return count;
}

void player_behavior(Simulation *sim, Entity &player) {
    if (player.pending_delete) return;
    if (!sim->ent_alive(player.parent)) {
        sim->request_delete(player.id);
        return;
    }
    Entity &camera = sim->get_ent(player.parent);
    float count = (float) count_petals(camera);
    assert(player.loadout_count < MAX_SLOT_COUNT);
    float rot_pos = 0;
    player.set_face_flags(0);
    for (uint8_t i = 0; i < camera.loadout_count; ++i) {
        LoadoutSlot &slot = camera.loadout[i];
        struct PetalData &petal_data = PETAL_DATA[slot.id];
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
                } else {
                    ++petal_slot.reload;
                }
            } else {
                Entity &petal = sim->get_ent(petal_slot.ent_id);
                if (petal.has_component(kPetal)) {
                    Vector wanting;
                    Vector delta(player.x - petal.x, player.y - petal.y);
                    wanting.unit_normal(2 * M_PI * rot_pos / count + player.petal_rotation);
                    if (BIT_AT(player.input, 0)) wanting *= 120;
                    else if (BIT_AT(player.input, 1)) wanting *= 40;
                    else wanting *= 60;
                    wanting += delta;
                    wanting *= 0.4;
                    petal.acceleration = wanting;
                } else {

                }
            }
            ++rot_pos;
        }
    };
    player.petal_rotation += 0.1;
    if (BIT_AT(player.input, 0)) player.set_face_flags(player.face_flags | 1);
    else if (BIT_AT(player.input, 1)) player.set_face_flags(player.face_flags | 2);
}

void tick_player_entities(Simulation *sim) {
    for (uint32_t i = 0; i < sim->active_entities.length; ++i) {
        Entity &ent = sim->get_ent(sim->active_entities[i]);
        if (ent.has_component(kFlower)) player_behavior(sim, ent);
        //else if (ent.has_component(kPetal) && ent.detached) tick_detached_petal_behavior(sim, ent);
        else if (ent.has_component(kPetal) && !sim->ent_alive(ent.parent)) sim->request_delete(ent.id);
    }
}