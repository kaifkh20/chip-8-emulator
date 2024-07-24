#include "disamb.h"
#include "emulator.h"

int main(int argc, char const *argv[])
{
    read_rom(argc,argv);
    Chip8State* state = InitChip8();
    EmulateChip8(state);
    return 0;
}

