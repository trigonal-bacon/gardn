#include <Shared/StaticData.hh>

#include <cmath>

struct PetalData PETAL_DATA[PetalID::kNumPetals] = {
    {"None", 0.0, 0.0, 0.0, 1.0, 0, RarityID::kCommon, {}},
    {"Basic", 10.0, 10.0, 10.0, 2.5, 1, RarityID::kCommon, {}},
    {"Light", 5.0, 7.0, 7.0, 1.0, 1, RarityID::kCommon, {}},
    {"Heavy", 20.0, 20.0, 10.0, 4.5, 1, RarityID::kCommon, {}},
    {"Stinger", 5.0, 35.0, 7.0, 4.0, 1, RarityID::kUnusual, {}},
    {"Leaf", 8.0, 10.0, 10.0, 1.0, 1, RarityID::kUnusual, {
        .constant_heal = 1,
        .icon_angle = -1
    }},
    {"Twin", 5.0, 7.0, 7.0, 1.0, 2, RarityID::kUnusual, {}},
    {"Rose", 5.0, 5.0, 10.0, 3.5, 1, RarityID::kUnusual, { 
        .secondary_reload = 1.5,
        .burst_heal = 10,
        .defend_only = 1
    }},
    {"Iris", 5.0, 5.0, 7.0, 5.0, 1, RarityID::kUnusual, { 
        .poison_damage = { 10.0, 6.0 }
    }},
    {"Missile", 5.0, 25.0, 10.0, 1.0, 1, RarityID::kRare, {
        .secondary_reload = 0.5, 
        .defend_only = 1,
        .icon_angle = 1,
        .rotation_style = PetalAttributes::kFollowRot 
    }},
    {"Dandelion", 10.0, 10.0, 10.0, 1.0, 1, RarityID::kRare, {
        .icon_angle = 1,
        .rotation_style = PetalAttributes::kFollowRot 
    }},
    {"Bubble", 1.0, 0.0, 12.0, 2.0, 1, RarityID::kRare, {
        .secondary_reload = 0.5,
        .defend_only = 1,
    }},
    {"Faster", 5.0, 7.0, 7.0, 0.5, 1, RarityID::kRare, {}},
    {"Rock", 100.0, 10.0, 10.0, 4.0, 1, RarityID::kRare, {}},
    {"Cactus", 15.0, 5.0, 10.0, 1.0, 1, RarityID::kRare, {}},
    {"Web", 10.0, 5.0, 10.0, 3.0, 1, RarityID::kRare, {
        .secondary_reload = 0.5,
        .defend_only = 1,
    }},
    {"Wing", 15.0, 20.0, 10.0, 2.5, 1, RarityID::kRare, {
        .icon_angle = 1,
    }},
    {"Peas", 5.0, 8.0, 7.0, 2.0, 4, RarityID::kRare, {
        .clump_radius = 8,
        .secondary_reload = 1.0,
        .defend_only = 1
    }},
    {"Sand", 10.0, 3.0, 7.0, 1.5, 4, RarityID::kRare, {
        .clump_radius = 10,
    }},
    {"Pincer", 10.0, 5.0, 10.0, 2.5, 1, RarityID::kRare, {
        .icon_angle = 0.7,
        .poison_damage = { 5.0, 1.0 }
    }},
    {"Dahlia", 5.0, 5.0, 7.0, 3.5, 3, RarityID::kRare, { 
        .clump_radius = 10,
        .secondary_reload = 1.5,
        .burst_heal = 3.5,
        .defend_only = 1
    }},
    {"Triplet", 5.0, 7.0, 7.0, 1.0, 3, RarityID::kEpic, {}},
    {"Egg", 15.0, 1.0, 12.5, 1.0, 2, RarityID::kEpic, { 
        .secondary_reload = 3.5,
        .defend_only = 1,
        .rotation_style = PetalAttributes::kNoRot,
        .spawns = MobID::kSoldierAnt
    }},
    {"Iris", 10.0, 5.0, 7.0, 5.0, 1, RarityID::kEpic, { 
        .poison_damage = { 15.0, 4.0 }
    }},
    {"Pollen", 7.0, 8.0, 7.0, 1.5, 3, RarityID::kEpic, {
        .secondary_reload = 0.5,
        .defend_only = 1
    }},
    {"Peas", 5.0, 2.0, 7.0, 2.0, 4, RarityID::kEpic, {
        .clump_radius = 8,
        .secondary_reload = 0.5,
        .defend_only = 1,
        .poison_damage = { 20.0, 0.5 }
    }},
    {"Egg", 15.0, 1.0, 15.0, 1.0, 1, RarityID::kEpic, { 
        .secondary_reload = 3.5,
        .defend_only = 1,
        .rotation_style = PetalAttributes::kNoRot,
        .spawns = MobID::kBeetle
    }},
    {"Azalea", 5.0, 5.0, 10.0, 3.5, 1, RarityID::kEpic, { 
        .secondary_reload = 1.5,
        .burst_heal = 22,
        .defend_only = 1
    }},
    {"Stick", 10.0, 1.0, 15.0, 3.0, 1, RarityID::kLegendary, { 
        .secondary_reload = 4.0,
        .defend_only = 1,
        .icon_angle = 1,
        .spawns = MobID::kSandstorm,
        .spawn_count = 2
    }},
    {"Stinger", 5.0, 35.0, 7.0, 5.0, 3, RarityID::kLegendary, {
        .clump_radius = 10
    }},
    {"Web", 10.0, 5.0, 10.0, 3.0, 3, RarityID::kLegendary, {
        .clump_radius = 10,
        .secondary_reload = 0.5,
        .defend_only = 1,
    }},
    {"Antennae", 0.0, 0.0, 12.5, 0.0, 0, RarityID::kLegendary, {}},
    {"Cactus", 15.0, 5.0, 10.0, 1.0, 3, RarityID::kLegendary, {
        .clump_radius = 10,
    }},
    {"Shield", 1000.0, 1.0, 25.0, 5.0, 1, RarityID::kMythic, {
        .rotation_style = PetalAttributes::kNoRot
    }},
    {"Third Eye", 0.0, 0.0, 20.0, 0.0, 0, RarityID::kMythic, {}},
    {"Observer", 0.0, 0.0, 12.5, 0.0, 0, RarityID::kMythic, {}}
};

