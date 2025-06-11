#include <Shared/StaticData.hh>

#include <cmath>

extern const uint32_t TPS = 25;
extern const uint32_t DELETION_ANIMATION_TICKS = TPS / 5;

extern const float PETAL_DISABLE_DELAY = 30.0f;
extern const float PLAYER_ACCELERATION = 3.0f;
extern const float DEFAULT_FRICTION = 0.25f;

extern const float SUMMON_RETREAT_RADIUS = 600.0f;

extern const float BASE_FOV = 0.9f;
extern const float BASE_HEALTH = 100.0f;


extern struct PetalData const PETAL_DATA[PetalID::kNumPetals] = {
    {"None", "How can you see this?",
        0.0, 0.0, 0.0, 1.0, 0, RarityID::kCommon, {}},
    {"Basic", "A nice petal, not too strong but not too weak",
        10.0, 10.0, 10.0, 2.5, 1, RarityID::kCommon, {}},
    {"Light", "Reloads quickly, but is weaker",
        5.0, 7.0, 7.0, 1.0, 1, RarityID::kCommon, {}},
    {"Heavy", "Takes a while to reload, but is more durable",
        20.0, 20.0, 12.0, 4.5, 1, RarityID::kCommon, {}},
    {"Stinger", "This one stings",
        5.0, 35.0, 7.0, 3.5, 1, RarityID::kUnusual, {}},
    {"Leaf", "Passively heals you",
        8.0, 10.0, 10.0, 1.0, 1, RarityID::kUnusual, {
        .constant_heal = 1,
        .icon_angle = -1
    }},
    {"Twin", "Two for one deal",
        5.0, 7.0, 7.0, 1.0, 2, RarityID::kUnusual, {}},
    {"Rose", "Heals you",
        5.0, 5.0, 10.0, 3.5, 1, RarityID::kUnusual, { 
        .secondary_reload = 1.0,
        .burst_heal = 10,
        .defend_only = 1
    }},
    {"Iris", "Poisonous, but takes a while to do its work",
         5.0, 5.0, 7.0, 5.0, 1, RarityID::kUnusual, { 
        .poison_damage = { 10.0, 6.0 }
    }},
    {"Missile", "You can shoot this one",
        5.0, 25.0, 10.0, 1.0, 1, RarityID::kRare, {
        .secondary_reload = 0.5, 
        .defend_only = 1,
        .icon_angle = 1,
        .rotation_style = PetalAttributes::kFollowRot 
    }},
    {"Dandelion", "Prevents targets from healing",
        10.0, 10.0, 10.0, 1.0, 1, RarityID::kRare, {
        .icon_angle = 1,
        .rotation_style = PetalAttributes::kFollowRot 
    }},
    {"Bubble", "Pop",
        1.0, 0.0, 12.0, 2.0, 1, RarityID::kRare, {
        .secondary_reload = 0.5,
        .defend_only = 1,
    }},
    {"Faster", "It's so light it makes your other petals spin faster",
        5.0, 7.0, 7.0, 0.5, 1, RarityID::kRare, {}},
    {"Rock", "Even more durable, but slower to recharge",
        100.0, 10.0, 12.0, 5.0, 1, RarityID::kRare, {}},
    {"Cactus", "Gives you extra health",
        15.0, 5.0, 10.0, 1.0, 1, RarityID::kRare, {}},
    {"Web", "Slows down enemies",
        10.0, 5.0, 10.0, 3.0, 1, RarityID::kRare, {
        .secondary_reload = 0.5,
        .defend_only = 1,
    }},
    {"Wing", "It comes and goes",
        15.0, 20.0, 10.0, 2.5, 1, RarityID::kRare, {
        .icon_angle = 1,
    }},
    {"Peas", "Four in one deal",
        5.0, 8.0, 7.0, 2.0, 4, RarityID::kRare, {
        .clump_radius = 8,
        .secondary_reload = 1.0,
        .defend_only = 1
    }},
    {"Sand", "Another four in one deal",
        10.0, 3.0, 7.0, 1.5, 4, RarityID::kRare, {
        .clump_radius = 10,
    }},
    {"Pincer", "Stuns targets",
        10.0, 5.0, 10.0, 2.5, 1, RarityID::kRare, {
        .icon_angle = 0.7,
        .poison_damage = { 5.0, 1.0 }
    }},
    {"Dahlia", "Heals less, but more consistently",
        5.0, 5.0, 7.0, 3.5, 3, RarityID::kRare, { 
        .clump_radius = 10,
        .secondary_reload = 1.0,
        .burst_heal = 3.5,
        .defend_only = 1
    }},
    {"Triplet", "Three in one deal",
        5.0, 7.0, 7.0, 1.0, 3, RarityID::kEpic, {}},
    {"Egg", "I wonder what this one does",
        15.0, 1.0, 12.5, 1.0, 2, RarityID::kEpic, { 
        .secondary_reload = 3.5,
        .defend_only = 1,
        .rotation_style = PetalAttributes::kNoRot,
        .spawns = MobID::kSoldierAnt
    }},
    {"Iris", "Even more poisonous",
        10.0, 5.0, 7.0, 5.0, 1, RarityID::kEpic, { 
        .poison_damage = { 15.0, 4.0 }
    }},
    {"Pollen", "Shoutout to all the allergies out there",
        7.0, 8.0, 7.0, 1.5, 3, RarityID::kEpic, {
        .secondary_reload = 0.5,
        .defend_only = 1
    }},
    {"Peas", "Poisonous four in one",
        5.0, 2.0, 7.0, 2.0, 4, RarityID::kEpic, {
        .clump_radius = 8,
        .secondary_reload = 0.5,
        .defend_only = 1,
        .poison_damage = { 20.0, 0.5 }
    }},
    {"Egg", "I wonder what this one does",
        15.0, 1.0, 15.0, 1.0, 1, RarityID::kEpic, { 
        .secondary_reload = 3.5,
        .defend_only = 1,
        .rotation_style = PetalAttributes::kNoRot,
        .spawns = MobID::kBeetle
    }},
    {"Azalea", "Heals you",
        5.0, 5.0, 10.0, 3.5, 1, RarityID::kEpic, { 
        .secondary_reload = 1.0,
        .burst_heal = 22,
        .defend_only = 1
    }},
    {"Stick", "Mob spawner",
        10.0, 1.0, 15.0, 3.0, 1, RarityID::kLegendary, { 
        .secondary_reload = 4.0,
        .defend_only = 1,
        .icon_angle = 1,
        .spawns = MobID::kSandstorm,
        .spawn_count = 2
    }},
    {"Stinger", "This one stings",
        5.0, 35.0, 7.0, 4.5, 3, RarityID::kLegendary, {
        .clump_radius = 10
    }},
    {"Web", "Slows down enemies",
        10.0, 5.0, 10.0, 3.0, 3, RarityID::kLegendary, {
        .clump_radius = 10,
        .secondary_reload = 0.5,
        .defend_only = 1,
    }},
    {"Antennae", "Lets you see farther",
        0.0, 0.0, 12.5, 0.0, 0, RarityID::kLegendary, {}},
    {"Cactus", "Gives you extra health",
        15.0, 5.0, 10.0, 1.0, 3, RarityID::kLegendary, {
        .clump_radius = 10,
    }},
    {"Shield", "Captain America",
        1000.0, 1.0, 25.0, 5.0, 1, RarityID::kMythic, {
        .rotation_style = PetalAttributes::kNoRot
    }},
    {"Third Eye", "Your petals now hate you",
        0.0, 0.0, 20.0, 0.0, 0, RarityID::kMythic, {}},
    {"Observer", "The one who sees all", 
        0.0, 0.0, 12.5, 0.0, 0, RarityID::kMythic, {}},
    {"Cactus", "Gives you extra health: now with poison",
        15.0, 5.0, 10.0, 1.0, 1, RarityID::kEpic, {
        .poison_damage = { 1.0, 5.0 }
    }},
    {"Salt", "Reflects some damage dealt to the flower. Does not stack",
        10.0, 10.0, 10.0, 2.5, 1, RarityID::kRare, {}},
    {"Basic", "A nice petal, not too strong but not too weak",
        10.0, 10.0, 10.0, 2.5, 1, RarityID::kUnique, {}},
    {"Square", "Wrong game^2",
        10.0, 10.0, 15.0, 2.5, 1, RarityID::kUnique, {
        .icon_angle = M_PI / 4 + 1
    }},
    {"Moon", "Where did this come from?",
        10000.0, 1.0, 50.0, 10.0, 1, RarityID::kUnique, {
        .secondary_reload = 0.5
    }},
};

