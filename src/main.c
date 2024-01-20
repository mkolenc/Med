#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "utils.h"
#include "font.h"
#include "render.h"
#include "line.h"

// Dependencies
#include "SDL.h"
#include "SDL_ttf.h"


// TODO: Save/Load file
// TODO: Jump forward/backward by a word
// TODO: Delete a word
// TODO: Blinking cursor
// TODO: Delete line
// TODO: Split the line on Enter
int main(int argc, const char* argv[])
{
    (void) argc;
    (void) argv;

    scc(SDL_Init(SDL_INIT_VIDEO));
    tcc((TTF_Init()));

    SDL_Window* window =
        scp(SDL_CreateWindow("Text Editor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_RESIZABLE));

    SDL_Renderer* renderer =
        scp(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED));

    Font* font = font_load_ttf(renderer, "./font/VictorMono-Regular.ttf");

    Line line = {.capacity = 0, .size = 0, .chars = NULL};
    size_t col = 0;

    bool quit = false;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: {
                    quit = true;
                }
                break;

                case SDL_TEXTINPUT: {
                    line_insert_text(&line, event.text.text, col);
                    col += strlen(event.text.text);
                }
                break;

                case SDL_KEYDOWN: {
                    switch (event.key.keysym.sym) {
                        case SDLK_BACKSPACE: {
                            line_backspace(&line, col);
                            if (col > 0)
                                col--;
                        }
                        break;

                        case SDLK_DELETE: {
                            line_delete(&line, col);
                        } break;

                        case SDLK_LEFT: {
                            if (col > 0)
                                col--;
                        }
                        break;

                        case SDLK_RIGHT: {
                            if (col < line.size)
                                col++;
                        }
                        break;
                    }
                    break;
                }
            }            
        }
        
        scc((SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0)));
        scc((SDL_RenderClear(renderer)));

        render_text_segment(renderer, font, line.chars, line.size, vec2f(0, 0), (SDL_Color) {.r = 255, .g = 255, .b = 255, .a = 255}, 1.0f);
        render_cursor(renderer, font, &line, col, vec2f(col * FONT_WIDTH * FONT_SCALE, 0), (SDL_Color) {0, 255, 0, 255}, (SDL_Color) {0, 0, 0, 255});

        SDL_RenderPresent(renderer);
        
    }
    font_free_ttf(font);
    free(line.chars);
    sdl_clean_up(window, renderer, NULL, NULL);
    
    return 0;
}