struct MobData MOB_DATA[MobID::kNumMobs] = {
    {"Baby Ant", RarityID::kCommon, {10.0}, 10.0, {14.0}, 1.0, {
        {PetalID::kLight, 0.36},
        {PetalID::kTwin, 0.12},
        {PetalID::kLeaf, 0.09},
        {PetalID::kTriplet, 0.005}
    }, {}},
    {"Worker Ant", RarityID::kCommon, {20.0}, 10.0, {14.0}, 3.0, {
        {PetalID::kLight, 0.36},
        {PetalID::kTwin, 0.12},
        {PetalID::kLeaf, 0.09}
    }, {}},
    {"Soldier Ant", RarityID::kCommon, {40.0}, 10.0, {14.0}, 5.0, {
        {PetalID::kLight, 0.36},
        {PetalID::kTwin, 0.12},
        {PetalID::kWing, 0.03}
    }, {}},
    {"Bee", RarityID::kCommon, {20.0}, 35.0, {20.0}, 4.0, {
        {PetalID::kLight, 0.36},
        {PetalID::kStinger, 0.12},
        {PetalID::kWing, 0.03},
        {PetalID::kBubble, 0.01}
    }, {}},
    {"Ladybug", RarityID::kCommon, {25.0}, 10.0, {30.0}, 3.0, {
        {PetalID::kLight, 0.36},
        {PetalID::kTwin, 0.12},
        {PetalID::kWing, 0.03},
        {PetalID::kBubble, 0.01}
    }, {}},
    {"Beetle", RarityID::kRare, {50.0}, 20.0, {35.0}, 10.0, {
        {PetalID::kTwin, 0.12},
        {PetalID::kIris, 0.09},
        {PetalID::kWing, 0.03}
    }, {}},
    {"Massive Ladybug", RarityID::kCommon, {1000.0}, 15.0, {90.0}, 400.0, {
        {PetalID::kRose, 1},
        {PetalID::kDahlia, 1},
        {PetalID::kBubble, 1},
        {PetalID::kAzalea, 1},
        {PetalID::kObserver, 0.0006}
    }, {}},
    {"Massive Beetle", RarityID::kRare, {600.0}, 40.0, {75.0}, 50.0, {
        {PetalID::kIris, 1},
        {PetalID::kTriplet, 0.06},
        {PetalID::kBeetleEgg, 0.06},
        {PetalID::kThirdEye, 0.0006}
    }, { .aggro_radius = 1000 }},
    {"Dark Ladybug", RarityID::kCommon, {35.0}, 10.0, {30.0}, 5.0, {
        {PetalID::kWing, 0.12},
        {PetalID::kDahlia, 0.36},
        {PetalID::kAzalea, 0.02}
    }, {}},
    {"Hornet", RarityID::kRare, {50.0}, 40.0, {35.0}, 12.0, {
        {PetalID::kMissile, 0.18},
        {PetalID::kWing, 0.09},
        {PetalID::kDandelion, 0.12},
        {PetalID::kAntennae, 0.003}
    }, { .aggro_radius = 750 }},
    {"Cactus", RarityID::kRare, {25.0, 50.0}, 30.0, {30.0, 60.0}, 2.0, {
        {PetalID::kStinger, 0.12},
        {PetalID::kCactus, 0.06}
    }, { .stationary = 1 }},
    {"Rock", RarityID::kRare, {10.0, 30.0}, 10.0, {10.0, 25.0}, 1.0, {
        {PetalID::kHeavy, 0.24},
        {PetalID::kRock, 0.01}
    }, { .stationary = 1 }},
    {"Centipede", RarityID::kRare, {25.0}, 10.0, {35.0}, 2.0, {
        {PetalID::kLight, 0.12},
        {PetalID::kTwin, 0.03},
        {PetalID::kLeaf, 0.03}
    }, { .segments = 10 }},
    {"Evil Centipede", RarityID::kRare, {35.0}, 10.0, {35.0}, 3.0, {
        {PetalID::kIris, 0.06},
        {PetalID::kTwin, 0.03},
        {PetalID::kBlueIris, 0.006}
    }, { .segments = 10, .poison_damage = { 5.0, 2.0 } }},
    {"Desert Centipede", RarityID::kRare, {35.0}, 10.0, {35.0}, 4.0, {
        {PetalID::kTwin, 0.03},
        {PetalID::kLeaf, 0.03},
        {PetalID::kFaster, 0.01}
    }, { .segments = 6 }},
    {"Sandstorm", RarityID::kRare, {30.0,45.0}, 40.0, {32.0,48.0}, 5.0, {
        {PetalID::kTwin, 0.03},
        {PetalID::kSand, 0.03},
        {PetalID::kFaster, 0.01},
        {PetalID::kStick, 0.001}
    }, { .stationary = 1 }},
    {"Scorpion", RarityID::kRare, {35.0}, 15.0, {35.0}, 10.0, {
        {PetalID::kIris, 0.24},
        {PetalID::kStinger, 0.12},
        {PetalID::kPincer, 0.03},
        {PetalID::kTringer, 0.006}
    }, { .aggro_radius = 750, .poison_damage = { 5.0, 1.0 } }},
    {"Spider", RarityID::kRare, {35.0}, 10.0, {15.0}, 8.0, {
        {PetalID::kWeb, 0.12},
        {PetalID::kStinger, 0.09},
        {PetalID::kTriweb, 0.006}
    }, { .poison_damage = { 5.0, 3.0 } }},
    {"Ant Hole", RarityID::kRare, {500.0}, 10.0, {45.0}, 25.0, {
        {PetalID::kTwin, 1},
        {PetalID::kWing, 0.24},
        {PetalID::kAntEgg, 0.06}
    }, { .stationary = 1 }},
    {"Queen Ant", RarityID::kEpic, {350.0}, 10.0, {25.0}, 15.0, {
        {PetalID::kTwin, 1},
        {PetalID::kWing, 0.24},
        {PetalID::kAntEgg, 0.06},
        {PetalID::kShield, 0.0006}
    }, { . aggro_radius = 750 }}
};

uint32_t RARITY_COLORS[RarityID::kNumRarities] = { 
    0xff7eef6d, 0xffffe65d, 0xff4d52e3, 
    0xff861fde, 0xffde1f1f, 0xff1fdbde
};//, 0xffff2b75, 0xfff70fb6};


float scoreToPassLevel(uint32_t level) {
    return pow(1.05, level) * (level + 1) * 2;
}

uint32_t scoreToLevel(float score) {
    uint32_t level = 0;
    while (1) {
        score -= scoreToPassLevel(level);
        if (score < 0) break;
        ++level;
    }
    return level;
}

float levelToScore(uint32_t level) {
    float score = 0;
    for (uint32_t i = 0; i < level; ++i)
        score += scoreToPassLevel(i);
    return score;
}

uint32_t loadOutSlotsAtLevel(uint32_t level) {
    if (level >= 45)
        return MAX_SLOT_COUNT;
    return 5 + level / 15;
}

float hpAtLevel(uint32_t level) {
    return BASE_HEALTH + level;
}