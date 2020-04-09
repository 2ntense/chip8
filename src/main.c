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
				// key_pressed = event.key.keysym.scancode;

				switch (event.key.keysym.scancode)
				{
				case SDL_SCANCODE_1:
					key[0x1] = 1;
					break;
				case SDL_SCANCODE_2:
					key[0x2] = 1;
					break;
				case SDL_SCANCODE_3:
					key[0x3] = 1;
					break;
				case SDL_SCANCODE_4:
					key[0xC] = 1;
					break;
				case SDL_SCANCODE_Q:
					key[0x4] = 1;
					break;
				case SDL_SCANCODE_W:
					key[0x5] = 1;
					break;
				case SDL_SCANCODE_E:
					key[0x6] = 1;
					break;
				case SDL_SCANCODE_R:
					key[0xD] = 1;
					break;
				case SDL_SCANCODE_A:
					key[0x7] = 1;
					break;
				case SDL_SCANCODE_S:
					key[0x8] = 1;
					break;
				case SDL_SCANCODE_D:
					key[0x9] = 1;
					break;
				case SDL_SCANCODE_F:
					key[0xE] = 1;
					break;
				case SDL_SCANCODE_Z:
					key[0xA] = 1;
					break;
				case SDL_SCANCODE_X:
					key[0x0] = 1;
					break;
				case SDL_SCANCODE_C:
					key[0xB] = 1;
					break;
				case SDL_SCANCODE_V:
					key[0xF] = 1;
					break;
				case SDL_SCANCODE_ESCAPE:
					quit = SDL_TRUE;
					break;
				default:
					break;
				}

				break;
			case SDL_KEYUP:
				switch (event.key.keysym.scancode)
				{
				case SDL_SCANCODE_1:
					key[0x1] = 0;
					break;
				case SDL_SCANCODE_2:
					key[0x2] = 0;
					break;
				case SDL_SCANCODE_3:
					key[0x3] = 0;
					break;
				case SDL_SCANCODE_4:
					key[0xC] = 0;
					break;
				case SDL_SCANCODE_Q:
					key[0x4] = 0;
					break;
				case SDL_SCANCODE_W:
					key[0x5] = 0;
					break;
				case SDL_SCANCODE_E:
					key[0x6] = 0;
					break;
				case SDL_SCANCODE_R:
					key[0xD] = 0;
					break;
				case SDL_SCANCODE_A:
					key[0x7] = 0;
					break;
				case SDL_SCANCODE_S:
					key[0x8] = 0;
					break;
				case SDL_SCANCODE_D:
					key[0x9] = 0;
					break;
				case SDL_SCANCODE_F:
					key[0xE] = 0;
					break;
				case SDL_SCANCODE_Z:
					key[0xA] = 0;
					break;
				case SDL_SCANCODE_X:
					key[0x0] = 0;
					break;
				case SDL_SCANCODE_C:
					key[0x0] = 0;
					break;
				case SDL_SCANCODE_V:
					key[0xF] = 0;
					break;
				default:
					break;
				}
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