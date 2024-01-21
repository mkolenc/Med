/*
 *  Utility functions for error handling and resource cleanup in an SDL-based application.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "utils.h"
#include "font.h"
#include "editor.h"
#include "SDL.h"
#include "SDL_ttf.h"

/*
 *  Purpose: Handle SDL errors, check the error code, and exit on failure.
 *
 *  Parameters:
 *    - error_code: The error code to be checked.
 */
void utils_scc(int error_code)
{
    if (error_code < 0) {
        fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

/*
 *  Purpose: Handle SDL pointer errors, check if a pointer is NULL, and exit on failure.
 *
 *  Parameters:
 *    - ptr: The pointer to be checked.
 *
 *  Returns:
 *    - The same pointer if it is not NULL.
 */
void* utils_scp(void* ptr)
{
    if (ptr == NULL) {
        fprintf(stderr, "SDL ERROR: %s\n",  SDL_GetError());
        exit(EXIT_FAILURE);
    }
    return ptr;
}

/*
 *  Purpose: Handle general pointer errors, check if a pointer is NULL, and exit on failure.
 *
 *  Parameters:
 *    - ptr: The pointer to be checked.
 *
 *  Returns:
 *    - The same pointer if it is not NULL.
 */
void* utils_cp(void* ptr)
{
    if (ptr == NULL) {
        fprintf(stderr, "ERROR: %s\n",  strerror(errno));
        exit(EXIT_FAILURE);
    }
    return ptr;   
}

/*
 *  Purpose: Perform cleanup by freeing resources and quitting SDL and related libraries.
 *
 *  Parameters:
 *    - window: A pointer to an SDL window to be destroyed.
 *    - renderer: A pointer to an SDL renderer to be destroyed.
 *    - font: A pointer to a Font structure to be freed.
 *    - editor: A pointer to an Editor structure to be freed.
 * 
 *  Returns: None.
 */
void utils_clean_up(SDL_Window* window, SDL_Renderer* renderer, Font* font, Editor* editor)
{
    if (window != NULL)
        SDL_DestroyWindow(window);
    if (renderer != NULL)
        SDL_DestroyRenderer(renderer);
    if (font != NULL)
        font_free_ttf(font);
    if (editor != NULL)
        editor_free(editor);
    
    TTF_Quit();
    SDL_Quit();
}