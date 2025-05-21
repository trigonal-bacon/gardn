#include <Client/Game.hh>

int main() {
    //setup_canvas();
    //setup_inputs();
    //static Game game;
    Game::init();
    main_loop();
    return 0;
}