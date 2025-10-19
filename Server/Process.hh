#pragma once

class Simulation;
class Entity;

void tick_ai_behavior(Simulation *, Entity &);
void tick_camera_behavior(Simulation *, Entity &);
void tick_chat_behavior(Simulation *, Entity &);
void tick_curse_behavior(Simulation *);
void tick_culling_behavior(Simulation *, Entity &);
void tick_drop_behavior(Simulation *, Entity &);
void tick_entity_motion(Simulation *, Entity &);
void tick_health_behavior(Simulation *, Entity &);
void tick_petal_behavior(Simulation *, Entity &);
void tick_player_behavior(Simulation *, Entity &);
void tick_player_ai_behavior(Simulation *, Entity &);
void tick_segment_behavior(Simulation *, Entity &);
void tick_score_behavior(Simulation *, Entity &);
void on_collide(Simulation *, Entity &, Entity &);
