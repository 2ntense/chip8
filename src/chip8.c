#include "chip8.h"
#include <stdlib.h>
#include <sys/stat.h>
#include "input.h"

chip8_t *init_chip8(screen_t *screen)
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
	memset(key, 0, sizeof(key));

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

uint8_t spr_addr(uint8_t c)
{
	if (c > 0xF)
		return 0;

	return c;
}

void inc_pc(chip8_t *chip8)
{
	chip8->pc += 2;
}

void drw(uint8_t x, uint8_t y, uint8_t size, chip8_t *chip8)
{
	chip8->V[0xF] = 0;
	for (int yline = 0; yline < size; yline++)
	{
		uint8_t pixels = chip8->mem[chip8->I + yline];
		for (int xline = 0; xline < 8; xline++)
		{
			if ((pixels & (0x80 >> xline)) != 0)
			{
				uint8_t abs_x = x + xline;
				uint8_t abs_y = y + yline;
				chip8->V[0xF] = (chip8->screen->frame_buf[abs_x % SCREEN_WIDTH][abs_y % SCREEN_HEIGHT] == 1) ? 1 : 0;
				chip8->screen->frame_buf[abs_x % SCREEN_WIDTH][abs_y % SCREEN_HEIGHT] ^= 1;
			}
		}
	}

	chip8->screen->draw_flag = 1;
}

