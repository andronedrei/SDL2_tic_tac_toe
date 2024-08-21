#ifndef GAME_INTERFACE_H
#define GAME_INTERFACE_H

#include <SDL2/SDL.h>
#include <vector>

#include "custom/utils.h"

class GameGrid {
  private:
    SDL_Rect viewport;
    SDL_Rect grid_dim;
    SDL_Renderer* renderer_used; // pointer to used renderer
    int grid_nr_columns;
    int grid_nr_rows;
    int cell_size;          
    SDL_Color color_grid;
    SDL_Color color_X;
    SDL_Color color_0;
    SDL_Color color_cross;
    std::vector<std::vector<cell_state>> grid_data; // matrix for cells data

    const int margins = 50;
    const int thickness = 2;

    void test_func();
    int get_vertical_fit_ratio(int height);     
    void find_grid_dim();
    void draw_X(int row, int column);
    void draw_0(int row, int column);

  public:
    void clear_grid_data();
    void set_X(int row, int column);
    void set_0(int row, int column);
    // function to check whether mouse hovers over a valid cell (saves row and column)
    bool check_mouse_cell(SDL_Point mouse_poz, int& row, int& column);

    GameGrid(SDL_Renderer* renderer, int grd_nr_cols, int grd_nr_rows, 
        SDL_Color col_grid, SDL_Color col_X, SDL_Color col_0, SDL_Color col_cross);
    void draw_grid();
};

#endif