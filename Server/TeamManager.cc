#include <Server/TeamManager.hh>

#include <Shared/Simulation.hh>

TeamManager::TeamManager(Simulation *sim) : simulation(sim) {}

void TeamManager::add_team(uint8_t color) {
    Entity &team_ent = simulation->alloc_ent();
    team_ent.add_component(kRelations);
    team_ent.set_color(color);
    team_ent.set_team(team_ent.id);
    team_ent.set_parent(team_ent.id);
    teams.push(team_ent.id);
}

EntityID const TeamManager::get_random_team() const {
    assert(teams.size() > 0);
    if (teams.size() == 0) return NULL_ENTITY;
    return teams[frand() * teams.size()];
}