#include <SDL2/SDL.h>
#include <cmath>
#include <iostream>

#include <custom/utils.h>

GameModifiers::GameModifiers() {
    nr_rows = 4;
    nr_columns = 4;
    nr_win_line = 3; // must be smaller than minimum of previous 2

    grid_color = {0, 0, 0, SDL_ALPHA_OPAQUE};
    color_X = {255, 0, 0, SDL_ALPHA_OPAQUE};
    color_0 = {0, 0, 255, SDL_ALPHA_OPAQUE};
    color_Z = {100, 100, 100, SDL_ALPHA_OPAQUE};
    color_win = {0, 255, 0, SDL_ALPHA_OPAQUE};

    nr_players = 2; // must be 2 or 3

    type1 = HUMAN;
    symbol1 = CELL_X; // must not be CELL_EMPTY and all different
    diff1 = HUMAN_DIFF; // diff of a potential robot player. For human player use "NOT_ROBOT"

    type2 = HUMAN;
    symbol2 = CELL_Z;
    diff2 = HUMAN_DIFF;

    type3 = HUMAN;
    symbol3 = CELL_0;
    diff3 = HUMAN_DIFF;

    small_delay = 20; // delay in ms
    big_delay = 2000;
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

void RenderZ(SDL_Renderer *renderer, SDL_Rect dimensions, int thickness) {
    int offset = std::min(dimensions.w, dimensions.h) / 10; // offset so it doesn t touch margins

    // draw upper line
    RenderThickLine(renderer, 
        {dimensions.x + offset, dimensions.y + offset}, 
        {dimensions.x + dimensions.w - offset, dimensions.y + offset}, 
        thickness
    );

    // draw diag line
    RenderThickLine(renderer, 
        {dimensions.x + dimensions.w - offset, dimensions.y + offset}, 
        {dimensions.x + offset, dimensions.y + dimensions.h - offset}, 
        thickness
    );

    // draw lower line
    RenderThickLine(renderer, 
        {dimensions.x + offset, dimensions.y + dimensions.h - offset}, 
        {dimensions.x + dimensions.w - offset, dimensions.y + dimensions.h - offset}, 
        thickness
    );

}

// resizes a surface 
SDL_Surface* ResizeSurface(SDL_Surface* original_surface, int window_width, int window_height) {
    // make sure the new surface has the same format so information can be copied correctly
    SDL_Surface* resized_surface = SDL_CreateRGBSurface(0, window_width, window_height,
        original_surface->format->BitsPerPixel,
        original_surface->format->Rmask,
        original_surface->format->Gmask,
        original_surface->format->Bmask,
        original_surface->format->Amask
    );
    if (resized_surface == nullptr) {
        std::cerr << "Could not create resized surface: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    // create a rectangle for the destination area
    SDL_Rect dstRect = {0, 0, window_width, window_height};

    // copy the original surface to the resized surface, scaling it to fit
    if (SDL_BlitScaled(original_surface, nullptr, resized_surface, &dstRect) != 0) {
        std::cerr << "Could not scale surface: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(resized_surface);
        return nullptr;
    }

    return resized_surface;
}

// function to creeate a new texture of desired size from surface (original surface is destroyed)
SDL_Texture* CreateSizedTexture(SDL_Renderer *renderer, SDL_Surface* original_surface, int window_width, int window_height) {
    original_surface = ResizeSurface(original_surface, window_width, window_height);
    if (original_surface == nullptr) {
        std::cerr <<  "Failed to create texture from surface" << std::endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, original_surface);
    if (texture == nullptr) {
        std::cerr <<  "Failed to create texture from surface: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    SDL_FreeSurface(original_surface); // free the original surface

    return texture;
}

SDL_Texture* CreateSizedTextureFromBMP(SDL_Renderer *renderer, const char *image_file_name, int window_width, int window_height) {
    SDL_Surface* aux_surface = SDL_LoadBMP(image_file_name);
    if (aux_surface == nullptr) {
        std::cerr << "Could not load image: " << SDL_GetError() << std::endl; 
        return nullptr;
    }

    // prepare texture
    SDL_Texture* texture = CreateSizedTexture(renderer, aux_surface, window_width, window_height);

    return texture;
}