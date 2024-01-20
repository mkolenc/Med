#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "render.h"
#include "utils.h"
#include "line.h"
#include "font.h"
#include "vec.h"
#include "SDL.h"

// Renders a single char to the window
void render_char(SDL_Renderer* renderer, const Font* font, char c, Vec2f pos, float scale)
{
    SDL_Rect dst = {
        .x = pos.x,
        .y = pos.y,
        .w = FONT_WIDTH * scale,
        .h = FONT_HEIGHT * scale,
    };

    size_t index = '?' - ASCII_DISPLAY_LOW;
    if (c >= ASCII_DISPLAY_LOW && c <= ASCII_DISPLAY_HIGH)
        index = c - ASCII_DISPLAY_LOW;

    scc(SDL_RenderCopy(renderer, font->spritesheet, &font->glyphs[index], &dst));
}

// private function used to set the color of the text on screen
static void set_texture_color(SDL_Texture *texture, SDL_Color color)
{
    scc(SDL_SetTextureColorMod(texture, color.r, color.g, color.b));
    scc(SDL_SetTextureAlphaMod(texture, color.a));
}

// renders a substring of text on the screen
void render_text_segment(SDL_Renderer *renderer, const Font *font, const char* text, size_t text_size, Vec2f pos, SDL_Color color, float scale)
{
    if (text_size == 0) return;

    set_texture_color(font->spritesheet, color);
    assert(text_size <= strlen(text));

    for (size_t i = 0; i < text_size; i++) {
        render_char(renderer, font, text[i], pos, scale);
        pos.x += FONT_WIDTH * scale;
    }
}

// renders a full string on the screen
void render_text(SDL_Renderer* renderer, const Font* font, const char* text, Vec2f pos, SDL_Color color, float scale)
{
    render_text_segment(renderer, font, text, strlen(text), pos, color, scale); 
}

static char* text_under_cursor(Line* line, size_t col)
{
    return col < line->size ? &line->chars[col] : NULL;
}

// Functions for the cursor
// eventually I want to have a struc for the cursor, and have the cursor color, and the text color under the cursor
// and function to get and set theses parameters
void render_cursor(SDL_Renderer* renderer, const Font* font, Line* line, size_t col, Vec2f pos, SDL_Color cursor_color, SDL_Color text_beneath_cursor_color)
{
    SDL_Rect dst = {
        .x = pos.x,
        .y = pos.y,
        .w = FONT_WIDTH * FONT_SCALE,
        .h = FONT_HEIGHT * FONT_SCALE,
    };

    scc(SDL_SetRenderDrawColor(renderer, cursor_color.r, cursor_color.g, cursor_color.b, cursor_color.a));
    scc(SDL_RenderFillRect(renderer, &dst));

    const char* c = text_under_cursor(line, col);
    if (c != NULL) {
        set_texture_color(font->spritesheet, text_beneath_cursor_color);
        render_char(renderer, font, *c, pos, FONT_SCALE);
    }
}