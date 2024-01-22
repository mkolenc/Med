/*
 *  Functions for loading and managing fonts from TrueType Font (TTF) files using SDL.
 */
#ifndef FONT_H_
#define FONT_H_

#include "SDL.h"

// The displayable characters
#define ASCII_DISPLAY_LOW 32
#define ASCII_DISPLAY_HIGH 126
#define NUM_GLYPHS ((ASCII_DISPLAY_HIGH - ASCII_DISPLAY_LOW) + 1)

// specs for the 'VictorMono-Regular' font
#define FONT_WIDTH 17
#define FONT_HEIGHT 40
#define POINT_SIZE 32

#define FONT_SCALE 1.0f
typedef struct {
  SDL_Texture* spritesheet;
  SDL_Rect glyphs[NUM_GLYPHS];
} Font;

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
Font* font_load_ttf(SDL_Renderer* renderer, const char* file_path);

/*
 *  Purpose: Free resources associated with a loaded TTF font.
 *
 *  Parameters:
 *    - font: The Font structure to be freed.
 *
 *  Returns: None.
 */
void font_free_ttf(Font* font);

#endif /* FONT_H_ */