#include <Shared/StaticData.hh>

#include <cmath>

uint32_t const MAX_LEVEL = 99;
uint32_t const TPS = 20;

float const PETAL_DISABLE_DELAY = 45.0f; //seconds
float const PLAYER_ACCELERATION = 5.0f;
float const DEFAULT_FRICTION = 1.0f/3.0f;
float const SUMMON_RETREAT_RADIUS = 600.0f;
float const DIGGER_SPAWN_CHANCE = 0.25f;
float const TEAMMATE_HEAL_RADIUS = 200.0f;

float const BASE_FLOWER_RADIUS = 24.0f;
float const BASE_PETAL_ROTATION_SPEED = 2.5f;
float const BASE_FOV = 0.9f;
float const BASE_HEALTH = 100.0f;
float const BASE_BODY_DAMAGE = 20.0f;

std::array<struct PetalData, PetalID::kNumPetals> const PETAL_DATA = {{
    {
        .name = "None",
        .description = "How can you see this?",
        .health = 0.0, 
        .damage = 0.0,
        .radius = 0.0,
        .reload = 1.0,
        .count = 0,
        .rarity = RarityID::kCommon,
        .attributes = {}
    },
    {
        .name = "Basic",
        .description = "A nice petal, not too strong but not too weak",
        .health = 10.0,
        .damage = 10.0,
        .radius = 10.0,
        .reload = 2.5,
        .count = 1,
        .rarity = RarityID::kCommon,
        .attributes = {}
    },
    {
        .name = "Fast",
        .description = "Weaker than most petals, but reloads very quickly",
        .health = 5.0,
        .damage = 8.0,
        .radius = 7.0,
        .reload = 1.0,
        .count = 1,
        .rarity = RarityID::kCommon,
        .attributes = {}
    },
    {
        .name = "Heavy",
        .description = "Very resilient and deals more damage, but reloads very slowly",
        .health = 20.0,
        .damage = 20.0,
        .radius = 12.0,
        .reload = 4.5,
        .count = 1,
        .rarity = RarityID::kCommon,
        .attributes = {}
    },
    {
        .name = "Stinger",
        .description = "It really hurts, but it's really fragile",
        .health = 5.0,
        .damage = 35.0,
        .radius = 7.0,
        .reload = 4.0,
        .count = 1,
        .rarity = RarityID::kUnusual,
        .attributes = {}
    },
    {
        .name = "Leaf",
        .description = "Gathers energy from the sun to passively heal your flower",
        .health = 10.0,
        .damage = 8.0,
        .radius = 10.0,
        .reload = 1.0,
        .count = 1,
        .rarity = RarityID::kUnusual,
        .attributes = {
            .constant_heal = 1,
            .icon_angle = -1
        }
    },
    {
        .name = "Twin",
        .description = "Why stop at one? Why not TWO?!",
        .health = 5.0,
        .damage = 8.0,
        .radius = 7.0,
        .reload = 1.0,
        .count = 2,
        .rarity = RarityID::kUnusual,
        .attributes = {}
    },
    {
        .name = "Rose",
        .description = "Its healing properties are amazing. Not so good at combat though",
        .health = 5.0,
        .damage = 5.0,
        .radius = 10.0,
        .reload = 3.5,
        .count = 1,
        .rarity = RarityID::kUnusual,
        .attributes = { 
            .secondary_reload = 1.0,
            .burst_heal = 10,
            .defend_only = 1
        }
    },
    {
        .name = "Iris",
        .description = "Very poisonous, but takes a while to do its work",
        .health = 5.0,
        .damage = 5.0,
        .radius = 7.0,
        .reload = 5.0,
        .count = 1,
        .rarity = RarityID::kUnusual,
        .attributes = { 
            .poison_damage = {
                .damage = 10.0,
                .time = 6.0
            }
        }
    },
    {
        .name = "Missile",
        .description = "You can actually shoot this one",
        .health = 5.0,
        .damage = 25.0,
        .radius = 10.0,
        .reload = 1.0,
        .count = 1,
        .rarity = RarityID::kRare,
        .attributes = {
            .secondary_reload = 0.5, 
            .defend_only = 1,
            .icon_angle = 1,
            .rotation_style = PetalAttributes::kFollowRot 
        }
    },
    {
        .name = "Dandelion",
        .description = "Its interesting properties prevent healing effects on affected units",
        .health = 15.0,
        .damage = 8.0,
        .radius = 10.0,
        .reload = 1.0,
        .count = 1,
        .rarity = RarityID::kRare,
        .attributes = {
            .secondary_reload = 0.5, 
            .defend_only = 1,
            .icon_angle = 1,
            .rotation_style = PetalAttributes::kFollowRot 
        }
    },
    {
        .name = "Bubble",
        .description = "You can right click to pop it and propel your flower",
        .health = 1.0,
        .damage = 0.0,
        .radius = 12.0,
        .reload = 1.9,
        .count = 1,
        .rarity = RarityID::kRare,
        .attributes = {
            .secondary_reload = 0.4,
            .defend_only = 1,
        }
    },
    {
        .name = "Faster",
        .description = "It's so light it makes your other petals spin faster",
        .health = 5.0,
        .damage = 7.0,
        .radius = 7.0,
        .reload = 0.5,
        .count = 1,
        .rarity = RarityID::kRare,
        .attributes = {}
    },
    {
        .name = "Fastest",
        .description = "Triplicātus!",
        .health = 5.0,
        .damage = 7.0,
        .radius = 12.0,
        .reload = 0.5,
        .count = 1,
        .rarity = RarityID::kLegendary,
        .attributes = {
            .clump_radius = 7
        }
    },
    {
        .name = "Rock",
        .description = "Even more durable, but slower to recharge",
        .health = 100.0,
        .damage = 10.0,
        .radius = 12.0,
        .reload = 7.5,
        .count = 1,
        .rarity = RarityID::kRare,
        .attributes = {}
    },
    {
        .name = "Cactus",
        .description = "Not very strong, but somehow increases your maximum health",
        .health = 15.0,
        .damage = 5.0, 
        .radius = 10.0,
        .reload = 1.0,
        .count = 1,
        .rarity = RarityID::kRare,
        .attributes = {}
    },
    {
        .name = "Web",
        .description = "It's really sticky",
        .health = 10.0,
        .damage = 5.0,
        .radius = 10.0,
        .reload = 3.0,
        .count = 1,
        .rarity = RarityID::kRare,
        .attributes = {
            .secondary_reload = 0.5,
            .defend_only = 1,
        }
    },
    {
        .name = "Wing",
        .description = "It comes and goes",
        .health = 15.0,
        .damage = 15.0,
        .radius = 10.0,
        .reload = 1.25,
        .count = 1,
        .rarity = RarityID::kRare,
        .attributes = {
            .icon_angle = 1,
        }
    },
    {
        .name = "Peas",
        .description = "4 in 1 deal",
        .health = 10.0,
        .damage = 8.0,
        .radius = 7.0,
        .reload = 2.0,
        .count = 4,
        .rarity = RarityID::kRare,
        .attributes = {
            .clump_radius = 8,
            .secondary_reload = 0.1,
            .defend_only = 1,
        }
    },
    {
        .name = "Sand",
        .description = "It's coarse, rough, and gets everywhere",
        .health = 10.0,
        .damage = 4.0,
        .radius = 7.0,
        .reload = 1.5,
        .count = 4,
        .rarity = RarityID::kRare,
        .attributes = {
            .clump_radius = 10,
        }
    },
    {
        .name = "Pincer",
        .description = "Stuns and poisons targets for a short duration",
        .health = 10.0,
        .damage = 10.0,
        .radius = 10.0,
        .reload = 2.5,
        .count = 1,
        .rarity = RarityID::kRare,
        .attributes = {
            .icon_angle = 0.7,
            .poison_damage = {
                .damage = 5.0,
                .time = 1.0
            }
        }
    },
    {
        .name = "Rose",
        .description = "Its healing properties are amazing. Not so good at combat though",
        .health = 5.0,
        .damage = 5.0,
        .radius = 7.0,
        .reload = 3.5,
        .count = 3,
        .rarity = RarityID::kRare,
        .attributes = { 
            .clump_radius = 10,
            .secondary_reload = 1.0,
            .burst_heal = 3.5,
            .defend_only = 1
        }
    },
    {
        .name = "Triplet",
        .description = "How about THREE?!",
        .health = 5.0,
        .damage = 8.0,
        .radius = 7.0,
        .reload = 1.0,
        .count = 3,
        .rarity = RarityID::kEpic,
        .attributes = {}
    },
    {
        .name = "Egg",
        .description = "Something interesting might pop out of this",
        .health = 50.0,
        .damage = 1.0,
        .radius = 12.5,
        .reload = 1.0,
        .count = 2,
        .rarity = RarityID::kEpic,
        .attributes = { 
            .secondary_reload = 3.5,
            .defend_only = 1,
            .rotation_style = PetalAttributes::kNoRot,
            .spawns = MobID::kSoldierAnt
        }
    },
    {
        .name = "Iris",
        .description = "Deals its effects quicker than traditional irises",
        .health = 10.0,
        .damage = 5.0,
        .radius = 7.0,
        .reload = 5.0,
        .count = 1,
        .rarity = RarityID::kEpic,
        .attributes = { 
            .poison_damage = {
                .damage = 15.0,
                .time = 4.0
            }
        }
    },
    {
        .name = "Pollen",
        .description = "Asthmatics beware",
        .health = 7.0,
        .damage = 8.0,
        .radius = 7.0,
        .reload = 1.5,
        .count = 3,
        .rarity = RarityID::kEpic,
        .attributes = {
            .secondary_reload = 0.5,
            .defend_only = 1
        }
    },
    {
        .name = "Peas",
        .description = "4 in 1 deal, now with a secret ingredient: poison",
        .health = 5.0,
        .damage = 4.0,
        .radius = 7.0,
        .reload = 2.0,
        .count = 4,
        .rarity = RarityID::kEpic,
        .attributes = {
            .clump_radius = 8,
            .secondary_reload = 0.1,
            .defend_only = 1,
            .poison_damage = {
                .damage = 20.0,
                .time = 1.0
            }
        }
    },
    {
    .name = "Peas",
    .description = "Easy peasy",
    .health = 5.0,
    .damage = 4.0,
    .radius = 12.0,
    .reload = 2,
    .count = 4,
    .rarity = RarityID::kLegendary,
    .attributes = {
        .clump_radius = 10,
        .secondary_reload = 0.1,
        .defend_only = 1,
        .poison_damage = { 
            .damage = 35.0,
            .time = 1.0
        }
        }
    },
    {
        .name = "Egg",
        .description = "Something interesting might pop out of this",
        .health = 50.0,
        .damage = 1.0,
        .radius = 15.0,
        .reload = 1.0,
        .count = 1,
        .rarity = RarityID::kLegendary,
        .attributes = { 
            .secondary_reload = 2.5,
            .defend_only = 1,
            .rotation_style = PetalAttributes::kNoRot,
            .spawns = MobID::kBeetle
        }
    },
    {
        .name = "Rose",
        .description = "Extremely powerful rose, almost unheard of",
        .health = 5.0,
        .damage = 5.0,
        .radius = 10.0,
        .reload = 3.5,
        .count = 1,
        .rarity = RarityID::kEpic,
        .attributes = { 
            .secondary_reload = 1.0,
            .burst_heal = 22,
            .defend_only = 1
        }
    },
    {
        .name = "Stick",
        .description = "Harnesses the power of the wind",
        .health = 10.0,
        .damage = 1.0,
        .radius = 15.0,
        .reload = 3.0,
        .count = 1,
        .rarity = RarityID::kLegendary,
        .attributes = { 
            .secondary_reload = 4.0,
            .defend_only = 1,
            .icon_angle = 1,
            .spawns = MobID::kSandstorm,
            .spawn_count = 2
        }
    },
    {
        .name = "Stinger",
        .description = "It really hurts, but it's really fragile",
        .health = 5.0,
        .damage = 35.0,
        .radius = 7.0,
        .reload = 4.0,
        .count = 3,
        .rarity = RarityID::kLegendary,
        .attributes = {
            .clump_radius = 10
        }
    },
    {
        .name = "Web",
        .description = "It's really sticky",
        .health = 10.0,
        .damage = 5.0,
        .radius = 10.0,
        .reload = 3.0,
        .count = 3,
        .rarity = RarityID::kLegendary,
        .attributes = {
            .clump_radius = 10,
            .secondary_reload = 0.5,
            .defend_only = 1,
        }
    },
    {
        .name = "Antennae",
        .description = "Allows your flower to sense foes from farther away",
        .health = 0.0,
        .damage = 0.0,
        .radius = 12.5,
        .reload = 0.0,
        .count = 0,
        .rarity = RarityID::kLegendary,
        .attributes = { 
            .equipment = EquipmentFlags::kAntennae 
        }
    },
    // {
    //     .name = "Antennae",
    //     .description = "Allows your flower to sense foes from farther away",
    //     .health = 0.0,
    //     .damage = 0.0,
    //     .radius = 12.5,
    //     .reload = 0.0,
    //     .count = 0,
    //     .rarity = RarityID::kEpic,
    //     .attributes = { 
    //         .equipment = EquipmentFlags::kAntennae 
    //     }
    // },
    {
        .name = "Cactus",
        .description = "Not very strong, but somehow increases your maximum health",
        .health = 15.0,
        .damage = 5.0,
        .radius = 10.0,
        .reload = 1.0,
        .count = 3,
        .rarity = RarityID::kLegendary,
        .attributes = {
            .clump_radius = 10,
        }
    },
    {
        .name = "Heaviest",
        .description = "This thing is so heavy that nothing gets in the way",
        .health = 200.0,
        .damage = 10.0,
        .radius = 12.0,
        .reload = 15.0,
        .count = 1,
        .rarity = RarityID::kEpic,
        .attributes = {
            .mass = 5
        }
    },
    {
        .name = "Third Eye",
        .description = "Allows your flower to expand your petals further out",
        .health = 0.0,
        .damage = 0.0,
        .radius = 20.0,
        .reload = 0.0,
        .count = 0,
        .rarity = RarityID::kLegendary,
        .attributes = { 
            .equipment = EquipmentFlags::kThirdEye
        }
    },
    {
        .name = "Observer",
        .description = "The one who sees all",
        .health = 0.0,
        .damage = 0.0,
        .radius = 12.5,
        .reload = 0.0,
        .count = 0,
        .rarity = RarityID::kUnique,
        .attributes = { 
            .equipment = EquipmentFlags::kObserver
        }
    },
    {
        .name = "Cactus",
        .description = "Turns your flower poisonous. Enemies will take poison damage on contact",
        .health = 15.0,
        .damage = 5.0,
        .radius = 10.0,
        .reload = 1.0,
        .count = 1,
        .rarity = RarityID::kEpic,
        .attributes = {
            .poison_damage = {
                .damage = 1.0,
                .time = 5.0
            }
        }
    },
    {
        .name = "Salt",
        .description = "Reflects some damage dealt to the flower. Does not stack with itself",
        .health = 10.0,
        .damage = 10.0,
        .radius = 10.0,
        .reload = 2.5,
        .count = 1,
        .rarity = RarityID::kRare,
        .attributes = {}
    },
    {
        .name = "Basic",
        .description = "Something incredibly rare and useful",
        .health = 20.0,
        .damage = 20.0,
        .radius = 10.0,
        .reload = 2.5,
        .count = 1,
        .rarity = RarityID::kUnique,
        .attributes = {}
    },
    {
        .name = "Square",
        .description = "This shape... it looks familiar...",
        .health = 100.0,
        .damage = 0.0,
        .radius = 15.0,
        .reload = 2.5,
        .count = 1,
        .rarity = RarityID::kUnique,
        .attributes = { 
            .secondary_reload = 30.0,
            .defend_only = 1,
            .rotation_style = PetalAttributes::kNoRot,
            .spawns = MobID::kSquare,
            .icon_angle = M_PI / 4 + 1
        }
    },
    {
        .name = "Moon",
        .description = "Where did this come from?",
        .health = 500.0,
        .damage = 0.0,
        .radius = 70.0,
        .reload = 15.0,
        .count = 1,
        .rarity = RarityID::kUnique,
        .attributes = {
            .mass = 150
        }
    },
    {
        .name = "Lotus",
        .description = "Absorbs some poison damage taken by the flower",
        .health = 5.0,
        .damage = 5.0,
        .radius = 12.0,
        .reload = 2.0,
        .count = 1,
        .rarity = RarityID::kEpic,
        .attributes = {
            .icon_angle = 0.1
        }
    },
    {
        .name = "Cutter",
        .description = "Increases the flower's body damage",
        .health = 0.0,
        .damage = 0.0,
        .radius = 40.0,
        .reload = 0.0,
        .count = 0,
        .rarity = RarityID::kEpic,
        .attributes = { 
            .equipment = EquipmentFlags::kCutter 
        }
    },
    {
        .name = "Yin Yang",
        .description = "This mysterious petal reverses the rotation direction of your petals...",
        .health = 15.0,
        .damage = 10.0,
        .radius = 10.0,
        .reload = 1.0,
        .count = 1,
        .rarity = RarityID::kEpic,
        .attributes = {}
    },
    {
        .name = "Yggdrasil",
        .description = "A dried leaf from the Yggdrasil tree. Rumored to be able to bring the fallen back to life",
        .health = 1.0,
        .damage = 0.0,
        .radius = 12.0,
        .reload = 60.0,
        .count = 1,
        .rarity = RarityID::kUnique,
        .attributes = {
            .defend_only = 1,
            .icon_angle = M_PI
        }
    },
    {
        .name = "Rice",
        .description = "Spawns instantly, but not very strong",
        .health = 1.0,
        .damage = 4.0,
        .radius = 13.0,
        .reload = 0.05,
        .count = 1,
        .rarity = RarityID::kEpic,
        .attributes = {
            .icon_angle = 0.7
        }
    },
    {
        .name = "Bone",
        .description = "Sturdy",
        .health = 15.0,
        .damage = 8.0,
        .radius = 12.0,
        .reload = 2.5,
        .count = 1,
        .rarity = RarityID::kLegendary,
        .attributes = {
            .icon_angle = 1
        }
    },
    {
        .name = "Yucca",
        .description = "Heals the flower, but only while in the defensive position",
        .health = 10.0,
        .damage = 5.0,
        .radius = 10.0,
        .reload = 1.0,
        .count = 1,
        .rarity = RarityID::kUnusual,
        .attributes = {
            .constant_heal = 1.5,
            .icon_angle = -1
        }
    },
    {
        .name = "Corn", 
        .description = "Takes a long time to spawn, but has a lot of health",
        .health = 500.0,
        .damage = 2.5,
        .radius = 16.0,
        .reload = 10.0,
        .count = 1,
        .rarity = RarityID::kEpic,
        .attributes = {
            .icon_angle = 0.5,
        }
    },
    {
        .name = "Soil",
        .description = "The bigger, the better",
        .health = 10.0,
        .damage = 2.5,
        .radius = 12.0,
        .reload = 1.5,
        .count = 1,
        .rarity = RarityID::kEpic,
        .attributes = {}
    },
    {
        .name = "Basil",
        .description = "Increases all healing recieved harmonically",
        .health = 1.0,
        .damage = 0.0,
        .radius = 12.0,
        .reload = 0.0,
        .count = 1,
        .rarity = RarityID::kEpic,
        .attributes = {
            .icon_angle = 0.0,
            .defend_only = 1,
            .rotation_style = PetalAttributes::kNoRot
        }
    },
    {
        .name = "Honey",
        .description = "This thing might put you in a sticky situation",
        .health = 10.0,
        .damage = 8.0,
        .radius = 12.0,
        .reload = 2.0,
        .count = 1,
        .rarity = RarityID::kRare,
        .attributes = {
            .icon_angle = (float)(M_PI / 6)
        }
    },
    {
        .name = "Wax",
        .description = "This thing is so sticky that nothing gets in it's way",
        .health = 300.0,
        .damage = 1.0,
        .radius = 80.0,
        .reload = 15,
        .count = 1,
        .rarity = RarityID::kEpic,
        .attributes = {
            .secondary_reload = 0.01,
            .defend_only = 1,
            .mass = 50,
            .rotation_style = PetalAttributes::kFollowRot,
            .icon_angle = (float)(M_PI / 6)
        }
    },
    {
    .name = "Crown",
    .description = "Beetle",
    .health = 0.0,
    .damage = 0.0,
    .radius = 12.0,
    .reload = 0.0,
    .count = 0,
    .rarity = RarityID::kUnique,
    .attributes = {
        .equipment = EquipmentFlags::kCrown
        }
    },
}};

