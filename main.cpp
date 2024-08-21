#include <SDL2/SDL.h>
#include <iostream>

#include "custom/utils.h"
#include "custom/game_interface.h"

void exit_game(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);    
    SDL_DestroyWindow(window);
    SDL_Quit();    
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("TicTacToe", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        480, 
        360,
        SDL_WINDOW_SHOWN
    );
    if (window == nullptr) {
        std::cerr << "Could not create window: " << SDL_GetError();
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window,
        -1, 
        SDL_RENDERER_ACCELERATED
    );
    if (renderer == nullptr) {
        std::cerr << "Could not create renderer: " << SDL_GetError();
        return 1;
    }

    GameGrid game_table(renderer, 30, 3, 
        {255, 255, 255, SDL_ALPHA_OPAQUE}, // grid color - white
        {255, 0, 0, SDL_ALPHA_OPAQUE}, // X color - red
        {0, 0, 255, SDL_ALPHA_OPAQUE}, // 0 color - blue
        {0, 255, 0, SDL_ALPHA_OPAQUE} // win cross color - green
    );

    ////
    game_table.set_0(1, 1);
    game_table.set_X(1, 2);
    ////
    int mouseX;
    int mouseY;

    bool run_game = true;
    SDL_Event event;

    while(run_game) { 
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                run_game = false;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                mouseX = event.button.x;
                mouseY = event.button.y;

                int row, column;
                bool ok = game_table.check_mouse_cell({mouseX, mouseY}, row, column);
                std::cout << ok << " " << row << " " << column << "\n";
            }
        }

        // clear renderer before drawing
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); // black background
        SDL_RenderClear(renderer);

        // main logic of game
        game_table.draw_grid();

        // update renderer to display image
        SDL_RenderPresent(renderer);
        SDL_Delay(25);
    }

    exit_game(window, renderer);
    return 0;
}