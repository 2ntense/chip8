#ifndef SCREEN_H
#define SCREEN_H

#include <SDL2/SDL.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

typedef struct screen_t
{
    uint8_t frame_buf[SCREEN_WIDTH][SCREEN_HEIGHT];
    SDL_Window *window;
    SDL_Renderer *renderer;
    uint8_t draw_flag;
} screen_t;

screen_t *init_screen();
void draw(screen_t *);
int close_gfx(screen_t *);

#endif