std::array<struct MobData, MobID::kNumMobs> const MOB_DATA = {{
    {
        .name = "Baby Ant",
        .description = "Weak and defenseless, but big dreams.",
        .rarity = RarityID::kCommon,
        .health = {10.0},
        .damage = 10.0,
        .radius = {14.0},
        .xp = 1,
        .drops = {
            PetalID::kLight, PetalID::kLeaf, PetalID::kTwin, PetalID::kRice, PetalID::kTriplet
        }, 
        .attributes = {}
    },
    {
        .name = "Worker Ant",
        .description = "It's temperamental, probably from working all the time.",
        .rarity = RarityID::kCommon,
        .health = {25.0},
        .damage = 10.0,
        .radius = {14.0},
        .xp = 3,
        .drops = {
            PetalID::kLight, PetalID::kLeaf, PetalID::kTwin, PetalID::kCorn, PetalID::kBone
        }, 
        .attributes = {}
    },
    {
        .name = "Soldier Ant",
        .description = "It's got wings and it's ready to use them.",
        .rarity = RarityID::kUnusual,
        .health = {40.0},
        .damage = 10.0,
        .radius = {14.0},
        .xp = 5,
        .drops = {
            PetalID::kTwin, PetalID::kIris, PetalID::kWing, PetalID::kFaster, PetalID::kTriplet, PetalID::kTriFaster
        }, 
        .attributes = {}
    },
    {
        .name = "Bee",
        .description = "It stings. Don't touch it.",
        .rarity = RarityID::kCommon,
        .health = {15.0},
        .damage = 50.0,
        .radius = {20.0},
        .xp = 4,
        .drops = {
            PetalID::kLight, PetalID::kStinger, PetalID::kTwin, PetalID::kWing, PetalID::kTriplet, PetalID::kWax
        },
        .attributes = {}
    },
    {
        .name = "Ladybug",
        .description = "Cute and harmless.",
        .rarity = RarityID::kCommon,
        .health = {25.0},
        .damage = 10.0,
        .radius = {30.0},
        .xp = 3,
        .drops = {
            PetalID::kLight, PetalID::kRose, PetalID::kTwin, PetalID::kBubble, PetalID::kDahlia
        },
        .attributes = {}
    },
    {
        .name = "Beetle",
        .description = "It's hungry and flowers are its favorite meal.",
        .rarity = RarityID::kUnusual,
        .health = {40.0},
        .damage = 35.0,
        .radius = {35.0},
        .xp = 10,
        .drops = {
            PetalID::kIris, PetalID::kSalt, PetalID::kWing, PetalID::kTriplet, PetalID::kBeetleEgg
        },
        .attributes = {}
    },
    {
        .name = "Massive Ladybug",
        .description = "Much larger, but still cute.",
        .rarity = RarityID::kEpic,
        .health = {1000.0},
        .damage = 10.0,
        .radius = {90.0},
        .xp = 400,
        .drops = {
            PetalID::kRose, PetalID::kDahlia, PetalID::kBubble, PetalID::kAzalea, PetalID::kObserver
        }, 
        .attributes = {}
    },
    {
        .name = "Massive Beetle",
        .description = "Someone overfed this one, you might be next.",
        .rarity = RarityID::kRare,
        .health = {600.0},
        .damage = 35.0,
        .radius = {75.0},
        .xp = 50,
        .drops = {
            PetalID::kIris, PetalID::kWing, PetalID::kBlueIris, PetalID::kTriplet, PetalID::kBeetleEgg, PetalID::kThirdEye
        }, 
        .attributes = { 
            .aggro_radius = 750
        }
    },
    {
        .name = "Ladybug",
        .description = "Cute and harmless... if left unprovoked.",
        .rarity = RarityID::kUnusual,
        .health = {35.0},
        .damage = 10.0,
        .radius = {30.0},
        .xp = 5,
        .drops = {
            PetalID::kDahlia, PetalID::kBubble, PetalID::kWing, PetalID::kYinYang, PetalID::kTriplet, PetalID::kAzalea
        },
        .attributes = {}
    },
        {
        .name = "Manbug",
        .description = "It's always provoked.",
        .rarity = RarityID::kRare,
        .health = {35.0},
        .damage = 10.0,
        .radius = {30.0},
        .xp = 25,
        .drops = {
        },
        .attributes = {
        }
    },
    {
        .name = "Massive Ladybug",
        .description = "Much larger, but still cute... if left unprovoked.",
        .rarity = RarityID::kLegendary,
        .health = {800.0, 1000.0},
        .damage = 10.0,
        .radius = {85.0, 95.0},
        .xp = 625,
        .drops = {
            PetalID::kBubble,
            PetalID::kYinYang,
            PetalID::kAzalea,
            PetalID::kObserver
        },
        .attributes = {
        }
    },
    {
        .name = "Hornet",
        .description = "These aren't quite as nice as the little bees.",
        .rarity = RarityID::kUnusual,
        .health = {40.0},
        .damage = 40.0,
        .radius = {40.0, 50.0},
        .xp = 12,
        .drops = {
            PetalID::kDandelion, PetalID::kMissile, PetalID::kWing,  PetalID::kHoney, /*PetalID::kEntennae,*/ PetalID::kAntennae
        },
        .attributes = {
            .aggro_radius = 600
        }
    },
    {
        .name = "Cactus",
        .description = "This one's prickly, don't touch it either.",
        .rarity = RarityID::kCommon,
        .health = {25.0, 50.0},
        .damage = 30.0,
        .radius = {30.0, 60.0},
        .xp = 2,
        .drops = {
            PetalID::kStinger, PetalID::kCactus, PetalID::kMissile, PetalID::kPoisonCactus, PetalID::kTricac
        },
        .attributes = {
            .stationary = 1
        }
    },
    {
        .name = "Rock",
        .description = "A rock. It doesn't do much.",
        .rarity = RarityID::kCommon,
        .health = {5.0, 15.0},
        .damage = 10.0,
        .radius = {25.0, 30.0},
        .xp = 1,
        .drops = {
            PetalID::kHeavy, PetalID::kLight, PetalID::kRock, PetalID::kHeaviest
        },
        .attributes = {
            .stationary = 1
        }
    },
    {
        .name = "Boulder",
        .description = "A bigger rock. It also doesn't do much.",
        .rarity = RarityID::kUnusual,
        .health = {40.0, 60.0},
        .damage = 10.0,
        .radius = {50.0, 75.0},
        .xp = 10,
        .drops = {
            PetalID::kHeavy, PetalID::kRock, PetalID::kHeaviest, PetalID::kMoon
        }, 
        .attributes = {
            .stationary = 1
        }
    },
    {
        .name = "Centipede",
        .description = "It's just there doing its thing.",
        .rarity = RarityID::kUnusual,
        .health = {50.0},
        .damage = 10.0,
        .radius = {35.0},
        .xp = 2,
        .drops = {
            PetalID::kLight, PetalID::kTwin, PetalID::kLeaf, PetalID::kPeas, PetalID::kPoisonPeas, PetalID::kTriplet
        },
        .attributes = {
            .segments = 10
        }
    },
    {
        .name = "Evil Centipede",
        .description = "This one loves flowers.",
        .rarity = RarityID::kRare,
        .health = {50.0},
        .damage = 10.0,
        .radius = {35.0},
        .xp = 3,
        .drops = {
            PetalID::kIris, PetalID::kPoisonPeas, PetalID::kBlueIris, PetalID::kFatPeas
        },
        .attributes = { 
            .segments = 10, 
            .poison_damage = {
                .damage = 5.0,
                .time = 2.0
            }
        }
    },
    {
        .name = "Desert Centipede",
        .description = "It doesn't like it when you interrupt its run.",
        .rarity = RarityID::kRare,
        .health= {50.0},
        .damage = 10.0,
        .radius = {35.0},
        .xp = 4,
        .drops = {
            PetalID::kSand, PetalID::kFaster, PetalID::kSalt, PetalID::kStick, PetalID::kTriFaster
        },
        .attributes = {
            .segments = 6
        }
    },
    {
        .name = "Sandstorm",
        .description = "Quite unpredictable.",
        .rarity = RarityID::kUnusual,
        .health = {30.0, 45.0},
        .damage = 40.0,
        .radius = {32.0, 48.0},
        .xp = 5,
        .drops = {
            PetalID::kSand, PetalID::kFaster, PetalID::kStick, PetalID::kTriFaster
        },
        .attributes = {}
    },
    {
        .name = "Scorpion",
        .description = "This one stings, now with poison.",
        .rarity = RarityID::kUnusual,
        .health = {35.0},
        .damage = 15.0,
        .radius = {35.0},
        .xp = 10,
        .drops = {
            PetalID::kIris, PetalID::kPincer, PetalID::kTriplet, PetalID::kLotus
        }, 
        .attributes = {
            .poison_damage = {
                .damage = 5.0,
                .time = 1.0
            }
        }
    },
    {
        .name = "Spider",
        .description = "Spooky.",
        .rarity = RarityID::kUnusual,
        .health = {35.0},
        .damage = 10.0,
        .radius = {15.0},
        .xp = 8,
        .drops = {
            PetalID::kStinger, PetalID::kWeb, PetalID::kFaster, PetalID::kTriweb, PetalID::kTriFaster
        },
        .attributes = { 
            .poison_damage = {
                .damage = 5.0,
                .time = 3.0
            }
        }
    },
    {
        .name = "Ant Hole",
        .description = "Ants go in, and come out. Can't explain that.",
        .rarity = RarityID::kRare,
        .health = {500.0},
        .damage = 10.0,
        .radius = {45.0},
        .xp = 25,
        .drops = {
            PetalID::kIris, PetalID::kWing, PetalID::kAntEgg, PetalID::kTriplet, PetalID::kSoil,
        },
        .attributes = {
            .stationary = 1 
        }
    },
    {
        .name = "Queen Ant",
        .description = "You must have done something really bad if she's chasing you.",
        .rarity = RarityID::kRare,
        .health = {350.0},
        .damage = 10.0,
        .radius = {25.0},
        .xp = 15,
        .drops = {
            PetalID::kTwin, PetalID::kIris, PetalID::kWing, PetalID::kAntEgg, PetalID::kTringer, PetalID::kBasil
        },
        .attributes = {
            .aggro_radius = 750
        }
    },
    {
        .name = "Ladybug",
        .description = "This one is shiny... I wonder what it could mean...",
        .rarity = RarityID::kEpic,
        .health = {25.0},
        .damage = 10.0,
        .radius = {30.0},
        .xp = 3,
        .drops = {
            PetalID::kDahlia, PetalID::kWing, PetalID::kBubble, PetalID::kYggdrasil
        },
        .attributes = {}
    },
    {
        .name = "Square",
        .description = "This shape... It looks familiar...",
        .rarity = RarityID::kUnique,
        .health = {20.0},
        .damage = 10.0,
        .radius = {40.0},
        .xp = 1,
        .drops = {
            PetalID::kSquare
        },
        .attributes = {
            .stationary = 1
        }
    },
    {
        .name = "Digger",
        .description = "Friend or foe? You'll never know...",
        .rarity = RarityID::kEpic,
        .health = {250.0},
        .damage = 25.0,
        .radius = {40.0},
        .xp = 1,
        .drops = {
            PetalID::kCutter
        },
        .attributes = {}
    },
    {
        .name = "Moraine",
        .description = "Older than time itself",
        .rarity = RarityID::kLegendary,
        .health = {5000.0},
        .damage = 5.0,
        .radius = {95.0},
        .xp = 250,
        .drops = {
            PetalID::kRock, PetalID::kHeaviest, PetalID::kMoon
        },
        .attributes = {}
    },
    {
        .name = "Fire Ant Soldier",
        .description = "This ant is on fireeeeeeeeee!",
        .rarity = RarityID::kUnusual,
        .health = {40.0},
        .damage = 20.0,
        .radius = {14.0},
        .xp = 10,
        .drops = {
            PetalID::kYucca, PetalID::kBone
        },
        .attributes = {}
    },
    {
        .name = "Fire Ant Burrow",
        .description = "What could be in here?",
        .rarity = RarityID::kRare,
        .health = {1.0},
        .damage = 15.0,
        .radius = {45.0},
        .xp = 0,
        .drops = {
            PetalID::kYucca, PetalID::kSoil
        },
        .attributes = { .stationary = 1 }
    },
}};

