#include "screen.h"

screen_t *init_screen()
{
    screen_t *screen = malloc(sizeof(screen_t));

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }

    screen->window = SDL_CreateWindow("Chip-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WINDOW_WIDTH, SCREEN_WINDOW_HEIGHT, 0);
    if (screen->window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }

    screen->renderer = SDL_CreateRenderer(screen->window, -1, SDL_RENDERER_ACCELERATED);
    if (screen->renderer == NULL)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }

    // SDL_CreateWindowAndRenderer(SCREEN_WINDOW_WIDTH, SCREEN_WINDOW_HEIGHT, 0, &screen->window, &screen->renderer);
    // SDL_SetRenderDrawColor(screen->renderer, 0, 255, 0, 255);
    return screen;
}

void draw_screen(screen_t *screen)
{
    // print_screen(screen);
    SDL_SetRenderDrawColor(screen->renderer, 0, 0, 0, 255);
    SDL_RenderClear(screen->renderer);
    SDL_SetRenderDrawColor(screen->renderer, 0, 255, 0, 255);

    SDL_Rect r = {0,
                  0,
                  1 * SCREEN_WIDTH_SCALE,
                  1 * SCREEN_HEIGHT_SCALE};

    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            if (screen->frame_buf[x][y] == 1)
            {
                r.x = x * SCREEN_WIDTH_SCALE;
                r.y = y * SCREEN_HEIGHT_SCALE;
                SDL_RenderFillRect(screen->renderer, &r);
            }
        }
    }
    SDL_RenderPresent(screen->renderer);
    screen->draw_flag = 0;
}

void print_screen(screen_t *screen)
{
    printf("\e[1;1H\e[2J");

    for (uint8_t y = 0; y < SCREEN_HEIGHT; y++)
    {
        for (uint8_t x = 0; x < SCREEN_WIDTH; x++)
        {
            if (screen->frame_buf[x][y] == 1)
            {
                printf(".");
            }
            else
            {
                printf("#");
            }
        }
        printf("\n");
    }
}

int free_screen(screen_t *screen)
{
    SDL_DestroyWindow(screen->window);
    SDL_Quit();
    free(screen);
}