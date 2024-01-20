/*
  Functions for handling dynamic text
*/
#ifndef FONT_H_
#define FONT_H_

#include "vec.h"
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

Font* font_load_ttf(SDL_Renderer* renderer, const char* file_path);
void font_free_ttf(Font* font);

#endif /* FONT_H_ */