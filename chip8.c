#include <SDL2/SDL.h>
#include <stdlib.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

unsigned char chip8_fontset[80] =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

unsigned char mem[4096];
// registers
unsigned short pc;
unsigned char V[16];
unsigned short I;
unsigned char sp;
unsigned char t_delay;
unsigned char t_sound;

unsigned char screen[SCREEN_WIDTH][SCREEN_HEIGHT];
unsigned short opcode;
unsigned short stack[16];
unsigned char key[16];
unsigned char key_press;

int init_chip8()
{
    memset(mem, 0, sizeof(mem));
    pc = 0x200;
    memset(V, 0, sizeof(V));
    I = 0;
    sp = 0;
    t_delay = 0;
    t_sound = 0;
    memset(screen, 0, sizeof(screen));
    opcode = 0;
    memset(stack, 0, sizeof(stack));
    memcpy(mem, chip8_fontset, sizeof(chip8_fontset));

    printf("CHIP-8 cleared and initialized\n");
}

int load_program()
{
    FILE *f = fopen("roms/AIRPLANE.ch8", "rb");
    size_t ret = fread(mem + 0x200, 1, 38, f);

    if (ret != 38)
    {
        printf("program loaded unsuccesfully\n");
        return -1;
    }
    printf("Program loaded succesfully\n");
    return 1;
}

int init_gfx()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    window = SDL_CreateWindow("Chip-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    return 1;
}

int close_gfx()
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int close()
{
    close_gfx();
}

int init()
{
    init_gfx();
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    SDL_Point p0 = {1, 1};
    SDL_Point p1 = {3, 3};
    SDL_Point points[2] = {p0, p1};

    SDL_RenderDrawPoints(renderer, points, sizeof(points) / sizeof(points[0]));

    SDL_RenderPresent(renderer);

    //Wait two seconds
    SDL_Delay(5000);

    return 0;
}

void draw_screen()
{
}

unsigned char spr_addr(unsigned char c)
{
    if (c > 0xF)
        return 0;

    return c;
}

