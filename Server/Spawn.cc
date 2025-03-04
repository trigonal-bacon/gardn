#include <Server/Spawn.hh>

#include <Server/Server.hh>
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