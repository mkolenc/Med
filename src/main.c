#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "utils.h"
#include "font.h"
#include "render.h"
#include "vec.h"

#include "editor.h"

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

    Editor* editor = editor_init();

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
                    editor_insert_text_before_cursor(editor, event.text.text); 
                    //printf("EDITOR: capacity %zu, size %zu, row %zu, col %zu\n", editor->capacity, editor->size, editor->cursor_row, editor->cursor_col);
                    //printf("LINE: capacity %zu, size %zu\n", editor->lines[editor->cursor_row].capacity, editor->lines[editor->cursor_row].size);
                }
                break;

                case SDL_KEYDOWN: {
                    switch (event.key.keysym.sym) {
                        case SDLK_BACKSPACE: {
                            editor_backspace(editor); // done
                        }
                        break;

                        case SDLK_DELETE: {
                            editor_delete(editor); // done
                        } break;

                        case SDLK_LEFT: {
                            editor_left_arrow(editor); // done
                        }
                        break;

                        case SDLK_RIGHT: {
                            editor_right_arrow(editor); // done
                        }
                        break;

                        case SDLK_UP: {
                            editor_up_arrow(editor); // done
                        }
                        break;

                        case SDLK_DOWN: {
                            editor_down_arrow(editor); // done
                        }
                        break;
                        
                        case SDLK_RETURN: { // done 
                            editor_return(editor);
                        }
                        break;

                        case SDLK_TAB: {
                            // I am just going to insert spaces (atleast for now) to make file editing easier,
                            // and to conserve space when you save a file, convert 4 spaces into a single \t character
                            editor_tab(editor); // done
                        }
                    }
                    break;
                }
            }            
        }
        scc((SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0)));
        scc((SDL_RenderClear(renderer)));

        for (size_t i = 0; i < editor->size; i++) {
            render_text_segment(renderer, font, editor->lines[i].chars, editor->lines[i].size, vec2f(0, i * FONT_HEIGHT * FONT_SCALE), (SDL_Color) {.r = 255, .g = 255, .b = 255, .a = 255}, FONT_SCALE);
        }
        render_cursor(renderer, font, editor, vec2f(editor->cursor_col * FONT_WIDTH * FONT_SCALE, editor->cursor_row * FONT_HEIGHT * FONT_SCALE), (SDL_Color) {0, 255, 0, 255}, (SDL_Color) {0, 0, 0, 255});

        SDL_RenderPresent(renderer);
        
    }
    editor_free(editor); // something is going wrong, with my free's
    font_free_ttf(font);
    sdl_clean_up(window, renderer, NULL, NULL);
    
    return 0;
}