extern struct MobData const MOB_DATA[MobID::kNumMobs] = {
    {
        "Baby Ant",
        "Weak and defenseless, but big dreams.",
        RarityID::kCommon, {10.0}, 10.0, {14.0}, 1, {
        {PetalID::kLight, 0.36},
        {PetalID::kTwin, 0.12},
        {PetalID::kLeaf, 0.09},
        {PetalID::kTriplet, 0.005}
    }, {}},
    {
        "Worker Ant",
        "It's temperamental, probably from working all the time.",
        RarityID::kCommon, {20.0}, 10.0, {14.0}, 3, {
        {PetalID::kLight, 0.36},
        {PetalID::kTwin, 0.12},
        {PetalID::kLeaf, 0.09}
    }, {}},
    {
        "Soldier Ant",
        "It's got wings and it's ready to use them.",
        RarityID::kUnusual, {40.0}, 10.0, {14.0}, 5, {
        {PetalID::kLight, 0.36},
        {PetalID::kTwin, 0.12},
        {PetalID::kWing, 0.03}
    }, {}},
    {
        "Bee",
        "It stings. Don't touch it.",
        RarityID::kCommon, {20.0}, 50.0, {20.0}, 4, {
        {PetalID::kLight, 0.36},
        {PetalID::kStinger, 0.12},
        {PetalID::kWing, 0.03},
        {PetalID::kBubble, 0.01}
    }, {}},
    {
        "Ladybug",
        "Cute and harmless.",
        RarityID::kCommon, {25.0}, 10.0, {30.0}, 3, {
        {PetalID::kLight, 0.36},
        {PetalID::kRose, 0.18},
        {PetalID::kTwin, 0.12},
        {PetalID::kWing, 0.03},
        {PetalID::kBubble, 0.01}
    }, {}},
    {
        "Beetle",
        "It's hungry and flowers are its favorite meal.",
        RarityID::kUnusual, {40.0}, 25.0, {35.0}, 10, {
        {PetalID::kIris, 0.18},
        {PetalID::kTwin, 0.12},
        {PetalID::kSalt, 0.06},
        {PetalID::kWing, 0.03}
    }, {}},
    {
        "Massive Ladybug",
        "Much larger, but still cute.",
        RarityID::kEpic, {1000.0}, 10.0, {90.0}, 400, {
        {PetalID::kRose, 1},
        {PetalID::kDahlia, 1},
        {PetalID::kBubble, 1},
        {PetalID::kAzalea, 1},
        {PetalID::kObserver, 0.0006}
    }, {}},
    {
        "Massive Beetle",
        "Someone overfed this one, you might be next.",
        RarityID::kRare, {600.0}, 30.0, {75.0}, 50, {
        {PetalID::kIris, 1},
        {PetalID::kSalt, 1},
        {PetalID::kTriplet, 0.06},
        {PetalID::kBeetleEgg, 0.06},
        {PetalID::kThirdEye, 0.0006}
    }, { .aggro_radius = 1000 }},
    {
        "Dark Ladybug",
        "Cute and harmless... if left unprovoked.",
        RarityID::kUnusual, {35.0}, 10.0, {30.0}, 5, {
        {PetalID::kWing, 0.12},
        {PetalID::kDahlia, 0.36},
        {PetalID::kAzalea, 0.02}
    }, {}},
    {
        "Hornet",
        "These aren't quite as nice as the little bees.",
        RarityID::kUnusual, {40.0}, 40.0, {35.0}, 12, {
        {PetalID::kMissile, 0.18},
        {PetalID::kDandelion, 0.12},
        {PetalID::kWing, 0.09},
        {PetalID::kAntennae, 0.003}
    }, { .aggro_radius = 750 }},
    {
        "Cactus",
        "This one's prickly, don't touch it either.",
        RarityID::kCommon, {25.0, 50.0}, 30.0, {30.0, 60.0}, 2, {
        {PetalID::kStinger, 0.18},
        {PetalID::kCactus, 0.03},
        {PetalID::kPoisonCactus, 0.001},
        {PetalID::kTricac, 0.00005}
    }, { .stationary = 1 }},
    {
        "Rock",
        "A rock. It doesn't do much.",
        RarityID::kCommon, {10.0, 20.0}, 10.0, {10.0, 25.0}, 1, {
        {PetalID::kHeavy, 0.24},
        {PetalID::kLight, 0.12},
        {PetalID::kRock, 0.01}
    }, { .stationary = 1 }},
    {
        "Boulder",
        "A bigger rock. It also doesn't do much.",
        RarityID::kUnusual, {35.0, 50.0}, 10.0, {35.0, 50.0}, 1, {
        {PetalID::kHeavy, 1},
        {PetalID::kRock, 0.04},
        {PetalID::kShield, 0.001},
        {PetalID::kMoon, 0.00001}
    }, { .stationary = 1 }},
    {
        "Centipede",
        "It's just there doing its thing.",
        RarityID::kUnusual, {50.0}, 10.0, {35.0}, 2, {
        {PetalID::kLight, 0.24},
        {PetalID::kTwin, 0.12},
        {PetalID::kLeaf, 0.12},
        {PetalID::kPeas, 0.01}
    }, { .segments = 10 }},
    {
        "Evil Centipede",
        "This one loves flowers.",
        RarityID::kRare, {50.0}, 10.0, {35.0}, 3, {
        {PetalID::kIris, 0.18},
        {PetalID::kTwin, 0.12},
        {PetalID::kPoisonPeas, 0.01},
        {PetalID::kBlueIris, 0.01}
    }, { .segments = 10, .poison_damage = { 5.0, 2.0 } }},
    {
        "Desert Centipede",
        "It doesn't like it when you interrupt its run.",
        RarityID::kRare, {50.0}, 10.0, {35.0}, 4, {
        {PetalID::kTwin, 0.12},
        {PetalID::kLeaf, 0.12},
        {PetalID::kFaster, 0.01},
        {PetalID::kStick, 0.001}
    }, { .segments = 6 }},
    {
        "Sandstorm",
        "Quite unpredictable.",
        RarityID::kUnusual, {30.0, 45.0}, 40.0, {32.0, 48.0}, 5, {
        {PetalID::kSand, 0.12},
        {PetalID::kFaster, 0.12},
        {PetalID::kStick, 0.005}
    }, { .stationary = 1 }},
    {
        "Scorpion",
        "This one stings, now with poison.",
        RarityID::kUnusual, {35.0}, 15.0, {35.0}, 10, {
        {PetalID::kIris, 0.24},
        {PetalID::kStinger, 0.18},
        {PetalID::kPincer, 0.06},
        {PetalID::kTringer, 0.005}
    }, { .aggro_radius = 750, .poison_damage = { 5.0, 1.0 } }},
    {
        "Spider",
        "Spooky.",
        RarityID::kUnusual, {35.0}, 10.0, {15.0}, 8, {
        {PetalID::kStinger, 0.24},
        {PetalID::kWeb, 0.18},
        {PetalID::kTriweb, 0.005}
    }, { .poison_damage = { 5.0, 3.0 } }},
    {
        "Ant Hole",
        "Ants go in, and come out. Can't explain that.",
        RarityID::kRare, {500.0}, 10.0, {45.0}, 25, {
        {PetalID::kTwin, 1},
        {PetalID::kIris, 1},
        {PetalID::kWing, 0.48},
        {PetalID::kTriplet, 0.06},
        {PetalID::kAntEgg, 0.06}
    }, { .stationary = 1 }},
    {
        "Queen Ant",
        "You must have done something really bad if she's chasing you.",
        RarityID::kRare, {350.0}, 10.0, {25.0}, 15, {
        {PetalID::kTwin, 1},
        {PetalID::kWing, 0.24},
        {PetalID::kTriplet, 0.06},
        {PetalID::kAntEgg, 0.06},
        {PetalID::kShield, 0.0006}
    }, { . aggro_radius = 750 }},
    {
        "Ladybug",
        "This one is shiny... I wonder what it could mean...",
        RarityID::kEpic, {25.0}, 10.0, {30.0}, 3, {
        {PetalID::kRose, 1},
        {PetalID::kWing, 1},
        {PetalID::kDahlia, 1},
        {PetalID::kAzalea, 0.06}
    }, {}},
    {
        "Square",
        "???",
        RarityID::kUnique, {20.0}, 10.0, {40.0}, 1, {
        {PetalID::kSquare, 1}
    }, { .stationary = 1 }},
};

