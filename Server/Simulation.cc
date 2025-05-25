#include <Shared/Simulation.hh>

#include <Server/Process.hh>
#include <Server/Client.hh>
#include <Server/EntityFunctions.hh>
#include <Server/Server.hh>
#include <Server/Spawn.hh>
#include <Server/SpatialHash.hh>

static void update_client(Simulation *sim, Client *client) {
    if (!sim->ent_exists(client->camera)) return;
    std::set<EntityID> in_view;
    in_view.insert(client->camera);
    Entity &camera = sim->get_ent(client->camera);
    if (sim->ent_exists(camera.player)) 
        in_view.insert(camera.player);
    Writer writer(Server::OUTGOING_PACKET);
    writer.write_uint8(kClientbound::kClientUpdate);
    writer.write_entid(client->camera);
    sim->spatial_hash.query(camera.camera_x, camera.camera_y, 960 / camera.fov, 540 / camera.fov, [&](Simulation *, Entity &ent){
        in_view.insert(ent.id);
    });

    for (EntityID i: client->last_in_view) {
        if (!in_view.contains(i)) writer.write_entid(i);
    }
    writer.write_entid(NULL_ENTITY);
    //upcreates
    for (EntityID id: in_view) {
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
    for (EntityID i: in_view) client->last_in_view.insert(i);

    std::string_view message(reinterpret_cast<char const *>(writer.packet), writer.at - writer.packet);
    client->ws->send(message, uWS::OpCode::BINARY, 0);
}

void Simulation::tick() {
    pre_tick();
    if (frand() < 0.004) alloc_mob(MobID::kCentipede, frand() * ARENA_WIDTH, frand() * ARENA_HEIGHT, NULL_ENTITY);
    for (uint32_t i = 0; i < active_entities.length; ++i) {
        Entity &ent = get_ent(active_entities[i]);
        if (ent.has_component(kPhysics)) {
            if (ent.deletion_tick > 0)
                request_delete(ent.id);
            spatial_hash.insert(ent);
        }
    }

    for_each<kWeb>(tick_web_behavior);
    for_each<kFlower>(tick_player_behavior);
    for_each<kMob>(tick_ai_behavior);
    for_each<kPetal>(tick_petal_behavior);
    for_each<kHealth>(tick_health_behavior);
    spatial_hash.collide(on_collide);
    for_each<kPhysics>(tick_entity_motion);
    for_each<kSegmented>(tick_segment_behavior);
    for_each<kDrop>(tick_drop_behavior);
    for_each<kCamera>(tick_camera_behavior);
    post_tick();
}

void Simulation::post_tick() {
    for (Client *client: Server::clients) update_client(this, client);
    //send_state & reset all remaining active entities
    //reset state of all entities FIRST
    for (uint32_t i = 0; i < active_entities.length; ++i) {
        assert(ent_exists(active_entities[i])); //no deletions mid tick
        Entity &ent = get_ent(active_entities[i]);
        ent.reset_protocol();
        ++ent.lifetime;
        if (ent.flags & EntityFlags::IsDespawning) {
            if (ent.despawn_tick == 0)
                request_delete(ent.id);
            else
                --ent.despawn_tick;
        }
    }
    for (uint32_t i = 0; i < pending_delete.length; ++i) {
        //guarantee entity exists
        assert(ent_exists(pending_delete[i]));
        Entity &ent = get_ent(pending_delete[i]);
        if (!ent.has_component(kPhysics)) 
            delete_ent(pending_delete[i]);
        else {
            if (ent.deletion_tick >= 4)
                delete_ent(pending_delete[i]);
            else {
                if (ent.deletion_tick == 0)
                    entity_on_death(this, ent);
                ent.set_deletion_tick(ent.deletion_tick + 1);
            }
            ent.pending_delete = 0;
        }
    }
    pending_delete.clear();
    active_entities.clear();
    spatial_hash.clear();
}
