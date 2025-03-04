#include <Server/Spawn.hh>

#include <Server/Server.hh>
#include <Shared/Helpers.hh>
#include <Shared/StaticData.hh>

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
    //player.add_component(kHealth);
    //player.set_health(100);
    //player.set_max_health(100);
    //player.damage = 25;
    //player.immunity_ticks = SERVER_TIME(2);
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
    //petal.mass = 0.2;
    petal.add_component(kRelations);
    petal.add_component(kPetal);
    petal.set_petal_id(petal_id);
    //petal.add_component(kHealth);
    //petal.set_max_health(PETAL_DATA[petal_id].health);
    //petal.set_health(PETAL_DATA[petal_id].health);
    //petal.damage = PETAL_DATA[petal_id].damage;
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
        camera.loadout[i].id = PetalID::kBasic;
    }
}