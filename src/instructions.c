#include "instructions.h"

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
                uint8_t abs_x = (x + xline) % SCREEN_WIDTH;
                uint8_t abs_y = (y + yline) % SCREEN_HEIGHT;
                chip8->V[0xF] |= (chip8->screen->frame_buf[abs_x][abs_y]);
                chip8->screen->frame_buf[abs_x][abs_y] ^= 1;
            }
        }
    }
}

void process_opcode(chip8_t *chip8)
{
    uint16_t opcode = chip8->mem[chip8->pc] << 8 | chip8->mem[chip8->pc + 1];
    printf("opcode: %04x\n", opcode);
    uint16_t a, b, c = 0;

    switch (opcode & 0xF000)
    {
    case 0x0000:
        switch (opcode & 0x00FF)
        {
        case 0x00E0:
            memset(chip8->screen->frame_buf, 0, sizeof(chip8->screen->frame_buf));
            chip8->screen->draw_flag = 1;
            break;
        case 0x00EE:
            chip8->sp--;
            chip8->pc = chip8->stack[chip8->sp];
            break;
        default:
            // Calls RCA 1802 program at address 0x0NNN. Not necessary for most ROMs.
            break;
        }
        inc_pc(chip8);
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
        {
            inc_pc(chip8);
        }
        inc_pc(chip8);
        break;
    case 0x4000:
        if (chip8->V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
        {
            inc_pc(chip8);
        }
        inc_pc(chip8);
        break;
    case 0x5000:
        if (chip8->V[(opcode & 0x0F00) >> 8] == chip8->V[(opcode & 0x00F0) >> 4])
        {
            inc_pc(chip8);
        }
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
            chip8->V[0xF] = chip8->V[(opcode & 0x0F00) >> 8] & 1;
            chip8->V[(opcode & 0x0F00) >> 8] >>= 1;
            break;
        case 0x0007:
            chip8->V[0xF] = chip8->V[(opcode & 0x00F0) >> 4] > chip8->V[(opcode & 0x0F00) >> 8] ? 1 : 0;
            chip8->V[(opcode & 0x00F0) >> 4] = chip8->V[(opcode & 0x00F0) >> 4] - chip8->V[(opcode & 0x0F00) >> 8];
            break;
        case 0x000E:
            // chip8->V[0xF] = chip8->V[(opcode & 0x0F00) >> 8] & 1 == 1 ? 1 : 0;
            chip8->V[0xF] = chip8->V[(opcode & 0x0F00) >> 8] >> 7;
            chip8->V[(opcode & 0x0F00) >> 8] <<= 1;
            break;
        default:
            break;
        }
        inc_pc(chip8);
        break;
    case 0x9000:
        if (chip8->V[(opcode & 0x0F00) >> 8] != chip8->V[(opcode & 0x00F0) >> 4])
        {
            inc_pc(chip8);
        }
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
        a = chip8->V[(opcode & 0x0F00) >> 8]; // x
        b = chip8->V[(opcode & 0x00F0) >> 4]; // y
        c = opcode & 0x000F;                  // size
        drw(a, b, c, chip8);
        chip8->screen->draw_flag = 1;
        inc_pc(chip8);
        break;
    case 0xE000:
        switch (opcode & 0x00FF)
        {
        case 0x009E:
            if (chip8->key[chip8->V[(opcode & 0xF00) >> 8]])
            {
                inc_pc(chip8);
            }
            break;
        case 0x00A1:
            if (!chip8->key[chip8->V[(opcode & 0xF00) >> 8]])
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
            for (a = 0; a < sizeof(chip8->key); a++)
            {
                if (chip8->key[a])
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
            chip8->I = chip8->V[(opcode & 0x0F00) >> 8] * 5;
            break;
        case 0x0033:
            a = (opcode & 0x0F00) >> 8;
            chip8->mem[chip8->I] = chip8->V[a] / 100 * 100;
            chip8->mem[chip8->I + 1] = chip8->V[a] % 100 / 10 * 10;
            chip8->mem[chip8->I + 2] = chip8->V[a] % 10;
            break;
        case 0x0055:
            for (a = 0; a <= ((opcode & 0x0F00) >> 8); a++)
            {
                chip8->mem[chip8->I + a] = chip8->V[a];
            }
            break;
        case 0x0065:
            for (a = 0; a <= ((opcode & 0x0F00) >> 8); a++)
            {
                chip8->V[a] = chip8->mem[chip8->I + a];
            }
            break;
        default:
            break;
        }
        inc_pc(chip8);
        break;
    }
}
