#pragma once

#include <Client/Render/Renderer.hh>
#include <Client/Input.hh>
#include <Client/Setup.hh>
#include <Client/Socket.hh>
#include <Client/Ui/Ui.hh>

#include <Shared/Simulation.hh>

namespace Game {
    extern Simulation simulation;
    extern Renderer renderer;
    extern Socket socket;
    extern Ui::Window window;
    extern EntityID camera_id;

    extern uint8_t loadout_count;
    extern uint8_t simulation_ready;
    extern uint8_t on_game_screen;
    
    void init();
    uint8_t alive();
    uint8_t in_game();
    void tick(double);
    void render_game();
    void render_title_screen();
    void process_ui();
    void send_inputs();
    void spawn_in();
    void delete_petal(uint8_t);
    void swap_petals(uint8_t, uint8_t);
    void on_message(uint8_t *, uint32_t);
};