std::array<StaticArray<float, MAX_DROPS_PER_MOB>, MobID::kNumMobs> const MOB_DROP_CHANCES = [](){
    std::array<StaticArray<float, MAX_DROPS_PER_MOB>, MobID::kNumMobs> ret;
    double const RARITY_MULT[RarityID::kNumRarities] = {50000,15000,2500,100,10,2.5,1};
    double MOB_SPAWN_RATES[MobID::kNumMobs] = {0};
    double PETAL_AGGREGATE_DROPS[PetalID::kNumPetals] = {0};
    for (struct ZoneDefinition const &zone : MAP_DATA) {
        double total = 0;
        for (SpawnChance const &s : zone.spawns) total += s.chance;
        for (SpawnChance const &s : zone.spawns) {
            double base_chance = (s.chance * zone.drop_multiplier / total);
            MOB_SPAWN_RATES[s.id] += base_chance;
            if (s.id == MobID::kAntHole) {
                MOB_SPAWN_RATES[MobID::kDigger] += DIGGER_SPAWN_CHANCE * base_chance;
                for (auto const &spawn_wave : ANTHOLE_SPAWNS)
                    for (MobID::T spawn : spawn_wave)
                        MOB_SPAWN_RATES[spawn] += base_chance;
            }
        }
    }

    for (MobID::T id = 0; id < MobID::kNumMobs; ++id)
        for (PetalID::T const drop_id : MOB_DATA[id].drops) PETAL_AGGREGATE_DROPS[drop_id]++;

    double const BASE_NUM = MOB_SPAWN_RATES[MobID::kSquare];
    if (BASE_NUM <= 0) assert(!"Square mob must spawn in at least one zone");

    for (MobID::T id = 0; id < MobID::kNumMobs; ++id) {
        for (PetalID::T const drop_id : MOB_DATA[id].drops) {
            float chance = fclamp((BASE_NUM * RARITY_MULT[PETAL_DATA[drop_id].rarity]) / (PETAL_AGGREGATE_DROPS[drop_id] * MOB_SPAWN_RATES[id] * MOB_DATA[id].attributes.segments), 0, 1);
            ret[id].push(chance);
        }
    }
    return ret;
}();

uint32_t score_to_pass_level(uint32_t level) {
    return (uint32_t)(pow(1.06, level - 1) * level) + 3;
}

uint32_t score_to_level(uint32_t score) {
    uint32_t level = 1;
    while (level < MAX_LEVEL) {
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
    if (level > MAX_LEVEL) level = MAX_LEVEL;
    uint32_t ret = 5 + level / LEVELS_PER_EXTRA_SLOT;
    if (level == MAX_LEVEL) ++ret;
    if (ret > MAX_SLOT_COUNT) return MAX_SLOT_COUNT;
    return ret;
}

float hp_at_level(uint32_t level) {
    if (level > MAX_LEVEL) level = MAX_LEVEL;
    return BASE_HEALTH + level;
}

std::string name_or_unnamed(std::string const &name) {
    if (name.size() == 0) return "Unnamed Flower";
    return name;
}
