#include <Client/StaticData.hh>

const std::array<uint32_t, RarityID::kNumRarities> RARITY_COLORS = {
    0xff7eef6d, 0xffffe65d, 0xff4d52e3, 0xff861fde,
    0xffde1f1f, 0xff1fdbde, 0xffde1f65}; // 0xffff2b75, 0xfff70fb6};

const std::array<uint32_t, ColorID::kNumColors> FLOWER_COLORS = {
    0xffffe763, 0xff999999, 0xff689ce2, 0xffec6869};

const std::array<std::string_view, RarityID::kNumRarities> RARITY_NAMES = {
    "Common", "Unusual", "Rare", "Epic", "Legendary", "Mythic", "Unique"};

const std::array<const char, MAX_SLOT_COUNT> SLOT_KEYBINDS = {
    '1', '2', '3', '4', '5', '6', '7', '8'};
