#include <Server/Spawn.hh>

#include <Server/Server.hh>
#include <Shared/Helpers.hh>
#include <Shared/StaticData.hh>

#include <cmath>

Entity &alloc_drop(uint8_t drop_id) {
    Entity &drop = game_server->simulation.alloc_ent();
    drop.add_component(kPhysics);
    drop.set_radius(1);
    drop.set_angle(0);
    drop.friction = 0.25;
    drop.add_component(kRelations);
    drop.set_team(NULL_ENTITY);
    drop.set_parent(NULL_ENTITY);
    drop.add_component(kDrop);
    drop.set_drop_id(drop_id);
    drop.despawn_tick = 0;
    return drop;
}

static Entity &__alloc_mob(uint8_t mob_id) {
    struct MobData const &data = MOB_DATA[mob_id];
    float seed = frand();
    Entity &mob = game_server->simulation.alloc_ent();
    mob.add_component(kPhysics);
    mob.set_radius(data.radius.get_single(seed));
    mob.set_angle(frand() * 2 * M_PI);
    mob.set_x(frand() * ARENA_WIDTH);
    mob.set_y(frand() * ARENA_HEIGHT);
    mob.friction = DEFAULT_FRICTION;
    mob.mass = 1 + (mob.radius * mob.radius / 100);
    mob.add_component(kHealth);
    mob.add_component(kRelations);
    mob.set_team(NULL_ENTITY);
    mob.set_parent(NULL_ENTITY);
    mob.add_component(kMob);
    mob.set_mob_id(mob_id);
    mob.health = mob.max_health = data.health.get_single(seed);
    mob.damage = data.damage;
    mob.set_health_ratio(1);
    if (data.attributes.segments > 0) mob.add_component(kSegmented);
    return mob;
}

Entity &alloc_mob(uint8_t mob_id) {
    struct MobData const &data = MOB_DATA[mob_id];
    if (data.attributes.segments == 0) return __alloc_mob(mob_id);
    else {
        Entity &head = __alloc_mob(mob_id);
        head.set_is_tail(0);
        Entity *curr = &head;
        for (uint32_t i = 1; i < data.attributes.segments; ++i) {
            Entity &seg = __alloc_mob(mob_id);
            seg.set_is_tail(1);
            seg.seg_head = curr->id;
            seg.set_angle(curr->angle + frand() * 0.1 - 0.05);
            seg.set_x(curr->x - (curr->radius + seg.radius) * cosf(seg.angle));
            seg.set_y(curr->y - (curr->radius + seg.radius) * sinf(seg.angle));
            curr = &seg;
        }
        return head;
    }
}

Entity &alloc_player(Entity &camera) {
    Entity &player = game_server->simulation.alloc_ent();
    player.add_component(kPhysics);
    player.set_x(camera.camera_x);
    player.set_y(camera.camera_y);
    player.set_radius(25);
    player.friction = DEFAULT_FRICTION;
    player.add_component(kFlower);
    camera.set_player(player.id);
    player.add_component(kRelations);
    player.set_parent(camera.id);
    player.set_team(camera.id);
    player.add_component(kHealth);
    player.health = 100;
    player.max_health = 100;
    player.set_health_ratio(1);
    player.damage = 25;
    player.immunity_ticks = 1 * TPS;
    //player.add_component(kScore);
    //player.set_score(camera.experience / 2);
    //player.no_friendly_collision = 1;
    return player;
}

Entity &alloc_petal(uint8_t petal_id, Entity &parent) {
    struct PetalData const &petal_data = PETAL_DATA[petal_id];
    Entity &petal = game_server->simulation.alloc_ent();
    petal.add_component(kPhysics);
    petal.set_x(parent.x);
    petal.set_y(parent.y);
    petal.set_radius(petal_data.radius);
    petal.mass = 0.05;
    if (petal_id == PetalID::kShield) petal.mass = 10;
    petal.friction = 0.5;
    petal.add_component(kRelations);
    petal.set_parent(parent.id);
    petal.set_team(parent.team);
    petal.add_component(kPetal);
    petal.set_petal_id(petal_id);
    petal.add_component(kHealth);
    petal.health = petal.max_health = petal_data.health;
    petal.damage = petal_data.damage;
    petal.set_health_ratio(1);
    petal.poison_damage = petal_data.attributes.poison_damage;
    //petal.effect_delay = 0;
    //petal.poison.define(REAL_TIME(PETAL_DATA[petal_id].extras.poison_damage / PETAL_DATA[petal_id].extras.poison_time), SERVER_TIME(PETAL_DATA[petal_id].extras.poison_time));
    //petal.no_friendly_collision = 1;
    return petal;
}

void player_spawn(Simulation *sim, Entity &camera, Entity &player) {
    float spawn_x = frand() * ARENA_WIDTH;
    float spawn_y = frand() * ARENA_HEIGHT;
    camera.set_camera_x(spawn_x);
    camera.set_camera_y(spawn_y);
    player.set_x(spawn_x);
    player.set_y(spawn_y);
    camera.set_loadout_count(5);
    for (uint32_t i = 0; i < camera.loadout_count; ++i) {
        camera.loadout[i].reset();
        if (i < 4) camera.loadout[i].id = PetalID::kDandelion;
        else camera.loadout[i].id = PetalID::kRose;
    }
    for (uint32_t i = camera.loadout_count; i < MAX_SLOT_COUNT + camera.loadout_count; ++i) {
        camera.set_loadout_ids(i, PetalID::kNone);
    }
}