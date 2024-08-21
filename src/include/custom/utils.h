#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>


enum cell_state {
    cell_empty,
    cell_X,
    cell_0
};

long PointsDist(SDL_Point p1, SDL_Point p2);
long LineSlope(SDL_Point p1, SDL_Point p2);

void RenderThickLine(SDL_Renderer *renderer, SDL_Point start, SDL_Point stop, int thickness);
void RenderX(SDL_Renderer *renderer, SDL_Rect dimensions, int thickness);
void Render0(SDL_Renderer *renderer, SDL_Rect dimensions, int thickness);

#endif