void emulate_cycle(chip8_t *chip8)
{
	// Fetch Opcode
	uint16_t opcode = chip8->mem[chip8->pc] << 8 | chip8->mem[chip8->pc + 1];
	// inc_pc(chip8);
	// printf("pc: %04x\topcode: %04x\tI: %04x\n", chip8->pc, opcode, chip8->I);
	// Decode Opcode
	uint16_t a, b, c, d = 0;
	switch (opcode & 0xF000)
	{
	case 0x0000:
		switch (opcode & 0x00FF)
		{
		case 0x00E0:
			memset(chip8->screen->frame_buf, 0, sizeof(chip8->screen->frame_buf));
			chip8->screen->draw_flag = 1;
			inc_pc(chip8);
			break;
		case 0x00EE:
			chip8->sp--;
			chip8->pc = chip8->stack[chip8->sp];
			break;
		default:
			// Calls RCA 1802 program at address 0x0NNN. Not necessary for most ROMs.
			break;
		}
		break;
	case 0x1000:
		chip8->pc = opcode & 0x0FFF;
		break;
	case 0x2000:
		chip8->stack[chip8->sp] = chip8->pc;
		chip8->sp++;
		chip8->pc = opcode & 0x0FFF;
		break;
	case 0x3000:
		if (chip8->V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
			inc_pc(chip8);
		inc_pc(chip8);
		break;
	case 0x4000:
		if (chip8->V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
			inc_pc(chip8);
		inc_pc(chip8);
		break;
	case 0x5000:
		if (chip8->V[(opcode & 0x0F00) >> 8] == chip8->V[(opcode & 0x00F0) >> 4])
			inc_pc(chip8);
		inc_pc(chip8);
		break;
	case 0x6000:
		chip8->V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
		inc_pc(chip8);
		break;
	case 0x7000:
		chip8->V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
		inc_pc(chip8);
		break;
	case 0x8000:
		switch (opcode & 0x000F)
		{
		case 0x0000:
			chip8->V[(opcode & 0x0F00) >> 8] = chip8->V[(opcode & 0x00F0) >> 4];
			break;
		case 0x0001:
			chip8->V[(opcode & 0x0F00) >> 8] |= chip8->V[(opcode & 0x00F0) >> 4];
			break;
		case 0x0002:
			chip8->V[(opcode & 0x0F00) >> 8] &= chip8->V[(opcode & 0x00F0) >> 4];
			break;
		case 0x0003:
			chip8->V[(opcode & 0x0F00) >> 8] ^= chip8->V[(opcode & 0x00F0) >> 4];
			break;
		case 0x0004:
			chip8->V[0xF] = chip8->V[(opcode & 0x0F00) >> 8] + chip8->V[(opcode & 0x00F0) >> 4] > 0xFF ? 1 : 0;
			chip8->V[(opcode & 0x0F00) >> 8] += chip8->V[(opcode & 0x00F0) >> 4];
			break;
		case 0x0005:
			chip8->V[0xF] = chip8->V[(opcode & 0x0F00) >> 8] > chip8->V[(opcode & 0x00F0) >> 4] ? 1 : 0;
			chip8->V[(opcode & 0x0F00) >> 8] = chip8->V[(opcode & 0x0F00) >> 8] - chip8->V[(opcode & 0x00F0) >> 4];
			break;
		case 0x0006:
			chip8->V[0xF] = chip8->V[(opcode & 0x0F00) >> 8] & 1 == 1 ? 1 : 0;
			chip8->V[(opcode & 0x0F00) >> 8] >>= 1;
			break;
		case 0x0007:
			chip8->V[0xF] = chip8->V[(opcode & 0x00F0) >> 4] > chip8->V[(opcode & 0x0F00) >> 8] ? 1 : 0;
			chip8->V[(opcode & 0x00F0) >> 4] = chip8->V[(opcode & 0x00F0) >> 4] - chip8->V[(opcode & 0x0F00) >> 8];
			break;
		case 0x000E:
			chip8->V[0xF] = chip8->V[(opcode & 0x0F00) >> 8] & 1 == 1 ? 1 : 0;
			chip8->V[(opcode & 0x0F00) >> 8] <<= 1;
			break;
		default:
			break;
		}
		inc_pc(chip8);
		break;
	case 0x9000:
		if (chip8->V[(opcode & 0x0F00) >> 8] != chip8->V[(opcode & 0x00F0) >> 4])
			inc_pc(chip8);
		inc_pc(chip8);
		break;
	case 0xA000:
		chip8->I = opcode & 0x0FFF;
		inc_pc(chip8);
		break;
	case 0xB000:
		chip8->pc = (opcode & 0x0FFF) + chip8->V[0];
		break;
	case 0xC000:
		chip8->V[(opcode & 0x0F00) >> 8] = (rand() % 256) & (opcode & 0x00FF);
		inc_pc(chip8);
		break;
	case 0xD000:
		// a = chip8->V[(opcode & 0x0F00) >> 8]; // x
		// b = chip8->V[(opcode & 0x00F0) >> 4]; // y
		// c = opcode & 0x000F;									// amount
		// // printf("drawing x: %d, y: %d, n: %d\n", a, b, c);
		// chip8->V[0xF] = 0;
		// for (int yline = 0; yline < c; yline++)
		// {
		// 	d = chip8->mem[chip8->I + yline];
		// 	for (int xline = 0; xline < 8; xline++)
		// 	{
		// 		if ((d & (0x80 >> xline)) != 0)
		// 		{
		// 			chip8->V[0xF] = (chip8->screen->frame_buf[a + xline][b + yline] == 1) ? 1 : 0;
		// 			chip8->screen->frame_buf[a + xline][b + yline] ^= 1;
		// 		}
		// 	}
		// }

		// chip8->screen->draw_flag = 1;
		a = chip8->V[(opcode & 0x0F00) >> 8]; // x
		b = chip8->V[(opcode & 0x00F0) >> 4]; // y
		c = opcode & 0x000F;									// amount
		drw(a, b, c, chip8);
		inc_pc(chip8);
		break;
	case 0xE000:
		switch (opcode & 0x00FF)
		{
		case 0x009E:
			if (key[chip8->V[(opcode & 0xF00) >> 8]])
			{
				inc_pc(chip8);
			}
			break;
		case 0x00A1:
			if (!key[chip8->V[(opcode & 0xF00) >> 8]])
			{
				inc_pc(chip8);
			}
			break;
		default:
			break;
		}
		inc_pc(chip8);
		break;
	case 0xF000:
		switch (opcode & 0x00FF)
		{
		case 0x0007:
			chip8->V[(opcode & 0x0F00) >> 8] = chip8->t_delay;
			break;
		case 0x000A:
			for (a = 0; a < NUM_KEYS; a++)
			{
				printf("key[%x] == %d\n", a, key[a]);
			}
			for (a = 0; a < NUM_KEYS; a++)
			{
				if (key[a])
				{
					chip8->V[(opcode & 0x0F00) >> 8] = a;
					inc_pc(chip8);
					break;
				}
			}
			chip8->pc -= 2;
			break;
		case 0x0015:
			chip8->t_delay = chip8->V[(opcode & 0x0F00) >> 8];
			break;
		case 0x0018:
			chip8->t_sound = chip8->V[(opcode & 0x0F00) >> 8];
			break;
		case 0x001E:
			chip8->I += (opcode & 0x0F00) >> 8;
			break;
		case 0x0029:
			chip8->I = spr_addr(chip8->V[(opcode & 0x0F00) >> 8]);
			break;
		case 0x0033:
			a = (opcode & 0x0F00) >> 8;
			chip8->mem[chip8->I] = chip8->V[a] / 100;
			chip8->mem[chip8->I + 1] = chip8->V[a] % 100 / 10;
			chip8->mem[chip8->I + 2] = chip8->V[a] % 100 % 10;
			break;
		case 0x0055:
			memcpy(chip8->mem + chip8->I, chip8->V, (opcode & 0x0F00) >> 8);
			break;
		case 0x0065:
			memcpy(chip8->V, chip8->mem + chip8->I, (opcode & 0x0F00) >> 8);
			break;
		default:
			break;
		}
		inc_pc(chip8);
		break;
	}
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