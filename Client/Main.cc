#include <Client/Game.hh>
#include <Client/Setup.hh>

int main() {
    Game::init();
    main_loop();
    return 0;
}