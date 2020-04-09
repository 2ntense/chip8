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
	chip8_t *chip8 = init_chip8(screen);

	load_program(chip8, argv[1]);

	// while (1)
	// {
	// 	emulate_cycle(chip8);
	// 	if (chip8->screen->draw_flag)
	// 		draw_screen(chip8->screen);
	// 	usleep(5000);
	// 	// usleep(500000);
	// }

	SDL_bool quit = SDL_FALSE;
	SDL_Event event;
	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				quit = SDL_TRUE;
				break;
			default:
				break;
			}
		}
		emulate_cycle(chip8);
		draw_screen(chip8->screen);
			// usleep(5000);
	}

	free_screen(screen);
	free_chip8(chip8);

	return 0;
}