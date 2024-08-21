#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

#include "custom/game_interface.h"
#include "custom/utils.h"

void GameGrid::test_func() {
    std::cout << "x y: " << grid_dim.x << " " << grid_dim.y << std::endl << "w h: " << grid_dim.w << " " << grid_dim.h << std::endl;
    for (int i = 0; i < grid_nr_rows; i++) {
        for (int j = 0; j < grid_nr_columns; j++) {
            std::cout << grid_data[i][j] << " ";
        }
        std::cout << std::endl;
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

void GameGrid::draw_X(int row, int column) {
    SDL_Rect cell_dim;

    cell_dim.x = grid_dim.x + column * cell_size;
    cell_dim.y = grid_dim.y + row * cell_size;
    cell_dim.w = cell_size;
    cell_dim.h = cell_size;

    SDL_SetRenderDrawColor(renderer_used, 
        color_X.r, color_X.g, color_X.b, color_X.a
    );
    RenderX(renderer_used, cell_dim, thickness * 2); // we use double thickness for symbols
}

void GameGrid::draw_0(int row, int column) {
    SDL_Rect cell_dim;

    cell_dim.x = grid_dim.x + column * cell_size;
    cell_dim.y = grid_dim.y + row * cell_size;
    cell_dim.w = cell_size;
    cell_dim.h = cell_size;

    SDL_SetRenderDrawColor(renderer_used, 
        color_0.r, color_0.g, color_0.b, color_0.a
    );
    Render0(renderer_used, cell_dim, thickness * 2);
}

void GameGrid::clear_grid_data() {
    for (int i = 0; i < grid_nr_rows; i++) {
        for (int j = 0; j < grid_nr_columns; j++) {
            grid_data[i][j] = cell_empty;
        }
    }
}

void GameGrid::set_X(int row, int column) {
    grid_data[row][column] = cell_X;
}

void GameGrid::set_0(int row, int column) {
    grid_data[row][column] = cell_0;
}

// function to check whether mouse hovers over a valid cell (saves row and column)
bool GameGrid::check_mouse_cell(SDL_Point mouse_poz, int& row, int& column) {
    // first check whether mouse is inside grid
    if (mouse_poz.x < grid_dim.x || mouse_poz.x > grid_dim.x + grid_dim.w) {
        return false;
    }

    if (mouse_poz.y < grid_dim.y || mouse_poz.y > grid_dim.y + grid_dim.h) {
        return false;
    }

    column = (mouse_poz.x - grid_dim.x) / cell_size;
    row = (mouse_poz.y - grid_dim.y) / cell_size;

    return grid_data[row][column] == cell_empty ? true : false;
}

GameGrid::GameGrid(SDL_Renderer* renderer, int grd_nr_cols, int grd_nr_rows, 
    SDL_Color col_grid, SDL_Color col_X, SDL_Color col_0, SDL_Color col_cross) {
    renderer_used = renderer;
    grid_nr_columns = grd_nr_cols;
    grid_nr_rows = grd_nr_rows;
    color_grid = col_grid;
    color_X = col_X;
    color_0 = col_0;
    color_cross = col_cross;

    // resize matrix with data
    grid_data.resize(grid_nr_rows);
    for (int i = 0; i < grid_nr_rows; i++) {
        grid_data[i].resize(grid_nr_columns);
    }

    clear_grid_data();
    find_grid_dim();
}

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
            if (grid_data[i][j] == cell_X) {
                draw_X(i, j);
            }
            if (grid_data[i][j] == cell_0) {
                draw_0(i, j);
            }
        }
    }
}