#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <vector>

#include "custom/game_interface.h"
#include "custom/utils.h"

SDL_Renderer* GameWindow::get_renderer() {
    return renderer;
}

GameWindow::GameWindow() {
    display_index = 0;

    // initialize SDL related aspects
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Could not initialize SDL video: " << SDL_GetError() << std::endl;
    }

    if (IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG == 0) {
        std::cerr << "Could not initialize IMAGE sdl: " << IMG_GetError() << std::endl;
    }

    // get display resolution
    if (SDL_GetCurrentDisplayMode(0, &display_mode) != 0) {
        std::cerr << "Could not get display mod: " << SDL_GetError() << std::endl;
    }

    window = SDL_CreateWindow("TicTacToe", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        720,//display_mode.w,
        480,//display_mode.h,
        SDL_WINDOW_SHOWN // SDL_WINDOW_FULLSCREEN
    );
    if (window == nullptr) {
        std::cerr << "Could not create window: " << SDL_GetError() << std::endl;
    }

    renderer = SDL_CreateRenderer(window,
        -1, 
        SDL_RENDERER_ACCELERATED
    );
    if (renderer == nullptr) {
        std::cerr << "Could not create renderer: " << SDL_GetError() << std::endl; 
    }

    SDL_Surface *surface = IMG_Load("src\\assets\\background.jpg");
    if (surface == nullptr) {
        std::cerr << "Could not load image: " << SDL_GetError() << std::endl; 
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (surface == nullptr) {
        std::cerr << "Could not load texture: " << SDL_GetError() << std::endl; 
    }
    // free the surface as it s now in video memory
    SDL_FreeSurface(surface);
}

void GameWindow::prepare_render() { // use before updating visual elements and render
    // clear renderer before drawing
    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); // black background
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, texture, nullptr, nullptr); // render the background
}

void GameWindow::render() { // use after updating visual elements and prepare_render
    SDL_RenderPresent(renderer);
}

GameWindow::~GameWindow() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);    
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

void GameGrid::DEBUG_func() {
    std::cout << "_____\nGRID DEBUG:\n";
    std::cout << "NR CELLS: Nr rows: " << grid_nr_rows << " Nr columns: " << grid_nr_columns << "\n";
    std::cout << "Dimensions: (x, y): " << grid_dim.x << " " << grid_dim.y << "\n";
    std::cout << "(w, h): " << grid_dim.w << " " << grid_dim.h << "\n";
    std::cout << "Cur mouse pos: (" << mouse_poz.x << "," << mouse_poz.y << ")" << "\n";
    for (int i = 0; i < grid_nr_rows; i++) {
        for (int j = 0; j < grid_nr_columns; j++) {
            std::cout << grid_data[i][j] << " ";
        }
        std::cout << "\n";
    }
}

int GameGrid::get_vertical_fit_ratio(int height) {
    return height * 3 / 4;
}
        
void GameGrid::find_grid_dim() {
    SDL_RenderGetViewport(renderer_used, &viewport); // get window size

    // grid will fit within 3/4 of screen with set "margins"
    int max_cell_dx = (viewport.w - 2 * margins) / grid_nr_columns;
    max_cell_dx = std::max(max_cell_dx, 0); // make sure we don t get a smaller than 0 value

    int max_cell_dy = (get_vertical_fit_ratio(viewport.h) - 2 * margins) / grid_nr_rows;
    max_cell_dy = std::max(max_cell_dy, 0);
    
    cell_size = std::min(max_cell_dx, max_cell_dy);
    grid_dim.w = cell_size * grid_nr_columns;
    grid_dim.h = cell_size * grid_nr_rows;

    grid_dim.x = std::max((viewport.w - grid_dim.w) / 2, 0); 
    grid_dim.y = std::max((get_vertical_fit_ratio(viewport.h) - grid_dim.h) / 2, 0);
}

void GameGrid::draw_cell(cell_pos pos, cell_state symbol_used) {
    SDL_Rect cell_dim;

    cell_dim.x = grid_dim.x + pos.column * cell_size;
    cell_dim.y = grid_dim.y + pos.row * cell_size;
    cell_dim.w = cell_size;
    cell_dim.h = cell_size;

    switch (symbol_used) {
        case CELL_X:
            SDL_SetRenderDrawColor(renderer_used, 
                color_X.r, color_X.g, color_X.b, color_X.a
            );
            RenderX(renderer_used, cell_dim, thickness * 2); // we use double thickness for symbols
            break;
        case CELL_0:
            SDL_SetRenderDrawColor(renderer_used, 
                color_0.r, color_0.g, color_0.b, color_0.a
            );
            Render0(renderer_used, cell_dim, thickness * 2); // we use double thickness for symbols
            break;
        default: break;
    }
}

