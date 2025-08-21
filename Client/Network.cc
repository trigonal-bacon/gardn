#include <Client/Game.hh>

#include <Client/Ui/Ui.hh>

#include <Shared/Binary.hh>
#include <Shared/Config.hh>

using namespace Game;

void Game::on_message(uint8_t *ptr, uint32_t len) {
    Reader reader(ptr);
    switch(reader.read<uint8_t>()) {
        case Clientbound::kClientUpdate: {
            simulation_ready = 1;
            camera_id = reader.read<EntityID>();
            EntityID curr_id = reader.read<EntityID>();
            while(!(curr_id == NULL_ENTITY)) {
                assert(simulation.ent_exists(curr_id));
                Entity &ent = simulation.get_ent(curr_id);
                simulation._delete_ent(curr_id);
                curr_id = reader.read<EntityID>();
            }
            curr_id = reader.read<EntityID>();
            while(!(curr_id == NULL_ENTITY)) {
                uint8_t create = reader.read<uint8_t>();
                if (BIT_AT(create, 0)) simulation.force_alloc_ent(curr_id);
                assert(simulation.ent_exists(curr_id));
                Entity &ent = simulation.get_ent(curr_id);
                ent.read(&reader, BIT_AT(create, 0));
                if (BIT_AT(create, 1)) ent.pending_delete = 1;
                curr_id = reader.read<EntityID>();
            }
            simulation.arena_info.read(&reader, reader.read<uint8_t>());
            break;
        }
        default:
            break;
    }
}

void Game::send_inputs() {
    uint8_t packet[64];
    Writer writer(static_cast<uint8_t *>(packet));
    writer.write<uint8_t>(Serverbound::kClientInput);
    if (Input::freeze_input) {
        writer.write<float>(0);
        writer.write<float>(0);
        writer.write<uint8_t>(0);
    } else {
        float x, y;
        if (Input::keyboard_movement) {
            x = 300 * (Input::keys_held.contains('D') - Input::keys_held.contains('A') + Input::keys_held.contains(39) - Input::keys_held.contains(37));
            y = 300 * (Input::keys_held.contains('S') - Input::keys_held.contains('W') + Input::keys_held.contains(40) - Input::keys_held.contains(38));
        } else {
           x = (Input::mouse_x - renderer.width / 2) / Ui::scale;
           y = (Input::mouse_y - renderer.height / 2) / Ui::scale;
        }
        writer.write<float>(x);
        writer.write<float>(y);
        uint8_t attack = Input::keys_held.contains(' ') || BIT_AT(Input::mouse_buttons_state, Input::LeftMouse);
        uint8_t defend = Input::keys_held.contains('\x10') || BIT_AT(Input::mouse_buttons_state, Input::RightMouse);
        writer.write<uint8_t>((attack << InputFlags::kAttacking) | (defend << InputFlags::kDefending));
    }
    socket.send(writer.packet, writer.at - writer.packet);
}

void Game::spawn_in() {
    uint8_t packet[100];
    Writer writer(static_cast<uint8_t *>(packet));
    if (Game::alive()) return;
    if (Game::on_game_screen == 0) {
        writer.write<uint8_t>(Serverbound::kClientSpawn);
        std::string name = Game::nickname;
        writer.write<std::string>(name);
        socket.send(writer.packet, writer.at - writer.packet);
    } else Game::on_game_screen = 0;
}

void Game::delete_petal(uint8_t pos) {
    uint8_t packet[8];
    Writer writer(static_cast<uint8_t *>(packet));
    if (!Game::alive()) return;
    writer.write<uint8_t>(Serverbound::kPetalDelete);
    writer.write<uint8_t>(pos);
    socket.send(writer.packet, writer.at - writer.packet);
}

void Game::swap_petals(uint8_t pos1, uint8_t pos2) {
    uint8_t packet[8];
    Writer writer(static_cast<uint8_t *>(packet));
    if (!Game::alive()) return;
    writer.write<uint8_t>(Serverbound::kPetalSwap);
    writer.write<uint8_t>(pos1);
    writer.write<uint8_t>(pos2);
    socket.send(writer.packet, writer.at - writer.packet);
}

void Game::swap_all_petals() {
    if (!Game::alive()) return;
    for (uint32_t i = 0; i < Game::loadout_count; ++i)
        Ui::ui_swap_petals(i, i + Game::loadout_count);
}