extern uint32_t const RARITY_COLORS[RarityID::kNumRarities] = { 
    0xff7eef6d, 0xffffe65d, 0xff4d52e3, 
    0xff861fde, 0xffde1f1f, 0xff1fdbde,
    0xffff2b75
 }; // 0xfff70fb6};

char const *RARITY_NAMES[RarityID::kNumRarities] = {
    "Common",
    "Unusual",
    "Rare",
    "Epic",
    "Legendary",
    "Mythic",
    "Unique"
};

uint32_t score_to_pass_level(uint32_t level) {
    return (uint32_t)(pow(1.05, level) * level * 2);
}

uint32_t score_to_level(uint32_t score) {
    uint32_t level = 1;
    while (1) {
        uint32_t level_score = score_to_pass_level(level);
        if (score < level_score) break;
        score -= level_score;
        ++level;
    }
    return level;
}

uint32_t level_to_score(uint32_t level) {
    uint32_t score = 0;
    for (uint32_t i = 1; i < level; ++i)
        score += score_to_pass_level(i);
    return score;
}

uint32_t loadout_slots_at_level(uint32_t level) {
    if (level >= 45)
        return MAX_SLOT_COUNT;
    return 5 + level / 15;
}

float hp_at_level(uint32_t level) {
    return BASE_HEALTH + level;
}