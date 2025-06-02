#include <Client/Game.hh>

#include <Client/DOM.hh>
#include <Client/Ui/Ui.hh>

#include <iostream>

using namespace Game;

void Game::on_message(uint8_t *ptr, uint32_t len) {
    Reader reader(ptr);
    switch(reader.read_uint8()) {
        case kClientbound::kClientUpdate: {
            simulation_ready = 1;
            camera_id = reader.read_entid();
            EntityID curr_id = reader.read_entid();
            while(1) {
                if (curr_id.null()) break;
                assert(simulation.ent_exists(curr_id));
                Entity &ent = simulation.get_ent(curr_id);
                simulation.delete_ent(curr_id);
                curr_id = reader.read_entid();
            }
            curr_id = reader.read_entid();
            DEBUG_ONLY(assert(!curr_id.null());)
            while(1) {
                if (curr_id.null()) break;
                uint8_t create = reader.read_uint8();
                if (create) simulation.force_alloc_ent(curr_id);
                DEBUG_ONLY(assert(simulation.ent_exists(curr_id));)
                Entity &ent = simulation.get_ent(curr_id);
                ent.read(&reader);
                curr_id = reader.read_entid();
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
    writer.write_uint8(kServerbound::kClientInput);
    //float x = Input::keys_pressed.contains('D') - Input::keys_pressed.contains('A');
    //float y = Input::keys_pressed.contains('S') - Input::keys_pressed.contains('W');
    if (Input::freeze_input) {
        writer.write_float(0);
        writer.write_float(0);
        writer.write_uint8(0);
        return;
    }
    float x = (Input::mouse_x - renderer.width / 2) / Ui::scale;
    float y = (Input::mouse_y - renderer.height / 2) / Ui::scale;
    writer.write_float(x);
    writer.write_float(y);
    uint8_t attack = Input::keys_pressed.contains('\x20') || BIT_AT(Input::mouse_buttons_state, Input::LeftMouse);
    uint8_t defend = Input::keys_pressed.contains('\x10') || BIT_AT(Input::mouse_buttons_state, Input::RightMouse);
    writer.write_uint8(attack | (defend << 1));
    socket.send(writer.packet, writer.at - writer.packet);
}

void Game::spawn_in() {
    uint8_t packet[100];
    Writer writer(static_cast<uint8_t *>(packet));
    if (Game::alive()) return;
    if (Game::on_game_screen == 0) {
        writer.write_uint8(kServerbound::kClientSpawn);
        std::string name = DOM::retrieve_text("t0", 16);
        std::cout << name << '\n';
        writer.write_string(name);
        socket.send(writer.packet, writer.at - writer.packet);
    }
}

void Game::delete_petal(uint8_t pos) {
    uint8_t packet[8];
    Writer writer(static_cast<uint8_t *>(packet));
    if (!Game::alive()) return;
    writer.write_uint8(kServerbound::kPetalDelete);
    writer.write_uint8(pos);
    socket.send(writer.packet, writer.at - writer.packet);
}

void Game::swap_petals(uint8_t pos1, uint8_t pos2) {
    uint8_t packet[8];
    Writer writer(static_cast<uint8_t *>(packet));
    if (!Game::alive()) return;
    writer.write_uint8(kServerbound::kPetalSwap);
    writer.write_uint8(pos1);
    writer.write_uint8(pos2);
    socket.send(writer.packet, writer.at - writer.packet);
}