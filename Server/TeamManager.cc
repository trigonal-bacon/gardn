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
    uint32_t min = simulation->get_ent(teams[0]).player_count;
    uint32_t min_index = 0;
    for (uint32_t i = 1; i < teams.size(); ++i) {
        uint32_t player_count = simulation->get_ent(teams[i]).player_count;
        if (player_count < min || player_count == min && frand() > 0.5) {
            min = player_count;
            min_index = i;
        }
    } 
    return teams[min_index];
}