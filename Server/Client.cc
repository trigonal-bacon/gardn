#include <Server/Client.hh>

#include <Server/Game.hh>
#include <Server/PetalTracker.hh>
#include <Server/Server.hh>
#include <Server/Spawn.hh>

#include <Helpers/UTF8.hh>

#include <Shared/Binary.hh>
#include <Shared/Config.hh>
#include <Shared/StaticData.hh>

#include <array>
#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include <unordered_map>

constexpr std::array<uint32_t, RarityID::kNumRarities> RARITY_TO_XP = { 2, 10, 50, 200, 1000, 5000, 0 };

// Dev session state keyed by player.id.id (uint16_t)
static std::unordered_map<uint16_t, float> DEV_SPEED_MAP;  // default 5x if not set
static std::unordered_map<uint16_t, bool>  DEV_IMMUNE;     // default true     (toggle flips)
static std::unordered_map<uint16_t, bool>  DEV_TAG;        // default true     (toggle flips)

Client::Client() : game(nullptr) {}

void Client::init(uint64_t recovery_id) {
    DEBUG_ONLY(assert(game == nullptr);)
    Server::game.add_client(this, recovery_id);
}

void Client::remove() {
    if (game == nullptr) return;
    game->remove_client(this);
}

void Client::disconnect(int reason, std::string const &message) {
    if (ws == nullptr) return;
    remove();
    ws->end(reason, message);
}

uint8_t Client::alive() {
    if (game == nullptr) return false;
    Simulation *simulation = &game->simulation;
    return simulation->ent_exists(camera)
        && simulation->ent_exists(simulation->get_ent(camera).get_player());
}

static inline std::string trim_ws(std::string const &s) {
    size_t b = s.find_first_not_of(" \t\n\r");
    if (b == std::string::npos) return {};
    size_t e = s.find_last_not_of(" \t\n\r");
    return s.substr(b, e - b + 1);
}

static inline std::string normalize_token(std::string s) {
    std::string out; out.reserve(s.size());
    for (unsigned char c : s) if (std::isalnum(c)) out.push_back((char)std::tolower(c));
    return out;
}

static PetalID::T parse_petal_by_name(std::string key) {
    key = normalize_token(key);
    for (PetalID::T i = 0; i < PetalID::kNumPetals; ++i) {
        const char *nm = PETAL_DATA[i].name;
        if (nm && normalize_token(nm) == key) return i;
    }
    // common aliases
    if      (key == "tringer")      return PetalID::kTringer;
    else if (key == "triweb")       return PetalID::kTriweb;
    else if (key == "tricac")       return PetalID::kTricac;
    else if (key == "uniquebasic")  return PetalID::kUniqueBasic;
    else if (key == "blueiris")     return PetalID::kBlueIris;
    else if (key == "beetleegg")    return PetalID::kBeetleEgg;
    else if (key == "antegg")       return PetalID::kAntEgg;
    else if (key == "poisonpeas")   return PetalID::kPoisonPeas;
    else if (key == "poisoncactus") return PetalID::kPoisonCactus;
    else if (key == "thirdeye")     return PetalID::kThirdEye;
    else if (key == "yinyang")      return PetalID::kYinYang;
    return PetalID::kNone;
}

static MobID::T parse_mob_by_name(std::string key) {
    key = normalize_token(key);
    for (MobID::T i = 0; i < MobID::kNumMobs; ++i) {
        const char *nm = MOB_DATA[i].name;
        if (nm && normalize_token(nm) == key) return i;
    }
    return MobID::kNumMobs;
}

