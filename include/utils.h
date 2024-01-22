/*
 *  Utility functions for error handling and resource cleanup in an SDL-based application.
 */
#ifndef UTILS_H_
#define UTILS_H_

#include "SDL.h"
#include "font.h"
#include "editor.h"

/*
 *  Purpose: Handle SDL errors, check the error code, and exit on failure.
 *
 *  Parameters:
 *    - error_code: The error code to be checked.
 */
void utils_scc(int error_code);

/*
 *  Purpose: Handle SDL pointer errors, check if a pointer is NULL, and exit on failure.
 *
 *  Parameters:
 *    - ptr: The pointer to be checked.
 *
 *  Returns:
 *    - The same pointer if it is not NULL.
 */
void* utils_scp(void* ptr);

/*
 *  Purpose: Handle general pointer errors, check if a pointer is NULL, and exit on failure.
 *
 *  Parameters:
 *    - ptr: The pointer to be checked.
 *
 *  Returns:
 *    - The same pointer if it is not NULL.
 */
void* utils_cp(void* ptr);

/*
 *  Purpose: Perform cleanup by freeing resources and quitting SDL and related libraries.
 *
 *  Parameters:
 *    - window: A pointer to an SDL window to be destroyed (Can be NULL).
 *    - renderer: A pointer to an SDL renderer to be destroyed (Can be NULL).
 *    - font: A pointer to a Font structure to be freed (Can be NULL).
 *    - editor: A pointer to an Editor structure to be freed (Can be NULL).
 * 
 *  Returns: None.
 */
void utils_clean_up(SDL_Window* window, SDL_Renderer* renderer, Font* font, Editor* editor);

#endif /* UTILS_H_ */