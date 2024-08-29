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
    CELL_EMPTY
};

struct grid_line_data {
    int start_row;
    int start_column;
    int stop_row;
    int stop_column;
};

long PointsDist(SDL_Point p1, SDL_Point p2);
long LineSlope(SDL_Point p1, SDL_Point p2);

void RenderThickLine(SDL_Renderer *renderer, SDL_Point start, SDL_Point stop, int thickness);
void RenderX(SDL_Renderer *renderer, SDL_Rect dimensions, int thickness);
void Render0(SDL_Renderer *renderer, SDL_Rect dimensions, int thickness);

#endif