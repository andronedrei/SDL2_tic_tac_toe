#include <SDL2/SDL.h>
#include <cmath>

#include <custom/utils.h>

GameModifiers::GameModifiers() {
    nr_rows = 3;
    nr_columns = 3;
    nr_win_line = 3; // must be smaller than minimum of previous 2

    grid_color = {255, 255, 255, SDL_ALPHA_OPAQUE};
    color_X = {255, 0, 0, SDL_ALPHA_OPAQUE};
    color_0 = {0, 0, 255, SDL_ALPHA_OPAQUE};
    color_Z = {100, 100, 100, SDL_ALPHA_OPAQUE};
    color_win = {0, 255, 0, SDL_ALPHA_OPAQUE};

    nr_players = 2; // must be 2 or 3

    type1 = HUMAN;
    symbol1 = CELL_X; // must not be CELL_EMPTY and all different

    type2 = HUMAN;
    symbol2 = CELL_0;

    type3 = HUMAN;
    symbol3 = CELL_Z;
}

long PointsDist(SDL_Point p1, SDL_Point p2) {
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;

    return sqrt(dx * dx + dy * dy);
}

long LineSlope(SDL_Point p1, SDL_Point p2) {
    if (p1.x != p2.x) {
        return 1.0 * (p2.y - p1.y) / (p2.x - p1.x); // slope of line
    }
    return 1000.0; // really big slope to avoid dividing by 0 in slope formula
}

void RenderThickLine(SDL_Renderer *renderer, SDL_Point start, SDL_Point stop, int thickness) {
    long slope; 

    slope = LineSlope(start, stop);

    // draw "base line"
    SDL_RenderDrawLine(renderer, start.x, start.y, stop.x, stop.y);

    // draw the "thickness"
    if (abs(slope) > 1) {
        // if "slope" > 1 => line is "somewhat" vertical
        for (int i = 0; i < thickness; i++) {
            SDL_RenderDrawLine(renderer, start.x + i, start.y, stop.x + i, stop.y);
            SDL_RenderDrawLine(renderer, start.x - i, start.y, stop.x - i, stop.y);
        }
    } else {
        // if "slope" <= 1 => line is "somewhat" horizontal
        for (int i = 0; i < thickness; i++) {
            SDL_RenderDrawLine(renderer, start.x, start.y + i, stop.x, stop.y + i);
            SDL_RenderDrawLine(renderer, start.x, start.y - i, stop.x, stop.y - i);
        }
    }
}

void RenderX(SDL_Renderer *renderer, SDL_Rect dimensions, int thickness) {
    int offset = std::min(dimensions.w, dimensions.h) / 10; // offset so it doesn t touch margins

    RenderThickLine(renderer, 
        {dimensions.x + offset, dimensions.y + offset}, 
        {dimensions.x + dimensions.w - offset, dimensions.y + dimensions.h - offset}, 
        thickness
    );

    RenderThickLine(renderer, 
        {dimensions.x + offset, dimensions.y + dimensions.h - offset}, 
        {dimensions.x + dimensions.w - offset, dimensions.y + offset}, 
        thickness
    );
}

void Render0(SDL_Renderer *renderer, SDL_Rect dimensions, int thickness) {
    int radius;
    SDL_Point center;
    SDL_Point cur_point;
    long deviation;

    center.x = dimensions.x + dimensions.w / 2;
    center.y = dimensions.y + dimensions.h / 2;
    radius = std::min(dimensions.h, dimensions.w) * 8 / 20; // we let a 10 % offset (/ 2 / 10)

    for (int i = 0; i < dimensions.w; i++) {
        for (int j = 0; j < dimensions.h; j++) {
            cur_point.x = dimensions.x + i;
            cur_point.y = dimensions.y + j;

            deviation = abs(PointsDist(cur_point, center) - radius);

            if (deviation < 0.5 || deviation < thickness) { // deviations < 0.5 <=> thickness == 0
                SDL_RenderDrawPoint(renderer, cur_point.x, cur_point.y);
            }
        }
    }
}