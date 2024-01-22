/*
 *  Functions for loading and managing fonts from TrueType Font (TTF) files using SDL.
 */
#include <stdlib.h>

#include "font.h"
#include "utils.h"
#include "SDL_ttf.h"

/*
 *  Purpose: Create an SDL surface atlas of a font from a TrueType Font (TTF) file.
 *
 *  Parameters: 
 *    - file_path: Path to the TrueType Font (TTF) file.
 *
 *  Returns:
 *    - An SDL surface containing the generated font surface atlas.
 */
static SDL_Surface* create_font_surface(const char* file_path)
{ 
    TTF_Font* ttf_font = utils_scp(TTF_OpenFont(file_path, POINT_SIZE));
    SDL_Surface* font_surface = utils_scp(SDL_CreateRGBSurfaceWithFormat(0, FONT_WIDTH * NUM_GLYPHS, FONT_HEIGHT, 32, SDL_PIXELFORMAT_BGRA32));

    SDL_Rect dst = {
        .x = 0,
        .y = 0,
        .w = FONT_WIDTH,
        .h = FONT_HEIGHT,
    };

    for (size_t ascii = ASCII_DISPLAY_LOW; ascii <= ASCII_DISPLAY_HIGH; ascii++) {
        SDL_Surface* ascii_surface = utils_scp(TTF_RenderGlyph32_Blended(ttf_font, ascii, (SDL_Color){.r = 255, .g = 255, .b = 255, .a = 255}));
        utils_scc(SDL_BlitSurface(ascii_surface, NULL, font_surface, &dst));
        SDL_FreeSurface(ascii_surface);
        dst.x += FONT_WIDTH;
    } 
    TTF_CloseFont(ttf_font);

    return font_surface;
}

/*
 *  Purpose: Load a font from a TrueType Font (TTF) file. The font should be released
 *           with 'font_free_ttf' when no longer needed.
 *
 *  Parameters:
 *    - renderer: The SDL renderer to create the font texture on.
 *    - file_path: Path to the TrueType Font (TTF) file.
 *
 *  Returns:
 *    - A Font structure containing the font texture and glyphs position.
 */
Font* font_load_ttf(SDL_Renderer* renderer, const char* file_path)
{
    Font* font = utils_cp(malloc(sizeof(*font)));
    SDL_Surface* font_surface = create_font_surface(file_path);
    font->spritesheet = utils_scp(SDL_CreateTextureFromSurface(renderer, font_surface));
    SDL_FreeSurface(font_surface);

    for (size_t ascii = ASCII_DISPLAY_LOW; ascii <= ASCII_DISPLAY_HIGH; ascii++) {
        const size_t index = ascii - ASCII_DISPLAY_LOW;
        font->glyphs[index] = (SDL_Rect) {.x = index * FONT_WIDTH, .y = 0, .w = FONT_WIDTH, .h = FONT_HEIGHT};
    }

    return font;
}

/*
 *  Purpose: Free resources associated with a loaded TTF font.
 *
 *  Parameters:
 *    - font: The Font structure to be freed.
 *  
 *  Returns: None.
 */
void font_free_ttf(Font* font)
{
    SDL_DestroyTexture(font->spritesheet);
    free(font);
}