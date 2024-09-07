#ifndef GAME_INTERFACE_H
#define GAME_INTERFACE_H

#include <SDL2/SDL.h>
#include <vector>

#include "custom/utils.h"

class GameWindow {
  private:
    int display_index;
    SDL_Window *window;
    SDL_Rect viewport;
    SDL_Renderer *renderer;
    SDL_Texture *background;

  public:
    SDL_Renderer* get_renderer();
    SDL_Window* get_window();
  
    GameWindow();
    // use before updating the rest of visual elements and render
    void prepare_render();
    // use after updating the rest of visual elements and prepare_render
    void render();
    // function that should be used when window was resized
    void handle_resize();
    ~GameWindow();
};

class GameGrid {
  private:
    SDL_Rect grid_dim;
    SDL_Renderer* renderer_used; // pointer to the renderer used

    int grid_nr_columns;
    int grid_nr_rows;
    int cell_size;          
    SDL_Color color_grid;
    SDL_Color color_X;
    SDL_Color color_0;
    SDL_Color color_Z;

    SDL_Point mouse_poz; // used to store mouse position at last relevant position

    bool game_won;
    SDL_Point start_win; // actual x coordonate, not position in grid
    SDL_Point stop_win;
    SDL_Color color_win;

    std::vector<std::vector<cell_state>> grid_data; // matrix for cells data

    const int margins = 50;
    const int thickness = 2;

    int get_vertical_fit_ratio(int height);     
    void draw_cell(cell_pos pos, cell_state symbol_used);
    // function to draw a win line line that shows that a player had won
    void draw_win_line();
    void clear_grid_data();

  public:
    void set_cell_state(cell_pos pos, cell_state state);
    // function used to signify that a player had won (also gives coordonates for winner line)
    void set_winner(grid_line_data data);
    void set_mouse_poz(SDL_Point point);
    // function to check whether mouse hovered over a valid cell (saves row and column)
    bool check_mouse_cell(cell_pos& pos);

    GameGrid(SDL_Renderer* renderer, int grd_nr_rows, int grd_nr_cols, 
      SDL_Color col_grid, SDL_Color col_X, SDL_Color col_0, SDL_Color col_Z, SDL_Color col_win);
    ~GameGrid();
    void draw_grid();
    // function that should be used each time after window was resized to update grid
    void update_grid_dim();

    void DEBUG_func();
};

#endif