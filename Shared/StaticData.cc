#include <Shared/StaticData.hh>

struct PetalData PETAL_DATA[PetalID::kNumPetals] = {
    {"None", 0.0, 0.0, 0.0, 1.0, 0, RarityID::kCommon, {}},
    {"Basic", 10.0, 10.0, 20.0, 2.5, 1, RarityID::kCommon, {}},
    {"Light", 5.0, 7.0, 14.0, 1.0, 1, RarityID::kCommon, {}},
    {"Heavy", 20.0, 20.0, 20.0, 4.5, 1, RarityID::kCommon, {}},
    {"Stinger", 5.0, 35.0, 14.0, 4.0, 1, RarityID::kUnusual, {}},
    {"Leaf", 8.0, 10.0, 20.0, 1.0, 1, RarityID::kUnusual, {
        .constant_heal = 1,
        .icon_angle = -1
    }},
    {"Twin", 5.0, 7.0, 14.0, 1.0, 2, RarityID::kUnusual, {}},
    {"Rose", 5.0, 5.0, 20.0, 3.5, 1, RarityID::kUnusual, { 
        .secondary_reload = 1.5,
        .burst_heal = 10,
        .defend_only = 1
    }},
    {"Iris", 5.0, 5.0, 14.0, 5.0, 1, RarityID::kUnusual, { 
        .poison_damage = { 10.0, 6.0 }
    }},
    {"Missile", 5.0, 25.0, 40.0, 1.5, 1, RarityID::kRare, {
        .secondary_reload = 0.5, 
        .defend_only = 1,
        .icon_angle = 1,
        .rotation_style = PetalAttributes::kFollowRot 
    }},
    {"Dandelion", 5.0, 5.0, 20.0, 1.0, 2, RarityID::kRare, {
        .icon_angle = 1,
        .rotation_style = PetalAttributes::kFollowRot 
    }},
    {"Triplet", 5.0, 7.0, 14.0, 1.0, 3, RarityID::kEpic, {}},
    {"Egg", 15.0, 1.0, 25.0, 1.0, 2, RarityID::kEpic, { 
        .secondary_reload = 4.5,
        .defend_only = 1,
        .rotation_style = PetalAttributes::kNoRot 
    }},
    {"Egg", 15.0, 1.0, 25.0, 1.0, 1, RarityID::kLegendary, { 
        .secondary_reload = 4.5,
        .defend_only = 1,
        .rotation_style = PetalAttributes::kNoRot 
    }},
    {"Stinger", 5.0, 35.0, 14.0, 5.0, 3, RarityID::kLegendary, {
        .clump_radius = 10
    }},
    {"Shield", 50000.0, 1.0, 50.0, 5.0, 1, RarityID::kMythic, {
        .defend_only = 1,
        .rotation_style = PetalAttributes::kNoRot
    }}
};

struct MobData MOB_DATA[MobID::kNumMobs] = {
    {"Baby Ant", RarityID::kCommon, {15.0}, 10.0, {14.0}, 1.0, {
        {PetalID::kShield, 0.99},{PetalID::kTwin, 0.12}, {PetalID::kLeaf, 0.09}, {PetalID::kTringer, 1}
    }, {}},
    {"Worker Ant", RarityID::kCommon, {20.0}, 10.0, {14.0}, 1.5, {
        {PetalID::kLight, 0.36},{PetalID::kTwin, 0.12}, {PetalID::kLeaf, 0.09}, {PetalID::kMissile, 1}
    }, {}},
    {"Soldier Ant", RarityID::kCommon, {40.0}, 10.0, {14.0}, 3.0, {
        {PetalID::kLight, 0.36},{PetalID::kTwin, 0.12}, {PetalID::kLeaf, 0.09}
    }, {}},
    {"Bee", RarityID::kCommon, {20.0}, 35.0, {20.0}, 2.0, {
        {PetalID::kStinger, 1}, {PetalID::kRose, 1}
    }, {}},
    {"Ladybug", RarityID::kCommon, {30.0}, 10.0, {30.0}, 1.5, {
        {PetalID::kLight, 0.36},{PetalID::kTwin, 0.12}, {PetalID::kLeaf, 0.09}
    }, {}},
    {"Beetle", RarityID::kRare, {40.0}, 20.0, {35.0}, 1.5, {
        {PetalID::kBeetleEgg, 0.36},{PetalID::kTwin, 0.12}, {PetalID::kLeaf, 0.09}
    }, {}},
    {"Massive Ladybug", RarityID::kCommon, {1000.0}, 10.0, {90.0}, 1.5, {
        {PetalID::kLight, 0.36},{PetalID::kTwin, 0.12}, {PetalID::kLeaf, 0.09}
    }, {}},
    {"Massive Beetle", RarityID::kRare, {600.0}, 20.0, {75.0}, 1.5, {
        {PetalID::kBeetleEgg, 0.36},{PetalID::kTwin, 0.12}, {PetalID::kLeaf, 0.09}
    }, {}},
    {"Hornet", RarityID::kRare, {40.0}, 40.0, {35.0}, 1.5, {
        {PetalID::kMissile, 0.99},{PetalID::kShield, 0.99}, {PetalID::kLeaf, 0.09}
    }},
    {"Cactus", RarityID::kRare, {25.0, 50.0}, 30.0, {30.0, 60.0}, 1.5, {
        {PetalID::kMissile, 0.99},{PetalID::kShield, 0.99}, {PetalID::kLeaf, 0.09}
    }, {}},
    {"Rock", RarityID::kRare, {10.0, 30.0}, 10.0, {10.0, 25.0}, 1.5, {
        {PetalID::kMissile, 0.99},{PetalID::kShield, 0.99}, {PetalID::kLeaf, 0.09}
    }, {}},
    {"Centipede", RarityID::kRare, {25.0}, 10.0, {35.0}, 1.5, {
        {PetalID::kMissile, 0.99},{PetalID::kShield, 0.99}, {PetalID::kLeaf, 0.09}
    }, { .segments = 10 }},
    {"Spider", RarityID::kRare, {30.0}, 5.0, {15.0}, 1.5, {
        {PetalID::kMissile, 0.99},{PetalID::kShield, 0.99}, {PetalID::kLeaf, 0.09}
    }},
};

uint32_t RARITY_COLORS[RarityID::kNumRarities] = { 
    0xff7eef6d, 0xffffe65d, 0xff4d52e3, 
    0xff861fde, 0xffde1f1f, 0xff1fdbde
};//, 0xffff2b75, 0xfff70fb6};

