// File: Server/Bot.hh
#pragma once

#include <Shared/Entity.hh>
#include <Shared/Simulation.hh>

// Public bot interfaces (implemented in Server/Process/Bot.cc and BotNames.cc)
// You will provide the definitions; these are referenced by the server.

// Ensure the bot player has a persistent name for this session (e.g., pick from a list).
void ensure_bot_name(Simulation *sim, Entity &player);

// Per-tick bot behavior for a player entity flagged as a bot (player.is_bot == 1).
void tick_bot_player_behavior(Simulation *sim, Entity &player);
