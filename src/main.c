#include "chip8.h"
#include <unistd.h>

int main(int argc, char **argv)
{

	if (argc < 2)
	{
		printf("Specify program\n");
		return 1;
	}

	screen_t *screen = init_screen();
	uint8_t key[NUM_KEYS];
	chip8_t *chip8 = init_chip8(screen, key);

	load_program(chip8, argv[1]);

	SDL_bool quit = SDL_FALSE;
	SDL_Event event;
	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
				printf("%d\n", event.key.keysym.scancode);
				if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				{
					quit = SDL_TRUE;
				}
				on_key_down(&event.key.keysym.scancode, chip8->key);
				break;
			case SDL_KEYUP:
				on_key_up(&event.key.keysym.scancode, chip8->key);
				break;
			case SDL_QUIT:
				quit = SDL_TRUE;
				break;
			default:
				break;
			}
		}
		emulate_cycle(chip8);
		draw_screen(chip8->screen);
		// usleep(50000);
	}

	free_screen(screen);
	free_chip8(chip8);

	return 0;
}