#include <Server/Client.hh>

#include <Server/Game.hh>
#include <Server/PetalTracker.hh>
#include <Server/Server.hh>
#include <Server/Spawn.hh>

#include <Helpers/UTF8.hh>

#include <Shared/Binary.hh>
#include <Shared/Config.hh>

#include <array>
#include <iostream>
#include <cctype>

constexpr std::array<uint32_t, RarityID::kNumRarities> RARITY_TO_XP = { 2, 10, 50, 200, 1000, 5000, 0 };

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

void Client::on_message(WebSocket *ws, std::string_view message, uint64_t code) {
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
            if (x == 0 && y == 0) player.acceleration.set(0,0);
            else {
                if (std::abs(x) > 5e3 || std::abs(y) > 5e3) break;
                Vector accel(x,y);
                float m = accel.magnitude();
                if (m > 200) accel.set_magnitude(PLAYER_ACCELERATION);
                else accel.set_magnitude(m / 200 * PLAYER_ACCELERATION);
                // 5x speed for dev players
                if (player.get_dev()) {
                    accel.set_magnitude(accel.magnitude() * 5.0f);
                }
                player.acceleration = accel;
            }
            player.input = reader.read<uint8_t>();
            break;
        }
        case Serverbound::kClientSpawn: {
            if (client->alive()) break;
            //check string length
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
            uint8_t dev = pwd == "ez hax       "; // ez hax 
            camera.set_dev(dev);
            player.set_dev(dev);
            std::cout << "player_spawn" << (dev ? "_dev " : " ") << name_or_unnamed(name)
                << " <" << +player.id.hash << "," << +player.id.id << ">" << std::endl;
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
                //need to delete if over cap
                if (player.deleted_petals.size() == player.deleted_petals.capacity())
                    //removes old trashed old petal
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

            // Dev-only commands via chat: "/give <petal name>"
if (player.get_dev() && !text.empty() && text[0] == '/') {
                auto trim = [](const std::string &s){
        size_t b = s.find_first_not_of(" \t\n\r");
        if (b == std::string::npos) return std::string();
                    size_t e = s.find_last_not_of(" \t\n\r");
        return s.substr(b, e - b + 1);
    };
                auto normalize = [](std::string s){
                    std::string out; out.reserve(s.size());
                    for (unsigned char c : s) if (std::isalnum(c)) out.push_back((char)std::tolower(c));
                    return out;
        };

                std::string line = trim(text.substr(1)); // strip '/'
                std::string lower = line; for (auto &c : lower) c = (char)std::tolower((unsigned char)c);
                if (lower.rfind("give", 0) == 0) {
                    std::string argRaw = trim(line.substr(4));
                    if (!argRaw.empty()) {
                        std::string key = normalize(argRaw);
                        PetalID::T give_id = PetalID::kNone;

                        // Try display names from PETAL_DATA (case/space-insensitive)
                        for (PetalID::T i = 0; i < PetalID::kNumPetals; ++i) {
                            const char *nm = PETAL_DATA[i].name;
                            if (nm && normalize(nm) == key) { give_id = i; break; }
                        }

                        // Common alias fallbacks for code-style names (extend as needed)
                        if (give_id == PetalID::kNone) {
                            if      (key == "tringer")        give_id = PetalID::kTringer;
                            else if (key == "triweb")         give_id = PetalID::kTriweb;
                            else if (key == "tricac")         give_id = PetalID::kTricac;
                            else if (key == "uniquebasic")    give_id = PetalID::kUniqueBasic;
                            else if (key == "blueiris")       give_id = PetalID::kBlueIris;
                            else if (key == "beetleegg")      give_id = PetalID::kBeetleEgg;
                            else if (key == "antegg")         give_id = PetalID::kAntEgg;
                            else if (key == "poisonpeas")     give_id = PetalID::kPoisonPeas;
                            else if (key == "poisoncactus")   give_id = PetalID::kPoisonCactus;
                            else if (key == "thirdeye")       give_id = PetalID::kThirdEye;
                            else if (key == "yinyang")        give_id = PetalID::kYinYang;
                        }

                        if (give_id != PetalID::kNone && give_id < PetalID::kNumPetals) {
                            // Find a slot: prefer reserve slots first, then any empty, else overwrite first reserve
                            uint32_t total = player.get_loadout_count() + MAX_SLOT_COUNT;
                            uint32_t pos = total; // invalid
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

                            // Feedback chat (do not echo the original command)
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
            }

            if (player.chat_sent != NULL_ENTITY) break;
            player.chat_sent = alloc_chat(simulation, text, player).id;
            std::cout << "chat " << name_or_unnamed(player.get_name()) << ": " << text << std::endl;
            break;
        }
    }
}

void Client::on_disconnect(WebSocket *ws, int code, std::string_view message) {
    std::printf("disconnect: [%d]\n", code);
    Client *client = ws->getUserData();
    if (client == nullptr) return;
    client->remove();
}

bool Client::check_invalid(bool valid) {
    if (valid) return false;
    std::cout << "client sent an invalid packet\n";
    //optional
    disconnect();

    return true;
}