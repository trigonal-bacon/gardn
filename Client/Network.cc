#include <Client/Game.hh>

#include <Client/DOM.hh>
#include <Client/Ui/Ui.hh>

using namespace Game;

void Game::on_message(uint8_t *ptr, uint32_t len) {
    Reader reader(ptr);
    switch(reader.read<uint8_t>()) {
        case kClientbound::kClientUpdate: {
            simulation_ready = 1;
            camera_id = reader.read<EntityID>();
            EntityID curr_id = reader.read<EntityID>();
            while(!curr_id.null()) {
                assert(simulation.ent_exists(curr_id));
                Entity &ent = simulation.get_ent(curr_id);
                simulation._delete_ent(curr_id);
                curr_id = reader.read<EntityID>();
            }
            curr_id = reader.read<EntityID>();
            //DEBUG_ONLY(assert(!curr_id.null());)
            while(!curr_id.null()) {
                uint8_t create = reader.read<uint8_t>();
                if (create) simulation.force_alloc_ent(curr_id);
                DEBUG_ONLY(assert(simulation.ent_exists(curr_id));)
                Entity &ent = simulation.get_ent(curr_id);
                ent.read(&reader, create);
                curr_id = reader.read<EntityID>();
            }
            simulation.arena_info.read(&reader);
            break;
        }
        default:
            break;
    }
}

void Game::send_inputs() {
    uint8_t packet[64];
    Writer writer(static_cast<uint8_t *>(packet));
    writer.write<uint8_t>(kServerbound::kClientInput);
    if (Input::freeze_input) {
        writer.write<float>(0);
        writer.write<float>(0);
        writer.write<uint8_t>(0);
        return;
    }
    float x = (Input::mouse_x - renderer.width / 2) / Ui::scale;
    float y = (Input::mouse_y - renderer.height / 2) / Ui::scale;
    if (Input::keyboard_movement) {
        x = 300 * (Input::keys_pressed.contains('D') - Input::keys_pressed.contains('A') + Input::keys_pressed.contains(39) - Input::keys_pressed.contains(37));
        y = 300 * (Input::keys_pressed.contains('S') - Input::keys_pressed.contains('W') + Input::keys_pressed.contains(40) - Input::keys_pressed.contains(38));
    }
    writer.write<float>(x);
    writer.write<float>(y);
    uint8_t attack = Input::keys_pressed.contains('\x20') || BIT_AT(Input::mouse_buttons_state, Input::LeftMouse);
    uint8_t defend = Input::keys_pressed.contains('\x10') || BIT_AT(Input::mouse_buttons_state, Input::RightMouse);
    writer.write<uint8_t>(attack | (defend << 1));
    socket.send(writer.packet, writer.at - writer.packet);
}

void Game::spawn_in() {
    uint8_t packet[100];
    Writer writer(static_cast<uint8_t *>(packet));
    if (Game::alive()) return;
    if (Game::on_game_screen == 0) {
        writer.write<uint8_t>(kServerbound::kClientSpawn);
        std::string name = DOM::retrieve_text("t0", 16);
        writer.write<std::string>(name);
        socket.send(writer.packet, writer.at - writer.packet);
    }
}

void Game::delete_petal(uint8_t pos) {
    uint8_t packet[8];
    Writer writer(static_cast<uint8_t *>(packet));
    if (!Game::alive()) return;
    writer.write<uint8_t>(kServerbound::kPetalDelete);
    writer.write<uint8_t>(pos);
    socket.send(writer.packet, writer.at - writer.packet);
}

void Game::swap_petals(uint8_t pos1, uint8_t pos2) {
    uint8_t packet[8];
    Writer writer(static_cast<uint8_t *>(packet));
    if (!Game::alive()) return;
    writer.write<uint8_t>(kServerbound::kPetalSwap);
    writer.write<uint8_t>(pos1);
    writer.write<uint8_t>(pos2);
    socket.send(writer.packet, writer.at - writer.packet);
}