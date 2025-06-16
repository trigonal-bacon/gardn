#include <Shared/StaticData.hh>

#include <Shared/Map.hh>

#include <cmath>

extern const uint64_t VERSION_HASH = 198456321345ll;
extern const uint32_t SERVER_PORT = 9001;
extern const uint32_t MAX_LEVEL = 99;
extern const uint32_t TPS = 20;
extern const uint32_t DELETION_ANIMATION_TICKS = TPS / 5;

extern const float PETAL_DISABLE_DELAY = 45.0f; //seconds
extern const float PLAYER_ACCELERATION = 4.0f;
extern const float DEFAULT_FRICTION = 0.25f;

extern const float SUMMON_RETREAT_RADIUS = 600.0f;

extern const float BASE_FOV = 0.9f;
extern const float BASE_HEALTH = 100.0f;


extern struct PetalData const PETAL_DATA[PetalID::kNumPetals] = {
    {"None", "How can you see this?",
        0.0, 0.0, 0.0, 1.0, 0, RarityID::kCommon, {}},
    {"Basic", "A nice petal, not too strong but not too weak",
        10.0, 10.0, 10.0, 2.5, 1, RarityID::kCommon, {}},
    {"Fast", "Weaker than most petals, but reloads very quickly",
        5.0, 8.0, 7.0, 1.0, 1, RarityID::kCommon, {}},
    {"Heavy", "Very resilient and reals more damage, but reloads very slowly",
        20.0, 20.0, 12.0, 4.5, 1, RarityID::kCommon, {}},
    {"Stinger", "It really hurts, but it's really fragile",
        5.0, 35.0, 7.0, 3.5, 1, RarityID::kUnusual, {}},
    {"Leaf", "Gathers energy from the sun to passively heal your flower",
        10.0, 8.0, 10.0, 1.0, 1, RarityID::kUnusual, {
        .constant_heal = 1,
        .icon_angle = -1
    }},
    {"Twin", "Why stop at one? Why not TWO?!",
        5.0, 8.0, 7.0, 1.0, 2, RarityID::kUnusual, {}},
    {"Rose", "Its healing properties are amazing. Not so good at combat though",
        5.0, 5.0, 10.0, 3.5, 1, RarityID::kUnusual, { 
        .secondary_reload = 1.0,
        .burst_heal = 10,
        .defend_only = 1
    }},
    {"Iris", "Very poisonous, but takes a while to do its work",
         5.0, 5.0, 7.0, 5.0, 1, RarityID::kUnusual, { 
        .poison_damage = { 10.0, 6.0 }
    }},
    {"Missile", "You can actually shoot this one",
        5.0, 25.0, 10.0, 1.0, 1, RarityID::kRare, {
        .secondary_reload = 0.5, 
        .defend_only = 1,
        .icon_angle = 1,
        .rotation_style = PetalAttributes::kFollowRot 
    }},
    {"Dandelion", "Its interesting properties prevent healing effects on affected units",
        10.0, 10.0, 10.0, 1.0, 1, RarityID::kRare, {
        .icon_angle = 1,
        .rotation_style = PetalAttributes::kFollowRot 
    }},
    {"Bubble", "You can right click to pop it and propel your flower",
        1.0, 0.0, 12.0, 2.0, 1, RarityID::kRare, {
        .secondary_reload = 0.5,
        .defend_only = 1,
    }},
    {"Faster", "It's so light it makes your other petals spin faster",
        5.0, 7.0, 7.0, 0.5, 1, RarityID::kRare, {}},
    {"Rock", "Even more durable, but slower to recharge",
        100.0, 10.0, 12.0, 5.0, 1, RarityID::kRare, {}},
    {"Cactus", "Not very strong, but somehow increases your maximum health",
        15.0, 5.0, 10.0, 1.0, 1, RarityID::kRare, {}},
    {"Web", "It's very sticky",
        10.0, 5.0, 10.0, 3.0, 1, RarityID::kRare, {
        .secondary_reload = 0.5,
        .defend_only = 1,
    }},
    {"Wing", "It comes and goes",
        15.0, 20.0, 10.0, 2.5, 1, RarityID::kRare, {
        .icon_angle = 1,
    }},
    {"Peas", "4 in 1 deal",
        5.0, 8.0, 7.0, 2.0, 4, RarityID::kRare, {
        .clump_radius = 8,
        .secondary_reload = 1.0,
        .defend_only = 1
    }},
    {"Sand", "It's coarse, rough, and gets everywhere",
        10.0, 3.0, 7.0, 1.5, 4, RarityID::kRare, {
        .clump_radius = 10,
    }},
    {"Pincer", "Stuns and poisons targets for a short duration",
        10.0, 5.0, 10.0, 2.5, 1, RarityID::kRare, {
        .icon_angle = 0.7,
        .poison_damage = { 5.0, 1.0 }
    }},
    {"Dahlia", "Its healing properties are amazing. Not so good at combat though",
        5.0, 5.0, 7.0, 3.5, 3, RarityID::kRare, { 
        .clump_radius = 10,
        .secondary_reload = 1.0,
        .burst_heal = 3.5,
        .defend_only = 1
    }},
    {"Triplet", "How about THREE?!",
        5.0, 8.0, 7.0, 1.0, 3, RarityID::kEpic, {}},
    {"Egg", "Something interesting mught pop out of this",
        15.0, 1.0, 12.5, 1.0, 2, RarityID::kEpic, { 
        .secondary_reload = 3.5,
        .defend_only = 1,
        .rotation_style = PetalAttributes::kNoRot,
        .spawns = MobID::kSoldierAnt
    }},
    {"Iris", "Deals its effects quicker than traditional irises",
        10.0, 5.0, 7.0, 5.0, 1, RarityID::kEpic, { 
        .poison_damage = { 15.0, 4.0 }
    }},
    {"Pollen", "Shoutout to all the allergies out there",
        7.0, 8.0, 7.0, 1.5, 3, RarityID::kEpic, {
        .secondary_reload = 0.5,
        .defend_only = 1
    }},
    {"Peas", "4 in 1, not with a secret ingredient: poison",
        5.0, 2.0, 7.0, 2.0, 4, RarityID::kEpic, {
        .clump_radius = 8,
        .secondary_reload = 0.5,
        .defend_only = 1,
        .poison_damage = { 20.0, 0.5 }
    }},
    {"Egg", "Something interesting might pop out of this",
        15.0, 1.0, 15.0, 1.0, 1, RarityID::kEpic, { 
        .secondary_reload = 3.5,
        .defend_only = 1,
        .rotation_style = PetalAttributes::kNoRot,
        .spawns = MobID::kBeetle
    }},
    {"Rose", "Extremely powerful rose, almost unheard of",
        5.0, 5.0, 10.0, 3.5, 1, RarityID::kEpic, { 
        .secondary_reload = 1.0,
        .burst_heal = 22,
        .defend_only = 1
    }},
    {"Stick", "Harnesses the power of the wind",
        10.0, 1.0, 15.0, 3.0, 1, RarityID::kLegendary, { 
        .secondary_reload = 4.0,
        .defend_only = 1,
        .icon_angle = 1,
        .spawns = MobID::kSandstorm,
        .spawn_count = 2
    }},
    {"Stinger", "It really hurts, but it's really fragile",
        5.0, 35.0, 7.0, 4.5, 3, RarityID::kLegendary, {
        .clump_radius = 10
    }},
    {"Web", "It's very sticky",
        10.0, 5.0, 10.0, 3.0, 3, RarityID::kLegendary, {
        .clump_radius = 10,
        .secondary_reload = 0.5,
        .defend_only = 1,
    }},
    {"Antennae", "Lets your flower see foes rom farther away",
        0.0, 0.0, 12.5, 0.0, 0, RarityID::kLegendary, {}},
    {"Cactus", "Not very strong, but somehow increases your maximum health",
        15.0, 5.0, 10.0, 1.0, 3, RarityID::kLegendary, {
        .clump_radius = 10,
    }},
    {"Heaviest", "This thing is so heavy that nothing can get in its way",
        200.0, 10.0, 12.0, 15.0, 1, RarityID::kEpic, {
        .rotation_style = PetalAttributes::kNoRot
    }},
    {"Third Eye", "Allows your flower to extend petals further out",
        0.0, 0.0, 20.0, 0.0, 0, RarityID::kMythic, {}},
    {"Observer", "The one who sees all", 
        0.0, 0.0, 12.5, 0.0, 0, RarityID::kMythic, {}},
    {"Cactus", "Turns your flower poisonous. Enemies will take poison damage on contact",
        15.0, 5.0, 10.0, 1.0, 1, RarityID::kEpic, {
        .poison_damage = { 1.0, 5.0 }
    }},
    {"Salt", "Reflects some damage dealt to the flower. Does not stack",
        10.0, 10.0, 10.0, 2.5, 1, RarityID::kRare, {}},
    {"Basic", "Something extremely rare and useless",
        10.0, 10.0, 10.0, 2.5, 1, RarityID::kUnique, {}},
    {"Square", "Wrong game^2",
        10.0, 10.0, 15.0, 2.5, 1, RarityID::kUnique, {
        .icon_angle = M_PI / 4 + 1
    }},
    {"Moon", "Where did this come from?",
        10000.0, 1.0, 50.0, 10.0, 1, RarityID::kMythic, {
        .secondary_reload = 0.5
    }},
    {"Lotus", "Absorbs some poison damage taken by the flower",
        5.0, 5.0, 12.0, 2.0, 1, RarityID::kEpic, {
        .icon_angle = 0.1
    }},
    {"Cutter", "Increases the flower's body damage",
        0.0, 0.0, 40.0, 0.0, 0, RarityID::kEpic, {}},
    {"Yin Yang", "Alters the flower's petal rotation in interesting ways",
        15.0, 15.0, 10.0, 2.5, 1, RarityID::kEpic, {}},
    {"Yggdrasil", "Unfortunately, its powers are useless here",
        1.0, 1.0, 12.0, 10.0, 1, RarityID::kUnique, {
        .defend_only = 1,
        .icon_angle = M_PI
    }},
    {"Rice", "Spawns instantly, but not very strong",
        1.0, 4.0, 13.0, 0, 1, RarityID::kEpic, {
        .icon_angle = 0.7
    }},
    {"Bone", "Sturdy",
        12.0, 10.0, 12.0, 2.5, 1, RarityID::kLegendary, {
        .icon_angle = 1
    }},
    {"Yucca", "Heals the flower, but only while in the defensive position",
        10.0, 5.0, 10.0, 1.0, 1, RarityID::kUnusual, {
        .constant_heal = 1.5,
        .icon_angle = -1
    }},
};

