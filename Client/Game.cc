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

    uint8_t simulation_ready = 0;
    uint8_t on_game_screen = 0;
}

using namespace Game;

void Game::init() {
    Ui::Element *title = new Ui::VContainer({
        new Ui::StaticText(40, "This is a game name", { .fill = 0xffffffff }),
        new Ui::Button(90, 40, new Ui::StaticText(30, "Enter"), [](uint8_t e){ if (e == Ui::kClick) spawn_in(); }, { .fill = 0xffcccccc })
    }, 10, 10, {});
    title->should_render = [](){
        return !alive() && on_game_screen == 0;
    };
    Ui::Element *continue_button = new Ui::Button(140, 40, new Ui::StaticText(30, "Continue"), [](uint8_t e){ if (e == Ui::kClick) on_game_screen = 0; }, { .fill = 0xffcccccc });
    continue_button->should_render = [](){
        return !alive() && on_game_screen;
    };
    window.add_child(
        title
    );
    window.add_child(
        continue_button
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
    Ui::dt = time - g_last_time;
    g_last_time = time;
    renderer.context.reset();
    renderer.reset_transform();
    Ui::window_width = renderer.width;
    Ui::window_height = renderer.height;
    double a = Ui::window_width / 1920;
    double b = Ui::window_height / 1080;
    Ui::scale = a > b ? a : b;
    if (alive()) on_game_screen = 1;
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
}