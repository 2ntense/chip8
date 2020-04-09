#include "chip8.h"
#include <unistd.h>

int main()
{
	screen_t *screen = init_screen();
	chip8_t *chip8 = init_chip8(screen);

	load_program(chip8);

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