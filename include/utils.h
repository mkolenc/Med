/*
  SDL utility functions for error handling and deallocating sdl resources
*/

#ifndef UTILS_H_
#define UTILS_H_

#include "SDL.h"

void scc(int error_code);
void tcc(int error_code);
void* scp(void* ptr);
void* cp(void* ptr);

void sdl_clean_up(SDL_Window* window, SDL_Renderer* renderer, SDL_Surface* surface, SDL_Texture* texture);

#endif /* UTILS_H_ */