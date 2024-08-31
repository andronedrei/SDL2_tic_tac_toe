#include <SDL2/SDL.h>
#include <iostream>

#include "custom/utils.h"
#include "custom/game_interface.h"
#include "custom/game_logic.h"

int main(int argc, char* argv[]) {
    GameManager* game_manager = new GameManager();

    game_manager->game_loop();
    game_manager->exit_game_window();
    delete game_manager; // delete game data, destructor can be called

    return 0;
}