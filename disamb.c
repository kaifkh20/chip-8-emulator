#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
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
            printf("0 not handled as of now");break;
        case 0x01:
            printf("1 not handled");break;
        case 0x02:
            printf("2 not handled yet");break;
        case 0x03: 
            printf("3 not handled"); break;
        case 0x04:
            printf("4 not handled"); break;
        case 0x05:
            printf("5 not handled yet"); break;
        case 0x06:
            // 6xkk meaning moving the value kk into register Vx
            //V0 - VF represents register 
            uint8_t reg = code[0]&0x0f;
            
            printf("%-10s V%01x,#$%02x","MVI",reg,code[1]);
            break;
        
        case 0x07: printf("7 not handled yet"); break;    
        case 0x08: printf("8 not handled yet"); break;    
        case 0x09: printf("9 not handled yet"); break;    
        case 0x0a:    
            {    
                // Annn meanings value of reg I is set nnn
                // N -> immediate values ; NN -> for values ; NNN -> address 
                // addresshi get the first n out of first byte
                uint8_t addresshi = code[0] & 0x0f;    
                printf("%-10s I,#$%01x%02x", "MVI", addresshi, code[1]);    
            }    
            break;    
        case 0x0b: printf("b not handled yet"); break;    
        case 0x0c: printf("c not handled yet"); break;    
        case 0x0d: printf("d not handled yet"); break;    
        case 0x0e: printf("e not handled yet"); break;    
        case 0x0f: printf("f not handled yet"); break;

    }
}

int main(int argc, char const *argv[])
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
    int pc = 0x200;
    
    // so the pc is within the range of 0x200 - fsize
    while(pc<(fsize+0x200)){
        DisassembleChip(buffer,pc);
        // Increasing +2 since in chip8 each instruction is contained in 1byte 0x200-2(one ins.) 0x202-4(second)
        pc+=2;
        printf("\n");
    }

    return 0;
}
