#ifndef EMULATOR_H

#define EMULATOR_H

#include <stdint.h>

typedef struct Chip8State{
    uint8_t V[16]; // 8bits 16 register 
    uint16_t I;     // 16bit register for adress storage
    uint16_t SP;    // 16bit stack pointer at 0xfA0
    uint16_t PC; // program counter
    uint8_t delay; //time delay
    uint8_t sound; //sound delay
    uint8_t *memory; // ram of chip8 4096KB
    uint8_t *screen; //screen pointer at 0xF00 
    uint8_t key_state[16]; // to store key state
    uint8_t save_key_state[16]; // to save current state of keyboard
    int waiting_for_key;
    uint8_t halt;
}Chip8State;

Chip8State* InitChip8(void);

void EmulateChip8(Chip8State* state);

#endif
