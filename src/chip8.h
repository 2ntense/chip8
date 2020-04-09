#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define MEMORY_SIZE 4096
#define NUM_REGS 16
#define STACK_SIZE 16
#define NUM_KEYS 16

uint8_t chip8_fontset[80] =
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

typedef struct chip8_t
{
    uint8_t mem[MEMORY_SIZE];                       // Memory
    uint8_t V[NUM_REGS];                            // General registers
    uint16_t pc;                                    // Program counter
    uint16_t I;                                     // Special register
    uint8_t sp;                                     // Stack pointer
    uint8_t t_delay;                                // Delay timer
    uint8_t t_sound;                                // Sound timer
    uint8_t frame_buf[SCREEN_WIDTH][SCREEN_HEIGHT]; // Frame buffer
    uint16_t stack[STACK_SIZE];                     // Stack
    uint8_t key[NUM_KEYS];                          // Keys
    uint8_t key_press;                              // Key pressed flag
    uint8_t draw_flag;                              // Draw screen flag
} chip8_t;

#endif