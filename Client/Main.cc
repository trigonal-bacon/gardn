#include <Client/Game.hh>
#include <Client/Setup.hh>

int main() {
    srand(std::time(0));
    Game::init();
    main_loop();
    return 0;
}