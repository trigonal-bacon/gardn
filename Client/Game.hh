#pragma once

#include <Client/Render/Renderer.hh>
#include <Client/Input.hh>
#include <Client/Socket.hh>
#include <Client/Ui/Ui.hh>

#include <Shared/Simulation.hh>

class Game {
public:
    Simulation simulation;
    Renderer renderer;
    Socket socket;
    EntityID camera_id;

    float scale = 1;
    uint8_t simulation_ready = 0;
    uint8_t on_game_screen = 0;
    
    Game();
    void init();
    uint8_t alive();
    uint8_t in_game();
    void tick(double);
    void render_game();
    void render_title_screen();
    //void render_ui();
    void send_inputs();
    void on_message(uint8_t *, uint32_t);
};

extern Game *game;
