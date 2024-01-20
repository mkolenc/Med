#include <stdlib.h>
#include "font.h"
#include "utils.h"
#include "SDL.h"
#include "SDL_ttf.h"

static SDL_Surface* create_font_surface(const char* file_path)
{
    TTF_Font* ttf_font = scp(TTF_OpenFont(file_path, POINT_SIZE));
    SDL_Surface* font_surface = scp(SDL_CreateRGBSurfaceWithFormat(0, FONT_WIDTH * NUM_GLYPHS, FONT_HEIGHT, 32, SDL_PIXELFORMAT_BGRA32));

    SDL_Rect dst = {
        .x = 0,
        .y = 0,
        .w = FONT_WIDTH,
        .h = FONT_HEIGHT,
    };

    for (size_t ascii = ASCII_DISPLAY_LOW; ascii <= ASCII_DISPLAY_HIGH; ascii++) {
        SDL_Surface* ascii_surface = scp(TTF_RenderGlyph32_Blended(ttf_font, ascii, (SDL_Color){.r = 255, .g = 255, .b = 255, .a = 255}));
        scc(SDL_BlitSurface(ascii_surface, NULL, font_surface, &dst));
        SDL_FreeSurface(ascii_surface);
        dst.x += FONT_WIDTH;
    }
    TTF_CloseFont(ttf_font);

    return font_surface;
}

Font* font_load_ttf(SDL_Renderer* renderer, const char* file_path)
{
    Font* font = cp(malloc(sizeof(*font)));
    SDL_Surface* font_surface = create_font_surface(file_path);
    font->spritesheet = scp(SDL_CreateTextureFromSurface(renderer, font_surface));
    SDL_FreeSurface(font_surface);

    for (size_t ascii = ASCII_DISPLAY_LOW; ascii <= ASCII_DISPLAY_HIGH; ascii++) {
        const size_t index = ascii - ASCII_DISPLAY_LOW;
        font->glyphs[index] = (SDL_Rect) {.x = index * FONT_WIDTH, .y = 0, .w = FONT_WIDTH, .h = FONT_HEIGHT};
    }

    return font;
}

void font_free_ttf(Font* font)
{
    SDL_DestroyTexture(font->spritesheet);
    free(font);
}