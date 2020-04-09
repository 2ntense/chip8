#include "screen.h"

screen_t *init_screen()
{
    screen_t *screen = malloc(sizeof(screen_t));

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }

    screen->window = SDL_CreateWindow("Chip-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (screen->window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }

    screen->renderer = SDL_CreateRenderer(screen->window, -1, 0);
    if (screen->renderer == NULL)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_SetRenderDrawColor(screen->renderer, 0, 255, 0, 255);
    return screen;
}


int close_gfx(screen_t *screen)
{
    SDL_DestroyWindow(screen->window);
    SDL_Quit();
}