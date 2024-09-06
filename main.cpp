#include <iostream>
#include "custom/game_logic.h"

int main(int argc, char* argv[]) {
    GameManager* game_manager = new GameManager();
    game_manager->game_loop();
    delete game_manager; // delete game data, destructor should be called

    return 0;
}