extern struct MobData const MOB_DATA[MobID::kNumMobs] = {
    {
        "Baby Ant",
        "Weak and defenseless, but big dreams.",
        RarityID::kCommon, {10.0}, 10.0, {14.0}, 1, {
        PetalID::kLight, PetalID::kLeaf, PetalID::kTwin, PetalID::kRice, PetalID::kTriplet
    }, {}},
    {
        "Worker Ant",
        "It's temperamental, probably from working all the time.",
        RarityID::kCommon, {20.0}, 10.0, {14.0}, 3, {
        PetalID::kLight, PetalID::kLeaf, PetalID::kTwin, PetalID::kTriplet, PetalID::kBone
    }, {}},
    {
        "Soldier Ant",
        "It's got wings and it's ready to use them.",
        RarityID::kUnusual, {40.0}, 10.0, {14.0}, 5, {
        PetalID::kTwin, PetalID::kIris, PetalID::kWing, PetalID::kFaster, PetalID::kTriplet
    }, {}},
    {
        "Bee",
        "It stings. Don't touch it.",
        RarityID::kCommon, {20.0}, 50.0, {20.0}, 4, {
        PetalID::kLight, PetalID::kStinger, PetalID::kTwin, PetalID::kWing
    }, {}},
    {
        "Ladybug",
        "Cute and harmless.",
        RarityID::kCommon, {25.0}, 10.0, {30.0}, 3, {
        PetalID::kLight, PetalID::kRose, PetalID::kTwin, PetalID::kBubble
    }, {}},
    {
        "Beetle",
        "It's hungry and flowers are its favorite meal.",
        RarityID::kUnusual, {40.0}, 25.0, {35.0}, 10, {
        PetalID::kIris, PetalID::kSalt, PetalID::kWing, PetalID::kTriplet
    }, {}},
    {
        "Massive Ladybug",
        "Much larger, but still cute.",
        RarityID::kEpic, {1000.0}, 10.0, {90.0}, 400, {
        PetalID::kRose, PetalID::kDahlia, PetalID::kBubble, PetalID::kAzalea, PetalID::kObserver
    }, {}},
    {
        "Massive Beetle",
        "Someone overfed this one, you might be next.",
        RarityID::kRare, {600.0}, 30.0, {75.0}, 50, {
        PetalID::kIris, PetalID::kWing, PetalID::kBlueIris, PetalID::kTriplet, PetalID::kBeetleEgg, PetalID::kThirdEye
    }, { .aggro_radius = 1000 }},
    {
        "Ladybug",
        "Cute and harmless... if left unprovoked.",
        RarityID::kUnusual, {35.0}, 10.0, {30.0}, 5, {
        PetalID::kDahlia, PetalID::kWing, PetalID::kYinYang, PetalID::kAzalea
    }, {}},
    {
        "Hornet",
        "These aren't quite as nice as the little bees.",
        RarityID::kUnusual, {40.0}, 40.0, {35.0}, 12, {
        PetalID::kDandelion, PetalID::kMissile, PetalID::kWing, PetalID::kAntennae
    }, { .aggro_radius = 750 }},
    {
        "Cactus",
        "This one's prickly, don't touch it either.",
        RarityID::kCommon, {25.0, 50.0}, 30.0, {30.0, 60.0}, 2, {
        PetalID::kStinger, PetalID::kYucca, PetalID::kCactus, PetalID::kPoisonCactus, PetalID::kTricac
    }, { .stationary = 1 }},
    {
        "Rock",
        "A rock. It doesn't do much.",
        RarityID::kCommon, {5.0, 15.0}, 10.0, {10.0, 25.0}, 1, {
        PetalID::kHeavy, PetalID::kLight, PetalID::kRock
    }, { .stationary = 1 }},
    {
        "Boulder",
        "A bigger rock. It also doesn't do much.",
        RarityID::kUnusual, {40.0, 60.0}, 10.0, {40.0, 60.0}, 1, {
        PetalID::kHeavy, PetalID::kRock, PetalID::kHeaviest, PetalID::kMoon
    }, { .stationary = 1 }},
    {
        "Centipede",
        "It's just there doing its thing.",
        RarityID::kUnusual, {50.0}, 10.0, {35.0}, 2, {
        PetalID::kLight, PetalID::kTwin, PetalID::kLeaf, PetalID::kPeas, PetalID::kTriplet
    }, { .segments = 10 }},
    {
        "Evil Centipede",
        "This one loves flowers.",
        RarityID::kRare, {50.0}, 10.0, {35.0}, 3, {
        PetalID::kIris, PetalID::kPoisonPeas, PetalID::kBlueIris
    }, { .segments = 10, .poison_damage = { 5.0, 2.0 } }},
    {
        "Desert Centipede",
        "It doesn't like it when you interrupt its run.",
        RarityID::kRare, {50.0}, 10.0, {35.0}, 4, {
        PetalID::kSand, PetalID::kFaster, PetalID::kSalt
    }, { .segments = 6 }},
    {
        "Sandstorm",
        "Quite unpredictable.",
        RarityID::kUnusual, {30.0, 45.0}, 40.0, {32.0, 48.0}, 5, {
        PetalID::kSand, PetalID::kFaster, PetalID::kStick
    }, {}},
    {
        "Scorpion",
        "This one stings, now with poison.",
        RarityID::kUnusual, {35.0}, 15.0, {35.0}, 10, {
        PetalID::kIris, PetalID::kPincer, PetalID::kTriplet
    }, { .aggro_radius = 750, .poison_damage = { 5.0, 1.0 } }},
    {
        "Spider",
        "Spooky.",
        RarityID::kUnusual, {35.0}, 10.0, {15.0}, 8, {
        PetalID::kStinger, PetalID::kWeb, PetalID::kFaster, PetalID::kTriweb
    }, { .poison_damage = { 5.0, 3.0 } }},
    {
        "Ant Hole",
        "Ants go in, and come out. Can't explain that.",
        RarityID::kRare, {500.0}, 10.0, {45.0}, 25, {
        PetalID::kIris, PetalID::kWing, PetalID::kAntEgg, PetalID::kTriplet
    }, { .stationary = 1 }},
    {
        "Queen Ant",
        "You must have done something really bad if she's chasing you.",
        RarityID::kRare, {350.0}, 10.0, {25.0}, 15, {
        PetalID::kTwin, PetalID::kIris, PetalID::kWing, PetalID::kAntEgg, PetalID::kTringer
    }, { . aggro_radius = 750 }},
    {
        "Ladybug",
        "This one is shiny... I wonder what it could mean...",
        RarityID::kEpic, {25.0}, 10.0, {30.0}, 3, {
        PetalID::kDahlia, PetalID::kWing, PetalID::kBubble, PetalID::kYggdrasil
    }, {}},
    {
        "Square",
        "???",
        RarityID::kUnique, {20.0}, 10.0, {40.0}, 1, {
        PetalID::kSquare
    }, { .stationary = 1 }},
    {
        "Digger",
        "Friend or foe? You'll never know...",
        RarityID::kEpic, {250.0}, 25.0, {40.0}, 1, {
        PetalID::kCutter
    }, {}},
};

extern uint32_t const RARITY_COLORS[RarityID::kNumRarities] = { 
    0xff7eef6d, 0xffffe65d, 0xff4d52e3, 
    0xff861fde, 0xffde1f1f, 0xff1fdbde,
    0xffde1f65
 }; // 0xffff2b75, 0xfff70fb6};

char const *RARITY_NAMES[RarityID::kNumRarities] = {
    "Common",
    "Unusual",
    "Rare",
    "Epic",
    "Legendary",
    "Mythic",
    "Unique"
};

extern std::array<std::vector<float>, MobID::kNumMobs> const MOB_DROP_CHANCES = Map::get_auto_petal_drops();

uint32_t score_to_pass_level(uint32_t level) {
    return (uint32_t)(pow(1.06, level - 1) * level) + 3;
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