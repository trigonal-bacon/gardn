#pragma once

#include <Shared/StaticData.hh>

#include <array>

extern std::array<uint32_t, RarityID::kNumRarities> const RARITY_COLORS;
extern std::array<uint32_t, ColorID::kNumColors> const FLOWER_COLORS;
extern std::array<char const *, RarityID::kNumRarities> const RARITY_NAMES;
extern std::array<char const, MAX_SLOT_COUNT> const SLOT_KEYBINDS;
