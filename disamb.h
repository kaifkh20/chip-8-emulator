#ifndef DISAMB_H

#define DISAMB_H

#include <stdint.h>

void DisassembleChip(uint8_t* codebuffer ,int pc);
int read_rom(int argc,char const* argv[]);

#endif