void emulate_cycle()
{
    // Fetch Opcode
    opcode = mem[pc] << 8 | mem[pc + 1];
    printf("opcode: 0x%x\n", opcode);
    // Decode Opcode
    unsigned short tmp;
    switch (opcode & 0xF000)
    {
    case 0x0000:
        switch (opcode & 0x00FF)
        {
        case 0x00E0:
            // clear screen
            break;
        case 0x00EE:
            // return from a subroutine
            pc = stack[sp];
            if (sp > 0)
                sp--;
            break;
        default:
            // Calls RCA 1802 program at address 0x0NNN. Not necessary for most ROMs.
            break;
        }
        break;
    case 0x1000:
        pc = opcode & 0x0FFF;
        break;
    case 0x2000:
        sp++;
        stack[sp] = pc;
        pc = opcode & 0x0FFF;
        break;
    case 0x3000:
        if (V[opcode & 0x0F00 >> 8] == opcode & 0x00FF)
            pc += 2;
        break;
    case 0x4000:
        if (V[opcode & 0x0F00 >> 8] != opcode & 0x00FF)
            pc += 2;
        break;
    case 0x5000:
        if (V[opcode & 0x0F00 >> 8] == V[opcode & 0x00F0 >> 4])
            pc += 2;
        break;
    case 0x6000:
        V[opcode & 0x0F00 >> 8] = opcode & 0x00FF;
        break;
    case 0x7000:
        V[opcode & 0x0F00 >> 8] = V[opcode & 0x0F00 >> 8] + opcode & 0x00FF;
        break;
    case 0x8000:
        switch (opcode & 0x000F)
        {
        case 0x0000:
            V[opcode & 0x0F00 >> 8] = V[opcode & 0x00F0 >> 4];
            break;
        case 0x0001:
            V[opcode & 0x0F00 >> 8] = V[opcode & 0x0F00 >> 8] | V[opcode & 0x00F0 >> 4];
            break;
        case 0x0002:
            V[opcode & 0x0F00 >> 8] = V[opcode & 0x0F00 >> 8] & V[opcode & 0x00F0 >> 4];
            break;
        case 0x0003:
            V[opcode & 0x0F00 >> 8] = V[opcode & 0x0F00 >> 8] ^ V[opcode & 0x00F0 >> 4];
            break;
        case 0x0004:
            tmp = V[opcode & 0x0F00 >> 8] + V[opcode & 0x00F0 >> 4];
            V[0xF] = tmp > 0xFF ? 1 : 0;
            V[opcode & 0x0F00 >> 4] = tmp & 0x000F;
            break;
        case 0x0005:
            V[0xF] = V[opcode & 0x0F00 >> 8] > V[opcode & 0x00F0 >> 4] ? 1 : 0;
            V[opcode & 0x0F00 >> 8] = V[opcode & 0x0F00 >> 8] - V[opcode & 0x00F0 >> 4];
            break;
        case 0x0006:
            V[0xF] = V[opcode & 0x0F00 >> 8] & 1 == 1 ? 1 : 0;
            V[opcode & 0x0F00 >> 8] = V[opcode & 0x0F00 >> 8] >> 1;
            break;
        case 0x0007:
            V[0xF] = V[opcode & 0x00F0 >> 4] > V[opcode & 0x0F00 >> 8] ? 1 : 0;
            V[opcode & 0x00F0 >> 4] = V[opcode & 0x00F0 >> 4] - V[opcode & 0x0F00 >> 8];
            break;
        case 0x000E:
            V[0xF] = V[opcode & 0x0F00 >> 8] & 1 == 1 ? 1 : 0;
            V[opcode & 0x0F00 >> 8] = V[opcode & 0x0F00 >> 8] << 1;
            break;
        default:
            break;
        }
        break;
    case 0x9000:
        if (V[opcode & 0x0F00 >> 8] != V[opcode & 0x00F0 >> 4])
            pc += 2;
        break;
    case 0xA000:
        I = opcode & 0x0FFF;
        break;
    case 0xB000:
        pc = (opcode & 0x0FFF) + V[0];
        break;
    case 0xC000:
        V[opcode & 0x0F00 >> 8] = (rand() % 256) & (opcode & 0x00FF);
        break;
    case 0xD000:
        // TODO
        break;
    case 0xE000:
        switch (opcode & 0x00FF)
        {
        case 0x009E:
            /* code */
            break;
        case 0x00A1:
            /* code */
            break;
        default:
            break;
        }
        break;
    case 0xF000:
        switch (opcode & 0x00FF)
        {
        case 0x0007:
            V[opcode & 0x0F00 >> 2] = t_delay;
            break;
        case 0x000A:
            key_press = 0;
            while (!key_press)
                ;
            V[opcode & 0x0F00 >> 2] = key_press;
            break;
        case 0x0015:
            t_delay = V[opcode & 0x0F00 >> 8];
            break;
        case 0x0018:
            t_sound = V[opcode & 0x0F00 >> 8];
            break;
        case 0x001E:
            I += opcode & 0x0F00 >> 8;
            break;
        case 0x0029:
            I = spr_addr(V[opcode & 0x0F00 >> 2]);
            break;
        case 0x0033:
            tmp = opcode & 0x0F00 >> 2;
            mem[I] = V[tmp] / 100;
            mem[I + 1] = V[tmp] % 100 / 10;
            mem[I + 2] = V[tmp] % 100 % 10;
            break;
        case 0x0055:
            memcpy(mem + I, V, opcode & 0x0F00 >> 2);
            break;
        case 0x0065:
            memcpy(V, mem + I, opcode & 0x0F00 >> 2);
            break;
        default:
            break;
        }
        break;
    }

    // Execute Opcode

    // Update timers
}

int main()
{
    init_chip8();
    load_program();

    while (1)
    {
        emulate_cycle();
    }
}