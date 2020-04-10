#include "chip8.h"
#include "instructions.h"
#include <sys/stat.h>

chip8_t *init_chip8(screen_t *screen, uint8_t *key)
{
	chip8_t *chip8 = malloc(sizeof(chip8_t));
	chip8->pc = 0x200;
	chip8->I = 0;
	chip8->sp = 0;
	chip8->t_delay = 0;
	chip8->t_sound = 0;
	memset(chip8->mem, 0, sizeof(chip8->mem));
	memset(chip8->V, 0, sizeof(chip8->V));
	memset(chip8->stack, 0, sizeof(chip8->stack));

	chip8->key = key;
	memset(chip8->key, 0, sizeof(chip8->key));

	chip8->screen = screen;
	memset(chip8->screen->frame_buf, 0, sizeof(chip8->screen->frame_buf));

	memcpy(chip8->mem, chip8_fontset, sizeof(chip8_fontset));

	printf("CHIP-8 initialized\n");

	return chip8;
}

int load_program(chip8_t *chip8, char *file_path)
{
	FILE *f = fopen(file_path, "rb");

	struct stat s;
	stat(file_path, &s);

	if (s.st_size > (0xFFF - 0x200))
	{
		printf("Program too big\n");
		return -1;
	}

	size_t ret = fread(chip8->mem + 0x200, 1, s.st_size, f);

	if (ret != s.st_size)
	{
		printf("Program loaded unsuccesfully\n");
		return -1;
	}
	printf("Program loaded succesfully\n");
	return 1;
}

void emulate_cycle(chip8_t *chip8)
{
	process_opcode(chip8);
}

void dec_timers(chip8_t *chip8)
{
	if (chip8->t_delay > 0)
	{
		chip8->t_delay--;
	}
	if (chip8->t_sound > 0)
	{
		chip8->t_sound--;
	}
}

void free_chip8(chip8_t *chip8)
{
	free(chip8);
}
