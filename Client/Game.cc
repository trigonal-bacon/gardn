#include <Client/Game.hh>

#include <iostream>

static double g_last_time = 0;
static const float max_transition_circle = 2500;

static int _c = setup_canvas();
static int _i = setup_inputs();

namespace Game {
    Simulation simulation;
    Renderer renderer;
    Socket socket;
    Ui::Window window;
    EntityID camera_id;
    EntityID player_id;
    Vector screen_shake;

    double timestamp = 0;

    float score = 0;
    float slot_indicator_opacity = 0;
    float transition_circle = 0;

    uint8_t cached_loadout[2 * MAX_SLOT_COUNT] = {PetalID::kNone};

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
        Ui::make_panel_buttons()
    );
    window.add_child(
        Ui::make_settings_panel()
    );
    window.add_child(
        Ui::make_petal_gallery()
    );
    window.add_child(
        Ui::make_mob_gallery()
    );
    window.set_divider();
    window.add_child(
        Ui::make_death_main_screen()
    );
    window.add_child(
        Ui::make_level_bar()
    );
    window.add_child(
        Ui::make_minimap()
    );
    window.add_child(
        Ui::make_loadout_backgrounds()
    );
    for (uint8_t i = 0; i < MAX_SLOT_COUNT * 2; ++i) window.add_child(new Ui::UiLoadoutPetal(i));
    window.add_child(
        Ui::make_leaderboard()
    );
    window.add_child(
        Ui::make_stat_screen()
    );
    Ui::make_petal_tooltips();
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

uint8_t Game::should_render_title_ui() {
    return transition_circle < max_transition_circle;
}

uint8_t Game::should_render_game_ui() {
    return transition_circle > 0 && simulation_ready && simulation.ent_exists(camera_id);
}

void Game::tick(double time) {
    renderer.reset_transform();
    simulation.tick();
    simulation.tick_lerp(time - g_last_time);
    Game::timestamp = time;
    Ui::dt = time - g_last_time;
    Ui::lerp_amount = 1 - pow(1 - 0.2, Ui::dt * 60 / 1000);
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
        Entity const &player = simulation.get_ent(player_id);
        for (uint32_t i = 0; i < 2 * MAX_SLOT_COUNT; ++i) 
            cached_loadout[i] = player.loadout_ids[i];
        score = player.score;
    } else {
        player_id = NULL_ENTITY;
    }

    if (in_game()) {
        transition_circle = fclamp(transition_circle * 1.05 + 5, 0, max_transition_circle);
        //transition_circle = fclamp(transition_circle + 100, 0, 2500);
    } else {
        transition_circle = fclamp(transition_circle / 1.05 - 5, 0, max_transition_circle);
        //transition_circle = fclamp(transition_circle - 100, 0, 2500);
    }

    window.refactor();
    window.poll_events();

    if (should_render_title_ui()) {
        render_title_screen();
        window.render_title_screen(renderer);
    }

    if (should_render_game_ui() && should_render_title_ui()) {
        renderer.set_stroke(0xff222222);
        renderer.set_line_width(Ui::scale * 10);
        renderer.begin_path();
        renderer.arc(renderer.width / 2, renderer.height / 2, transition_circle);
        renderer.stroke();
        renderer.clip();
    }

    if (should_render_game_ui()) {
        render_game();
        if (!Game::alive()) {
            RenderContext c(&renderer);
            renderer.reset_transform();
            renderer.set_fill(0x20000000);
            renderer.fill_rect(0,0,renderer.width,renderer.height);
        }
        window.render_game_screen(renderer);
        if (Input::keys_pressed_this_tick.contains('E')) {
            Ui::advance_key_select();
        }
        if (Ui::UiLoadout::selected_with_keys < MAX_SLOT_COUNT) {
            if (Input::keys_pressed_this_tick.contains('T')) {
                Ui::ui_delete_petal(Ui::UiLoadout::selected_with_keys + Game::loadout_count);
                Ui::advance_key_select();
            } else {
                for (uint8_t i = 0; i < Game::loadout_count; ++i) {
                    if (Input::keys_pressed_this_tick.contains('1' + i)) {
                        Ui::ui_swap_petals(i, Ui::UiLoadout::selected_with_keys + Game::loadout_count);
                        //Ui::advance_key_select();
                        break;
                    }
                }
            }
        }
    } else {
        Ui::UiLoadout::selected_with_keys = MAX_SLOT_COUNT;
    }
    if (Game::timestamp - Ui::UiLoadout::last_key_select > 5000)
        Ui::UiLoadout::selected_with_keys = MAX_SLOT_COUNT;
    LERP(slot_indicator_opacity, Ui::UiLoadout::selected_with_keys != MAX_SLOT_COUNT, Ui::lerp_amount);

    window.on_render_tooltip(renderer);
    window.tick_render_skip(renderer);

    //no rendering past this point

    if (socket.ready && alive()) send_inputs();

    //clearing operations
    simulation.post_tick();
    Input::keys_pressed_this_tick.clear();
    Input::mouse_buttons_pressed = Input::mouse_buttons_released = 0;
    Input::prev_mouse_x = Input::mouse_x;
    Input::prev_mouse_y = Input::mouse_y;
    Input::wheel_delta = 0;
}