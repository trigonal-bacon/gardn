#pragma once

#include <Client/Render/Renderer.hh>
#include <Client/Input.hh>
#include <Client/Socket.hh>
#include <Client/Ui/Ui.hh>
#include <Client/StaticData.hh>

#include <Shared/Simulation.hh>

#include <array>

namespace Game {
    extern Simulation simulation;
    extern Renderer renderer;
    extern Renderer game_ui_renderer;
    extern Socket socket;
    extern Ui::Window title_ui_window;
    extern Ui::Window game_ui_window;
    extern Ui::Window other_ui_window;
    extern EntityID camera_id;
    extern EntityID player_id;
    extern std::string nickname;
    extern std::array<uint8_t, PetalID::kNumPetals> seen_petals;
    extern std::array<uint8_t, MobID::kNumMobs> seen_mobs;
    
    extern double timestamp;
    
    extern double score;
    extern float overlevel_timer;
    extern float slot_indicator_opacity;
    extern float transition_circle;

    extern uint32_t respawn_level;

    extern std::array<PetalID::T, 2 * MAX_SLOT_COUNT> cached_loadout;

    extern uint8_t loadout_count;
    extern uint8_t simulation_ready;
    extern uint8_t on_game_screen;
    extern uint8_t show_debug;
    extern uint8_t is_mobile;
    
    void init();
    void reset();
    uint8_t alive();
    uint8_t in_game();
    uint8_t should_render_title_ui();
    uint8_t should_render_game_ui();
    void tick(double);
    void render_game();
    void render_title_screen();
    void send_inputs();
    void spawn_in();
    void delete_petal(uint8_t);
    void swap_petals(uint8_t, uint8_t);
    void swap_all_petals();
    void on_message(uint8_t *, uint32_t);
};
