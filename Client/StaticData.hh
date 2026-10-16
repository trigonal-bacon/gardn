#pragma once

#include <Shared/StaticData.hh>

#include <array>
#include <string_view>

extern const std::array<uint32_t, RarityID::kNumRarities> RARITY_COLORS;
extern const std::array<uint32_t, ColorID::kNumColors> FLOWER_COLORS;
extern const std::array<std::string_view, RarityID::kNumRarities> RARITY_NAMES;
extern const std::array<const char, MAX_SLOT_COUNT> SLOT_KEYBINDS;
