#include <iostream>
// #include "custom/game_logic.h"

// remove later
#include "custom/game_interface.h"

int main(int argc, char* argv[]) {
    // GameManager* game_manager = new GameManager();
    // game_manager->game_loop();
    // delete game_manager; // delete game data, destructor can be called

    GameWindow *window = new GameWindow();
    SDL_Event event;
    bool run_game = true;

    while (run_game) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                run_game = false;
            }
        }

        window->prepare_render();
        window->render();

        if (run_game == false) {
            delete window;
        }               
    }
    return 0;
}