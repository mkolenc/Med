#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include "utils.h"
#include "font.h"
#include "render.h"
#include "vec.h"
#include "editor.h"

// Dependencies
#include "SDL.h"
#include "SDL_ttf.h"

// TODO: Change how you save a file to ctr + s
// TODO: Jump forward/backward by a word
// TODO: Delete a word
// TODOinking cursor: Bl
// TODO: Delete line

int main(int argc, const char* argv[])
{
    utils_scc(SDL_Init(SDL_INIT_VIDEO));
    utils_scc((TTF_Init()));

    SDL_Window* window =
        utils_scp(SDL_CreateWindow("Text Editor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_RESIZABLE));

    SDL_Renderer* renderer =
        utils_scp(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED));

    Font* font = font_load_ttf(renderer, "./font/VictorMono-Regular.ttf");

    Editor editor = {0};

    // Temporary way to load in files from command line arguments
    const char* file_path = argc > 1 ? argv[1] : NULL;

    if (file_path != NULL) {
        FILE* fp = fopen(file_path, "r");

        if (fp != NULL) {
            editor_load_from_file(&editor, fp);
            fclose(fp);
        } else {
            printf("Error: Unable to load file '%s': %s\n", file_path, strerror(errno));
            printf("To create '%s' and save your work, press F2\n", file_path);
        }
    }

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
                    editor_insert_text_before_cursor(&editor, event.text.text);
                }
                break;

                case SDL_KEYDOWN: {
                    switch (event.key.keysym.sym) {
                        case SDLK_BACKSPACE: {
                            editor_backspace(&editor);
                        }
                        break;

                        case SDLK_DELETE: {
                            editor_delete(&editor);
                        } break;

                        case SDLK_LEFT: {
                            editor_left_arrow(&editor);
                        }
                        break;

                        case SDLK_RIGHT: {
                            editor_right_arrow(&editor);
                        }
                        break;

                        case SDLK_UP: {
                            editor_up_arrow(&editor);
                        }
                        break;

                        case SDLK_DOWN: {
                            editor_down_arrow(&editor);
                        }
                        break;
                        
                        case SDLK_RETURN: {
                            editor_return(&editor);
                        }
                        break;

                        case SDLK_TAB: {
                            editor_tab(&editor);
                        }
                        break;

                        case SDLK_F2: {
                            if (file_path != NULL) {
                                editor_save_to_file(&editor, file_path);
				puts("Saved!");
                            } else
                                fprintf(stderr, "Usage: ./med [FILE-PATH]\n");
                        }
                        break;
                    }
                }
                break;
            }            
        }
        utils_scc((SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0)));
        utils_scc((SDL_RenderClear(renderer)));

        for (size_t i = 0; i < editor.size; i++) {
            render_text_segment(renderer, font, editor.lines[i].chars, editor.lines[i].size,
                                vec2f(0, i * FONT_HEIGHT * FONT_SCALE),
                                (SDL_Color) {.r = 255, .g = 255, .b = 255, .a = 255},
                                FONT_SCALE);
        }
        render_cursor(renderer, font, &editor, (SDL_Color) {255, 255, 255, 255}, (SDL_Color) {0, 0, 0, 255});

        SDL_RenderPresent(renderer);
    }
    utils_clean_up(window, renderer, font, &editor);
    
    return EXIT_SUCCESS;
}
