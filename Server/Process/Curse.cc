#include <Server/Process.hh>

#include <Shared/Simulation.hh>

#include <Shared/StaticData.hh>

void tick_curse_behavior(Simulation *sim) {
    EntityID leader = NULL_ENTITY;
    uint32_t max_score = level_to_score(60);
    sim->for_each<kCamera>([&](Simulation *sim, Entity &ent) {
        if (!sim->ent_alive(ent.player)) return;
        if (sim->get_ent(ent.player).score < max_score) return;
        max_score = sim->get_ent(ent.player).score;
        leader = ent.player;
    });
    if (!sim->ent_alive(leader)) return;
    sim->for_each<kMob>([=](Simulation *sim, Entity &ent) {
        if (sim->ent_alive(ent.parent)) return;
        if (sim->ent_alive(ent.target)) return;
        Entity const &player = sim->get_ent(leader);
        Vector delta(player.x - ent.x, player.y - ent.y);
        if (delta.magnitude() > 1000) return;
        ent.target = leader;
    });
}