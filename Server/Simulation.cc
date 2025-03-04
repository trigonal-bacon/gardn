#include <Shared/Simulation.hh>

#include <Server/Process/Process.hh>
#include <Server/Client.hh>
#include <Server/Server.hh>
#include <Server/SpatialHash.hh>

#include <iostream>

static void update_client(Simulation *sim, Client *client) {
    if (!sim->ent_exists(client->camera)) return;
    std::set<EntityId> in_view;
    in_view.insert(client->camera);
    Entity &camera = sim->get_ent(client->camera);
    if (sim->ent_exists(camera.player)) {
        Entity &player = sim->get_ent(camera.player);
        camera.set_camera_x(player.x);
        camera.set_camera_y(player.y);
        in_view.insert(player.id);
    }
    Writer writer(OUTGOING_PACKET);
    writer.write_uint8(kClientbound::kClientUpdate);
    writer.write_entid(client->camera);
    sim->spatial_hash.query(camera.camera_x, camera.camera_y, 960 / camera.fov, 540 / camera.fov, [&](Simulation *, Entity &ent){
        in_view.insert(ent.id);
    });

    for (EntityId i: client->last_in_view) {
        if (!in_view.contains(i)) writer.write_entid(i);
    }
    writer.write_entid(NULL_ENTITY);
    //upcreates
    for (EntityId id: in_view) {
        assert(sim->ent_exists(id));
        Entity &ent = sim->get_ent(id);
        uint8_t create = !client->last_in_view.contains(id);
        writer.write_entid(id);
        writer.write_uint8(create);
        ent.write(&writer, create);
    }
    writer.write_entid(NULL_ENTITY);
    //set client->last_in_view
    client->last_in_view.clear();
    for (EntityId i: in_view) client->last_in_view.insert(i);

    std::string_view message(reinterpret_cast<char const *>(writer.packet), writer.at - writer.packet);
    client->ws->send(message, uWS::OpCode::BINARY, 0);
}

void Simulation::tick() {
    pre_tick();
    for (uint32_t i = 0; i < active_entities.length; ++i) {
        Entity &ent = get_ent(active_entities[i]);
        //ent.set_damaged(0); //very ugly but whatever, will make component vectors later
        if (ent.has_component(kPhysics)) {
            if (ent.deletion_tick > 0) request_delete(ent.id);
            else spatial_hash.insert(ent);
        }
    }

    tick_player_entities(this);
    for (uint32_t i = 0; i < active_entities.length; ++i) {
        Entity &ent = get_ent(active_entities[i]);
        //ent.set_damaged(0); //very ugly but whatever, will make component vectors later
        if (ent.has_component(kPhysics)) tick_entity_motion(this, ent);
    }
    post_tick();
}

void Simulation::post_tick() {
    for (Client *client: game_server->clients) update_client(this, client);
    //send_state & reset all remaining active entities
    //reset state of all entities FIRST
    for (uint32_t i = 0; i < active_entities.length; ++i) {
        assert(ent_exists(active_entities[i])); //no deletions mid tick
        Entity &ent = get_ent(active_entities[i]);
        ent.reset_protocol();
    }
    for (uint32_t i = 0; i < pending_delete.length; ++i) {
        //guarantee entity exists
        assert(ent_exists(pending_delete[i]));
        Entity &ent = get_ent(pending_delete[i]);
        if (!ent.has_component(kPhysics)) delete_ent(pending_delete[i]);
        else {
            if (ent.deletion_tick >= 0) delete_ent(pending_delete[i]);
            else ent.set_deletion_tick(ent.deletion_tick + 1);
        }
    }
    pending_delete.clear();
    active_entities.clear();
    spatial_hash.clear();
}
