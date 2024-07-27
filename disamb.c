#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "emulator.h"
// uint8_t corresponds to 1byte

void DisassembleChip(uint8_t* codebuffer ,int pc){
    
    // pointer to first byte out of two bytes in a file at pc(address)
    
    uint8_t* code = &codebuffer[pc];
    printf("%04x %02x %02x ",pc,code[0],code[1]);
    
    // get the first nibble meaning the first four bits out of 8bits of first bytes
    // to get the op code for eg:- 1NNN getting the 1
    uint8_t firstnib = (code[0]>>4);
    switch(firstnib){
        case 0x00:
            switch(code[1]){
                // e0 meaning clear screen
                case 0xe0: printf("%-10s","CLS");break;
                // ee -> return from a subrouting
                case 0xee : printf("%-10s","RTS");break;
                default : printf("Unknown");break; 
            }
            break;
        case 0x01:
            // 0x1NNN -> jump to address NNN
            printf("%-10s $%01x%02x","JUMP",code[0]&0xf,code[1]);
            break;
        case 0x02:
            // 0x2NNN -> Execute subroutine at NNN
            printf("%-10s $%01x%02x","CALL",code[0]&0xf,code[1]);
            break;
        case 0x03: 
            // 0x3XNN -> Skip the following instruction if the value of register VX equals NN
            printf("%-10s V%01X,#$%02x","SKIP.EQ",code[0]&0xf,code[1]);
            break;
        case 0x04:
            // 0x4XNN -> Skip the following instruction if the value of register VX is not equal to NN
            printf("%-10s V%01X,#$%02x","SKIP.NE",code[0]&0xf,code[1]);
            break;
        case 0x05:
            // 0x5XY0 -> Skip the following instruction if the value of register VX is equal to the value of register VY
            printf("%-10s V%01X,#V%01X","SKIP.EQ",code[0]&0xf,code[1]>>4);
            break;
        case 0x06:
            // 6xkk meaning moving the value kk into register Vx
            //V0 - VF represents register 
            printf("%-10s V%01X,#$%02x","MVI",code[0]&0xf,code[1]);
            break;
        case 0x07:
            printf("%-10s V%01X,#$%02x", "ADI", code[0]&0xf, code[1]);
            break;
        case 0x08:
            // 08 -> represents a lot of instructions ;  it follows a pattern of 0x8XYK
            // where K is 0-E
            uint8_t lastnib = code[1] & 0x0f;
            // printf("last nib %01x",lastnib);
            switch(lastnib){
                case 0:
                    // Store the value of register VY in register VX
                    printf("%-10s V%01X,V%01X","MOV",code[0]&0xf,code[1]>>4);
                    break;
                case 1:
                    // Set VX to VX or VY (VX||VY)
                    printf("%-10s V%01X, V%01X","OR.",code[0]&0xf,code[1]>>4);
                    break;
                case 2:
                    // Set VX to VX and VY (VX & VY)  
                    printf("%-10s V%01X, V%01X","AND.",code[0]&0xf,code[1]>>4);
                    break;
                case 3:
                    // Set VX to VX XOR VY (VX^VY) 
                    printf("%-10s V%01X, V%01X","XOR.",code[0]&0xf,code[1]>>4);
                    break;
                case 4:
                    // Add the value of register VY to register VX 
                    printf("%-10s V%01X, V%01X","ADD.",code[0]&0xf,code[1]>>4);
                    break;
                case 5:
                    // Subtract val(VY) from val(VX)
                    printf("%-10s V%01X, V%01X, V%01X","SUB.",code[0]&0xf,code[0]&0xf,code[1]>>4);
                    break;
                case 6:
                    // Set VX -> VX>>1
                    printf("%-10s V%01X, V%01X","SHR.",code[0]&0xf,code[1]>>4);
                    break;
                case 7:
                    // Set VX to val(VY-VX)
                    printf("%-10s V%01X, V%01X, V%01X","SUB.",code[0]&0xf,code[1]>>4,code[1]>>4);
                    break;
                case 0xe:
                    // Set VX -> VY<<1
                    printf("%-10s V%01X, V%01X","SHL.",code[0]&0xf,code[1]>>4);
                    break;
                default:
                    printf("UNKNOWN 8");
                    break;
            }

            break;
        case 0x9: 
            // Skip, if val(VX)!=val(VY)
            printf("%-10s V%01X,V%01X", "SKIP.NE", code[0]&0xf, code[1]>>4); 
            break;
		case 0xa: 
            //0xANNN  
            // Set I -> NNN
            printf("%-10s I,#$%01x%02x", "MVI", code[0]&0xf, code[1]); 
            break;
		case 0xb: 
            // 0xBNNN 
            // JUMP NNN + V0 
            printf("%-10s $%01x%02x(V0)", "JUMP", code[0]&0xf, code[1]); 
            break;
		case 0xc: 
            // 0xCXNNN
            // Set VX -> to a random number with a mask of NN; meaning (Rand&NN)
            printf("%-10s V%01X,#$%02X", "RNDMSK", code[0]&0xf, code[1]);
             break;
		case 0xd: 
            // Draw a sprite at position VX, VY with N bytes of sprite data starting at the address stored in I
            
            printf("%-10s V%01X,V%01X,#$%01x", "SPRITE", code[0]&0xf, code[1]>>4, code[1]&0xf);
             break;
		case 0xe: 
			switch(code[1])
			{
                // Skip the following instruction if the key corresponding to the hex value currently stored in register VX is pressed
				case 0x9E: printf("%-10s V%01X", "SKIPKEY.Y", code[0]&0xf); break;
				// Skip the following instruction if the key corresponding to the hex value currently stored in register VX is not pressed
                case 0xA1: printf("%-10s V%01X", "SKIPKEY.N", code[0]&0xf); break;
				default: printf("UNKNOWN E"); break;
			}
			break;
		case 0xf: 
			switch(code[1])
			{
                // Store the current value of the delay timer in register VX
				case 0x07: printf("%-10s V%01X,DELAY", "MOV", code[0]&0xf); break;
                // Wait for a keypress and store the result in register VX
				case 0x0a: printf("%-10s V%01X", "KEY", code[0]&0xf); break;
                // Set the delay timer to the value of register VX
				case 0x15: printf("%-10s DELAY,V%01X", "MOV", code[0]&0xf); break;
                // Set the sound timer to the value of register VX
				case 0x18: printf("%-10s SOUND,V%01X", "MOV", code[0]&0xf); break;
                // Add the value stored in register VX to register I
				case 0x1e: printf("%-10s I,V%01X", "ADI", code[0]&0xf); break;
                // Set I to the memory address of the sprite data corresponding to the hexadecimal digit stored in register VX
				case 0x29: printf("%-10s I,V%01X", "SPRITECHAR", code[0]&0xf); break;
                // Store the binary-coded decimal equivalent of the value stored in register VX at addresses I, I + 1, and I + 2
                case 0x33: printf("%-10s (I),V%01X", "MOVBCD", code[0]&0xf); break;
                // Store the values of registers V0 to VX inclusive in memory starting at address I
                // I is set to I + X + 1 after operation²
				case 0x55: printf("%-10s (I),V0-V%01X", "MOVM", code[0]&0xf); break;
                // Fill registers V0 to VX inclusive with the values stored in memory starting at address I
                // I is set to I + X + 1 after operation
				case 0x65: printf("%-10s V0-V%01X,(I)", "MOVM", code[0]&0xf); break;


			}
			break;

    }
}

