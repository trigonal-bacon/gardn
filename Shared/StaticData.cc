#include <Shared/StaticData.hh>

struct PetalData PETAL_DATA[PetalID::kNumPetals] = {
    {"None", 0.0, 0.0, 0.0, 1.0, 0, RarityID::kCommon, {}},
    {"Basic", 10.0, 10.0, 20.0, 2.5, 1, RarityID::kCommon, {}},
    {"Light", 5.0, 7.0, 14.0, 1.0, 1, RarityID::kCommon, {}},
    {"Twin", 5.0, 7.0, 14.0, 1.0, 2, RarityID::kUnusual, {}},
    {"Triplet", 5.0, 7.0, 14.0, 1.0, 3, RarityID::kEpic, {}}
};

struct MobData MOB_DATA[MobID::kNumMobs] = {
    {"Baby Ant", RarityID::kCommon, 15.0, 10.0, 14.0, 1.0, {}},
    {"Worker Ant", RarityID::kCommon, 20.0, 10.0, 14.0, 1.5, {}}
};

uint32_t RARITY_COLORS[RarityID::kNumRarities] = { 
    0xff7eef6d, 0xffffe65d, 0xff4d52e3, 
    0xff861fde, 0xffde1f1f, 0xff1fdbde
};//, 0xffff2b75, 0xfff70fb6};
