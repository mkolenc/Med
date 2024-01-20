/*
*  Functions for sdl error handling and deallocating resources
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "utils.h"
#include "SDL.h"
#include "SDL_ttf.h"
////////////////////////////////////////////////////////////////
void scc(int error_code)
{
    if (error_code < 0) {
        fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

void tcc(int error_code)
{
    if (error_code < 0) {
        fprintf(stderr, "SDL ERROR: %s\n",  TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

void* scp(void* ptr)
{
    if (ptr == NULL) {
        fprintf(stderr, "SDL ERROR: %s\n",  SDL_GetError());
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void* cp(void* ptr)
{
    if (ptr == NULL) {
        fprintf(stderr, "ERROR: %s\n",  strerror(errno));
        exit(EXIT_FAILURE);
    }
    return ptr;   
}

void sdl_clean_up(SDL_Window* window, SDL_Renderer* renderer, SDL_Surface* surface, SDL_Texture* texture)
{
    if (window != NULL)
        SDL_DestroyWindow(window);
    if (renderer != NULL)
        SDL_DestroyRenderer(renderer);
    if (surface != NULL)
        SDL_FreeSurface(surface);
    if (texture != NULL)
        SDL_DestroyTexture(texture);
    
    TTF_Quit();
    SDL_Quit();
}