Chip8State* read_rom(int argc, char const *argv[])
{
    FILE *f = fopen(argv[1],"rb");
    if(f==NULL){
        printf("Error reading the file\n");
        exit(1);
    }
    fseek(f,0L,SEEK_END);
    int fsize = ftell(f);
    fseek(f,0L,SEEK_SET);

    // +0x200 because actual chip8 program starts from 0x200
    // 0x000 to 0x200 is reserved for interpreter and so we are mimicing this

    unsigned char* buffer = malloc(fsize+0x200);    

    // we start reading from 0x200 till the end of file   

    fread(buffer+0x200,fsize,1,f);
    fclose(f);

    // program counter address of instruction to disassemble
    
    // so the pc is within the range of 0x200 - fsize
    Chip8State* c = InitChip8();
    long i = 0;
    
    c->memory = buffer;
    
    // Copying memory

    for(long i=0;i<fsize;i++){
        c->memory[0x200+i] = buffer[i+0x200];
    }

    int pc = 0x200;
    printf("Memory Copied Succesfully\n");
    while(pc<(fsize+0x200)){
        DisassembleChip(buffer,pc);
        // Increasing +2 since in chip8 each instruction is contained in 1byte 0x200-2(one ins.) 0x202-4(second)
        pc+=2;
        if(c->halt==1){
            break;
        }
        // c->PC+=2;
        printf("\n");
    }

    return c;

    // printf("Emulation Finsihed\n");
}
