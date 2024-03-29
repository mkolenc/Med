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
#include "camera.h"


#include <math.h> // newly added for floor

// Dependencies
#include "SDL.h"
#include "SDL_ttf.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// TODO: Change how you save a file to ctr + s
// TODO: Jump forward/backward by a word
// TODO: Delete a word
// TODO: Blinking cursor when inactive
// TODO: Delete line

int main(int argc, const char* argv[])
{
    utils_scc(SDL_Init(SDL_INIT_VIDEO));
    utils_scc((TTF_Init()));

    SDL_Window* window =
        utils_scp(SDL_CreateWindow("Text Editor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE));

    SDL_Renderer* renderer =
        utils_scp(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED));

    Font* font = font_load_ttf(renderer, "./font/VictorMono-Regular.ttf");

    Editor editor = {0};
    Camera camera = {0};

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

    CursorShape cursor_shape = 0;
    const float cursor_period_ms = 500; // half a second
    Uint32 last_stroke_time = 0;
    const Uint32 blink_threshold_ms = 500; // an issue here is that sometimew hwen it starts blinking agin it is quick on the first one and sometimes it is long
                                            // thsi is becuase the cursor is always blinking and we may start rendering it again in the middle of a period
    bool quit = false;
    while (!quit) {
        // start of the frame time
        Uint32 frame_start_time_ms = SDL_GetTicks();
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: {
                    quit = true;
                }
                break;

                case SDL_TEXTINPUT: {
                    editor_insert_text_before_cursor(&editor, event.text.text);
                    last_stroke_time = SDL_GetTicks();
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
				                puts("Save successful!");
                            } else
                                fprintf(stderr, "Usage: ./med [FILE-PATH]\n");
                        }
                        break;

                        case SDLK_F1: {
                            if (cursor_shape < 2)
                                cursor_shape++;
                            else
                                cursor_shape = 0;
                        }
                    }
                    last_stroke_time = SDL_GetTicks();
                }
                break;
            }            
        }
        // clear the screen
        utils_scc((SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255)));
        utils_scc((SDL_RenderClear(renderer)));
        
        camera_update(&camera, &editor);
        render_editor(renderer, font, &editor, window, &camera, (SDL_Color) {.r = 255, .g = 0, .b = 255, .a = 255}, FONT_SCALE);


        if (SDL_GetTicks() - last_stroke_time < blink_threshold_ms)
            render_cursor(renderer, font, &editor, window, camera.pos, (SDL_Color) {255, 255, 255, 255}, (SDL_Color) {0, 0, 0, 255}, cursor_shape);

        // we can generate an on/off cycles for the cursor to simulate the blinking
         else if (((int)floor(SDL_GetTicks() / cursor_period_ms) % 2))
            render_cursor(renderer, font, &editor, window, camera.pos, (SDL_Color) {255, 255, 255, 255}, (SDL_Color) {0, 0, 0, 255}, cursor_shape);

        // update the screen
        SDL_RenderPresent(renderer);
        
        camera_cap_fps(SDL_GetTicks() - frame_start_time_ms);
    }
    utils_clean_up(window, renderer, font, &editor);
    
    return EXIT_SUCCESS;
}
