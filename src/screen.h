#ifndef SCREEN_H
#define SCREEN_H

#include <SDL2/SDL.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

#define SCREEN_WIDTH_SCALE 20
#define SCREEN_HEIGHT_SCALE 20
#define SCREEN_WINDOW_WIDTH SCREEN_WIDTH * SCREEN_WIDTH_SCALE
#define SCREEN_WINDOW_HEIGHT SCREEN_HEIGHT * SCREEN_HEIGHT_SCALE

typedef struct screen_t
{
    uint8_t frame_buf[SCREEN_WIDTH][SCREEN_HEIGHT];
    SDL_Window *window;
    SDL_Renderer *renderer;
    uint8_t draw_flag;
} screen_t;

screen_t *init_screen();
void print_screen(screen_t *);
void draw_screen(screen_t *);
int free_screen(screen_t *);

#endif