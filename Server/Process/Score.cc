#include <Server/Process.hh>

#include <Shared/Simulation.hh>
#include <Shared/Entity.hh>

void tick_score_behavior(Simulation *sim, Entity &ent) {
    ent.score_reward = ent.get_score() / 2;
}