void GameGrid::draw_win_line() {
    SDL_SetRenderDrawColor(renderer_used,
        color_win.r, color_win.g, color_win.b, color_win.a
    );
    RenderThickLine(renderer_used, start_win, stop_win, thickness);
}

void GameGrid::clear_grid_data() {
    for (int i = 0; i < grid_nr_rows; i++) {
        for (int j = 0; j < grid_nr_columns; j++) {
            grid_data[i][j] = CELL_EMPTY;
        }
    }

    game_won = false;
}

void GameGrid::set_cell_state(cell_pos pos, cell_state state) {
    grid_data[pos.row][pos.column] = state;
}

void GameGrid::set_winner(grid_line_data data) {
    start_win.x = grid_dim.x + data.start_cell.column * cell_size + cell_size / 2;
    start_win.y = grid_dim.y + data.start_cell.row * cell_size + cell_size / 2;

    stop_win.x = grid_dim.x + data.stop_cell.column * cell_size + cell_size / 2;
    stop_win.y = grid_dim.y + data.stop_cell.row * cell_size + cell_size / 2;

    game_won = true;
}

void GameGrid::set_mouse_poz(SDL_Point point) {
    mouse_poz = point;
}

// function to check whether mouse hovers over a valid cell (saves row and column)
bool GameGrid::check_mouse_cell(cell_pos& pos) {
    // first check whether mouse is inside grid
    if (mouse_poz.x < grid_dim.x || mouse_poz.x > grid_dim.x + grid_dim.w) {
        return false;
    }

    if (mouse_poz.y < grid_dim.y || mouse_poz.y > grid_dim.y + grid_dim.h) {
        return false;
    }

    pos.column = (mouse_poz.x - grid_dim.x) / cell_size;
    pos.row = (mouse_poz.y - grid_dim.y) / cell_size;

    return grid_data[pos.row][pos.column] == CELL_EMPTY ? true : false;
}

GameGrid::GameGrid(SDL_Renderer* renderer, int grd_nr_cols, int grd_nr_rows, 
    SDL_Color col_grid, SDL_Color col_X, SDL_Color col_0, SDL_Color col_win)
    : renderer_used(renderer), grid_nr_columns(grd_nr_cols), grid_nr_rows(grd_nr_rows),
    color_grid(col_grid), color_X(col_X), color_0(col_0), color_win(col_win) {

    // resize matrix with data
    grid_data.resize(grid_nr_rows);
    for (int i = 0; i < grid_nr_rows; i++) {
        grid_data[i].resize(grid_nr_columns);
    }

    // prepare state of game
    clear_grid_data();
    find_grid_dim();
}

GameGrid::~GameGrid() {};

void GameGrid::draw_grid() {
    SDL_SetRenderDrawColor(renderer_used, 
        color_grid.r, color_grid.g, color_grid.b, color_grid.a
    );

    // draw vertical lines
    for (int i = 0; i <= grid_nr_columns; i++) {
        int x_poz = grid_dim.x + i * cell_size; // const x coord for each vert line
        RenderThickLine(renderer_used, 
            {x_poz, grid_dim.y}, 
            {x_poz, grid_dim.y + grid_dim.h}, 
            thickness
        );
    }

    // draw horizontal lines
    for (int i = 0; i <= grid_nr_rows; i++) {
        int y_poz = grid_dim.y + i * cell_size; // const y coord for each horiz line
        RenderThickLine(renderer_used, 
            {grid_dim.x, y_poz}, 
            {grid_dim.x + grid_dim.w, y_poz},
            thickness
        ); 
    }

    // draw "X" and "0" symbols
    for (int i = 0; i < grid_nr_rows; i++) {
        for (int j = 0; j < grid_nr_columns; j++) {
            draw_cell({i, j}, grid_data[i][j]);
        }
    }

    // draw cross line for winner if case
    if (game_won == true) {
        draw_win_line();
    }
}