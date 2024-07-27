#ifndef DISAMB_H

#define DISAMB_H

#include <stdint.h>
#include "emulator.h"

void DisassembleChip(uint8_t* codebuffer ,int pc);

Chip8State* read_rom(int argc,char const* argv[]);

#endif