#include <Client/Game.hh>

#include <iostream>

static double g_last_time = 0;

static int _c = setup_canvas();
static int _i = setup_inputs();

namespace Game {
    Simulation simulation;
    Renderer renderer;
    Socket socket;
    Ui::Window window;
    EntityID camera_id;
    EntityID player_id;

    uint8_t loadout_count = 5;
    uint8_t simulation_ready = 0;
    uint8_t on_game_screen = 0;
}

using namespace Game;

void Game::init() {
    window.add_child(
        Ui::make_title_main_screen()
    );
    window.add_child(
        Ui::make_death_main_screen()
    );
    window.add_child(
        Ui::make_loadout_backgrounds()
    );
    window.add_child(
        Ui::make_level_bar()
    );
    for (uint8_t i = 0; i < MAX_SLOT_COUNT * 2; ++i) window.add_child(new Ui::UiLoadoutPetal(i));
    window.add_child(
        Ui::make_leaderboard()
    );
    socket.connect("ws://localhost:9001");
}

uint8_t Game::alive() {
    return simulation_ready
    && simulation.ent_exists(camera_id)
    && simulation.ent_alive(simulation.get_ent(camera_id).player);
}

uint8_t Game::in_game() {
    return simulation_ready && on_game_screen
    && simulation.ent_exists(camera_id);
}

void Game::tick(double time) {
    renderer.reset_transform();
    simulation.tick();
    simulation.tick_lerp(time - g_last_time);
    Ui::timestamp = time;
    Ui::dt = time - g_last_time;
    g_last_time = time;
    renderer.context.reset();
    renderer.reset_transform();
    renderer.round_line_cap();
    renderer.round_line_join();
    renderer.center_text_align();
    renderer.center_text_baseline();
    Ui::window_width = renderer.width;
    Ui::window_height = renderer.height;
    double a = Ui::window_width / 1920;
    double b = Ui::window_height / 1080;
    Ui::scale = a > b ? a : b;
    if (alive()) {
        on_game_screen = 1;
        player_id = simulation.get_ent(camera_id).player;
    } else {
        player_id = NULL_ENTITY;
    }
    if (in_game()) render_game();
    else render_title_screen();

    process_ui();
    
    if (socket.ready && alive()) send_inputs();

    //clearing operations
    simulation.post_tick();
    Input::keys_pressed_this_tick.clear();
    Input::mouse_buttons_pressed = Input::mouse_buttons_released = 0;
    Input::prev_mouse_x = Input::mouse_x;
    Input::prev_mouse_y = Input::mouse_y;

    //temp print arena
    /*
    for (uint32_t i = 0; i < simulation.arena_info.player_count; ++i) {
        std::printf("[%d]: %s-%.1f\n", i, simulation.arena_info.names[i].c_str(), (float) simulation.arena_info.scores[i]);
    }
        */
}