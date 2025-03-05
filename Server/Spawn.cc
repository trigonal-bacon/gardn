#include <Server/Spawn.hh>

#include <Server/Server.hh>
#include <Shared/Helpers.hh>
#include <Shared/StaticData.hh>

Entity &alloc_drop(uint8_t drop_id) {
    Entity &drop = game_server->simulation.alloc_ent();
    drop.add_component(kPhysics);
    drop.set_radius(3);
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

Entity &alloc_mob(uint8_t mob_id) {
    Entity &mob = game_server->simulation.alloc_ent();
    mob.add_component(kPhysics);
    mob.set_radius(MOB_DATA[mob_id].radius);
    mob.set_angle(frand() * 2 * M_PI);
    mob.set_x(frand() * ARENA_WIDTH);
    mob.set_y(frand() * ARENA_HEIGHT);
    mob.friction = DEFAULT_FRICTION;
    mob.add_component(kHealth);
    mob.add_component(kRelations);
    mob.set_team(NULL_ENTITY);
    mob.set_parent(NULL_ENTITY);
    mob.add_component(kMob);
    mob.set_mob_id(mob_id);
    mob.health = MOB_DATA[mob_id].health;
    mob.max_health = MOB_DATA[mob_id].health;
    mob.damage = MOB_DATA[mob_id].damage;
    mob.set_health_ratio(1);
    return mob;
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
    Entity &petal = game_server->simulation.alloc_ent();
    petal.add_component(kPhysics);
    petal.set_radius(10);
    petal.friction = DEFAULT_FRICTION;
    petal.mass = 0.2;
    petal.friction = 0.5;
    petal.add_component(kRelations);
    petal.add_component(kPetal);
    petal.set_petal_id(petal_id);
    petal.add_component(kHealth);
    petal.health = PETAL_DATA[petal_id].health;
    petal.max_health = PETAL_DATA[petal_id].health;
    petal.damage = PETAL_DATA[petal_id].damage;
    petal.set_health_ratio(1);
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
        if (i < 2) camera.loadout[i].id = PetalID::kBasic;
    }
}