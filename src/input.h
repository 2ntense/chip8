#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>
#include <SDL2/SDL.h>

#define NUM_KEYS 16

uint8_t key[NUM_KEYS];

void on_key_down(SDL_Scancode *);
void on_key_up(SDL_Scancode *);

#endif