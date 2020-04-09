#include "chip8.h"
#include <unistd.h>

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Specify program\n");
		return 1;
	}

	screen_t *screen = init_screen();
	chip8_t *chip8 = init_chip8(screen);

	load_program(chip8, argv[1]);

	while (1)
	{
		emulate_cycle(chip8);
		if (chip8->screen->draw_flag)
			draw(chip8->screen);
		usleep(5000);
		// usleep(500000);
	}

	return 0;
}