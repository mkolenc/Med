#ifndef RENDER_H_
#define RENDER_H_

#include "SDL.h"
#include "font.h"
#include "editor.h"
#include "vec.h"

// Render text to the screen
void render_char(SDL_Renderer* renderer, const Font* font, char ch, Vec2f pos, float scale);
void render_text(SDL_Renderer* renderer, const Font* font, const char* text, Vec2f pos, SDL_Color color, float scale);
void render_text_segment(SDL_Renderer* renderer, const Font* font, const char* text, size_t size, Vec2f pos, SDL_Color color, float scale);

// Render the cursor
void render_cursor(SDL_Renderer* renderer, const Font* font, Editor* editor, Vec2f pos, SDL_Color cursor_color, SDL_Color text_beneath_cursor_color);

#endif /* RENDER_H_ */