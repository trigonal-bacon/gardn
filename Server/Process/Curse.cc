#include <Server/Process.hh>

#include <Shared/Simulation.hh>

#include <Shared/StaticData.hh>

void tick_curse_behavior(Simulation *sim) {
    EntityID leader = NULL_ENTITY;
    uint32_t max_score = level_to_score(60);
    sim->for_each<kCamera>([&](Simulation *sim, Entity &ent) {
        if (!sim->ent_alive(ent.get_player())) return;
        if (sim->get_ent(ent.get_player()).get_score() < max_score) return;
        max_score = sim->get_ent(ent.get_player()).get_score();
        leader = ent.get_player();
    });
    if (!sim->ent_alive(leader)) return;
    sim->for_each<kMob>([=](Simulation *sim, Entity &ent) {
        if (!(ent.get_team() == NULL_ENTITY)) return;
        if (sim->ent_alive(ent.target)) return;
        Entity const &player = sim->get_ent(leader);
        Vector delta(player.get_x() - ent.get_x(), player.get_y() - ent.get_y());
        if (delta.magnitude() > ent.detection_radius * 2) return;
        ent.target = leader;
    });
}