void Client::on_message(WebSocket *ws, std::string_view message, uint64_t /*code*/) {
    if (ws == nullptr) return;
    uint8_t const *data = reinterpret_cast<uint8_t const *>(message.data());
    Reader reader(data);
    Validator validator(data, data + message.size());
    Client *client = ws->getUserData();
    if (client == nullptr) {
        ws->end(CloseReason::kServer, "Server Error");
        return;
    }
    if (!client->verified) {
        if (client->check_invalid(
            validator.validate_uint8() &&
            validator.validate_uint64() &&
            validator.validate_uint64()
        )) return;
        if (reader.read<uint8_t>() != Serverbound::kVerify) {
            client->disconnect();
            return;
        }
        if (reader.read<uint64_t>() != VERSION_HASH) {
            client->disconnect(CloseReason::kOutdated, "Outdated Version");
            return;
        }
        client->verified = 1;
        client->init(reader.read<uint64_t>());
        return;
    }
    if (client->game == nullptr) {
        client->disconnect(CloseReason::kServer, "Server Error");
        return;
    }
    if (client->check_invalid(validator.validate_uint8())) return;

    switch (reader.read<uint8_t>()) {
        case Serverbound::kVerify:
            client->disconnect();
            return;

        case Serverbound::kClientInput: {
            if (!client->alive()) break;
            Simulation *simulation = &client->game->simulation;
            Entity &camera = simulation->get_ent(client->camera);
            Entity &player = simulation->get_ent(camera.get_player());
            if (client->check_invalid(
                validator.validate_float() &&
                validator.validate_float() &&
                validator.validate_uint8()
            )) return;

            float x = reader.read<float>();
            float y = reader.read<float>();
            if (x == 0 && y == 0) {
                player.acceleration.set(0, 0);
            } else {
                if (std::abs(x) > 5e3 || std::abs(y) > 5e3) break;
                Vector accel(x, y);
                float m = accel.magnitude();
                if (m > 200) accel.set_magnitude(PLAYER_ACCELERATION);
                else accel.set_magnitude(m / 200 * PLAYER_ACCELERATION);

                // Dev speed multiplier; keep immunity fresh if toggled
                if (player.get_dev()) {
                    // speed
                    float mul = 5.0f;
                    auto it = DEV_SPEED_MAP.find(player.id.id);
                    if (it != DEV_SPEED_MAP.end()) mul = it->second;
                    float baseMag = accel.magnitude();
                    accel.set_magnitude(baseMag * mul);
                    // immunity (refresh)
                    auto itI = DEV_IMMUNE.find(player.id.id);
                    bool immune = (itI == DEV_IMMUNE.end()) ? true : itI->second;
                    if (immune) player.immunity_ticks = TPS; // refresh a second
                }
                player.acceleration = accel;
            }
            player.input = reader.read<uint8_t>();
            break;
        }

        case Serverbound::kClientSpawn: {
            if (client->alive()) break;
            std::string name, pwd;
            if (client->check_invalid(validator.validate_string(MAX_NAME_LENGTH))) return;
            if (client->check_invalid(validator.validate_string(MAX_DEV_PWD_LENGTH))) return;
            reader.read<std::string>(name);
            reader.read<std::string>(pwd);
            if (client->check_invalid(UTF8Parser::is_valid_utf8(name))) return;
            if (client->check_invalid(UTF8Parser::is_valid_utf8(pwd))) return;

            Simulation *simulation = &client->game->simulation;
            Entity &camera = simulation->get_ent(client->camera);
            Entity &player = alloc_player(simulation, camera.get_team());
            player_spawn(simulation, camera, player);
            player.set_name(name);

            // dev auth (keep exact padded string used in your repo)
            uint8_t dev = (pwd == "ez hax       ");
            camera.set_dev(dev);
            player.set_dev(dev);

            // default dev state for new session
            if (dev) {
                DEV_SPEED_MAP[player.id.id] = 5.0f;
                DEV_IMMUNE[player.id.id] = true;
                DEV_TAG[player.id.id] = true;
            }

            std::cout << "player_spawn" << (dev ? "_dev " : " ") << name_or_unnamed(name)
                      << " <" << +player.id.hash << "," << +player.id.id << ">\n";
            break;
        }

        case Serverbound::kPetalDelete: {
            if (!client->alive()) break;
            Simulation *simulation = &client->game->simulation;
            Entity &camera = simulation->get_ent(client->camera);
            Entity &player = simulation->get_ent(camera.get_player());
            if (client->check_invalid(validator.validate_uint8())) return;

            uint8_t pos = reader.read<uint8_t>();
            if (pos >= MAX_SLOT_COUNT + player.get_loadout_count()) break;

            PetalID::T old_id = player.get_loadout_ids(pos);
            if (old_id != PetalID::kNone && old_id != PetalID::kBasic) {
                uint8_t rarity = PETAL_DATA[old_id].rarity;
                player.set_score(player.get_score() + RARITY_TO_XP[rarity]);
                if (player.deleted_petals.size() == player.deleted_petals.capacity())
                    PetalTracker::remove_petal(simulation, player.deleted_petals[0]);
                player.deleted_petals.push_back(old_id);
            }
            player.set_loadout_ids(pos, PetalID::kNone);
            break;
        }

        case Serverbound::kPetalSwap: {
            if (!client->alive()) break;
            Simulation *simulation = &client->game->simulation;
            Entity &camera = simulation->get_ent(client->camera);
            Entity &player = simulation->get_ent(camera.get_player());
            if (client->check_invalid(validator.validate_uint8() && validator.validate_uint8())) return;

            uint8_t pos1 = reader.read<uint8_t>();
            if (pos1 >= MAX_SLOT_COUNT + player.get_loadout_count()) break;
            uint8_t pos2 = reader.read<uint8_t>();
            if (pos2 >= MAX_SLOT_COUNT + player.get_loadout_count()) break;

            PetalID::T tmp = player.get_loadout_ids(pos1);
            player.set_loadout_ids(pos1, player.get_loadout_ids(pos2));
            player.set_loadout_ids(pos2, tmp);
            break;
        }

        case Serverbound::kChatSend: {
            if (!client->alive()) break;
            std::string text;
            if (client->check_invalid(validator.validate_string(MAX_CHAT_LENGTH))) return;
            reader.read<std::string>(text);
            if (client->check_invalid(UTF8Parser::is_valid_utf8(text))) return;
            if (text.size() == 0) break;

            Simulation *simulation = &client->game->simulation;
            Entity &camera = simulation->get_ent(client->camera);
            Entity &player = simulation->get_ent(camera.get_player());

            if (player.get_dev() && text.size() > 1 && text[0] == '/') {
                std::string line = trim_ws(text.substr(1));
                std::string lower = line; for (auto &c : lower) c = (char)std::tolower((unsigned char)c);

                // /give <petal>
                if (lower.rfind("give", 0) == 0) {
                    std::string argRaw = trim_ws(line.substr(4));
                    if (!argRaw.empty()) {
                        PetalID::T give_id = parse_petal_by_name(argRaw);
                        if (give_id != PetalID::kNone && give_id < PetalID::kNumPetals) {
                            uint32_t total = player.get_loadout_count() + MAX_SLOT_COUNT;
                            uint32_t pos = total;
                            for (uint32_t i = player.get_loadout_count(); i < total; ++i)
                                if (player.get_loadout_ids(i) == PetalID::kNone) { pos = i; break; }
                            if (pos == total)
                                for (uint32_t i = 0; i < total; ++i)
                                    if (player.get_loadout_ids(i) == PetalID::kNone) { pos = i; break; }
                            if (pos == total) pos = player.get_loadout_count();

                            PetalTracker::add_petal(simulation, give_id);
                            player.set_loadout_ids(pos, give_id);
                            if (pos < player.get_loadout_count()) {
                                LoadoutSlot &slot = player.loadout[pos];
                                slot.update_id(simulation, give_id);
                                slot.force_reload();
                            }
                            std::string ok = std::string("gave ") + PETAL_DATA[give_id].name;
                            if (player.chat_sent == NULL_ENTITY)
                                player.chat_sent = alloc_chat(simulation, ok, player).id;
                            break;
                        } else {
                            std::string err = std::string("unknown petal: ") + argRaw;
                            if (player.chat_sent == NULL_ENTITY)
                                player.chat_sent = alloc_chat(simulation, err, player).id;
                            break;
                        }
                    }
                }
                // /speed <multiplier>
                else if (lower.rfind("speed", 0) == 0) {
                    std::string argRaw = trim_ws(line.substr(5));
                    float mul = 0.0f; try { mul = std::stof(argRaw); } catch (...) { mul = 0.0f; }
                    if (mul <= 0.0f) mul = 1.0f;
                    if (mul > 100.0f) mul = 100.0f;
                    DEV_SPEED_MAP[player.id.id] = mul;
                    std::string ok = std::string("speed set to ") + std::to_string(mul) + "x";
                    if (player.chat_sent == NULL_ENTITY)
                        player.chat_sent = alloc_chat(simulation, ok, player).id;
                    break;
                }
                // /toggle  -> flip immunity and "dev tag"
                else if (lower.rfind("toggle", 0) == 0) {
                    bool curImm = DEV_IMMUNE.count(player.id.id) ? DEV_IMMUNE[player.id.id] : true;
                    bool curTag = DEV_TAG.count(player.id.id) ? DEV_TAG[player.id.id] : true;
                    DEV_IMMUNE[player.id.id] = !curImm;
                    DEV_TAG[player.id.id]    = !curTag;
                    std::string ok = std::string("dev: immunity=") + (DEV_IMMUNE[player.id.id]?"on":"off")
                                   + ", tag=" + (DEV_TAG[player.id.id]?"on":"off");
                    if (player.chat_sent == NULL_ENTITY)
                        player.chat_sent = alloc_chat(simulation, ok, player).id;
                    break;
                }
                // /spawn <mob>  (spawn a mob at player position, no drops)
                else if (lower.rfind("spawn", 0) == 0) {
                    std::string argRaw = trim_ws(line.substr(5));
                    if (!argRaw.empty()) {
                        MobID::T mob_id = parse_mob_by_name(argRaw);
                        if (mob_id < MobID::kNumMobs) {
                            Entity &mob = alloc_mob(simulation, mob_id, player.get_x(), player.get_y(), player.get_team());
                            BitMath::set(mob.flags, EntityFlags::kNoDrops);
                            mob.set_parent(player.id);
                            mob.set_color(player.get_color());
                            mob.base_entity = player.id;
                            std::string ok = std::string("spawned mob: ") + MOB_DATA[mob_id].name;
                            if (player.chat_sent == NULL_ENTITY)
                                player.chat_sent = alloc_chat(simulation, ok, player).id;
                            break;
                        } else {
                            std::string err = std::string("unknown mob: ") + argRaw;
                            if (player.chat_sent == NULL_ENTITY)
                                player.chat_sent = alloc_chat(simulation, err, player).id;
                            break;
                        }
                    }
                }
                // /announcement <text>  (broadcast a chat bubble to all players)
                else if (lower.rfind("announcement", 0) == 0) {
                    std::string msg = trim_ws(line.substr(12));
                    if (msg.empty()) msg = "Announcement";
                    // Broadcast: attach a chat bubble to every player's flower
                    simulation->for_each<kCamera>([&](Simulation *sim, Entity &cam){
                        if (!sim->ent_alive(cam.get_player())) return;
                        Entity &pl = sim->get_ent(cam.get_player());
                        if (pl.chat_sent == NULL_ENTITY) {
                            pl.chat_sent = alloc_chat(sim, msg, pl).id;
                        }
                    });
                    break;
                }
            }

            if (player.chat_sent != NULL_ENTITY) break;
            player.chat_sent = alloc_chat(simulation, text, player).id;
            std::cout << "chat " << name_or_unnamed(player.get_name()) << ": " << text << std::endl;
            break;
        }
    }
}

void Client::on_disconnect(WebSocket *ws, int code, std::string_view /*message*/) {
    std::printf("disconnect: [%d]\n", code);
    Client *client = ws->getUserData();
    if (client == nullptr) return;
    client->remove();
}

bool Client::check_invalid(bool valid) {
    if (valid) return false;
    std::cout << "client sent an invalid packet\n";
    disconnect();
    return true;
}