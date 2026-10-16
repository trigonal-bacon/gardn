#include <Server/Process.hh>

#include <Shared/Entity.hh>
#include <Shared/Simulation.hh>

void tick_score_behavior(Simulation* sim, Entity& ent) {
  ent.score_reward = ent.get_score() / 2;
}