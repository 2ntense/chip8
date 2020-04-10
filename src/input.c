#include "input.h"

void on_key_down(SDL_Scancode *code, uint8_t *key)
{
    switch (*code)
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
    default:
        break;
    }
}

void on_key_up(SDL_Scancode *code, uint8_t *key)
{
    switch (*code)
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
}