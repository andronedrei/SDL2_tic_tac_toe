#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>

enum player_type {
    HUMAN,
    ROBOT
};

enum cell_state {
    CELL_X,
    CELL_0,
    CELL_Z,
    CELL_EMPTY
};

struct grid_line_data {
    int start_row;
    int start_column;
    int stop_row;
    int stop_column;
};

struct GameModifiers {
    int nr_rows;
    int nr_columns;
    int nr_win_line;

    SDL_Color grid_color;
    SDL_Color color_X;
    SDL_Color color_0;
    SDL_Color color_Z;
    SDL_Color color_win;

    int nr_players; // nr players, 2 or 3

    player_type type1; // type of first player
    cell_state symbol1; // symbol of first player

    player_type type2; // type of second player
    cell_state symbol2; // symbol of second player

    player_type type3; // type of thirth player
    cell_state symbol3; // type of thirth player

    GameModifiers(); // change this function to easily change game modifiers
};

long PointsDist(SDL_Point p1, SDL_Point p2);
long LineSlope(SDL_Point p1, SDL_Point p2);

void RenderThickLine(SDL_Renderer *renderer, SDL_Point start, SDL_Point stop, int thickness);
void RenderX(SDL_Renderer *renderer, SDL_Rect dimensions, int thickness);
void Render0(SDL_Renderer *renderer, SDL_Rect dimensions, int thickness);

#endif