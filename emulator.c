#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "emulator.h"
#include "font.h"

#define FONT_BASE 0    
#define FONT_SIZE 5*16  //5 bytes each * 16 characterss   

Chip8State* InitChip8(){
    Chip8State* c = calloc(sizeof(Chip8State),1);
    c->memory = calloc(1024*4,1); // 4096kb
    c->screen = &c->memory[0xf00]; // we make screen buffer at 0xf00
    c->SP = 0xfa0;
    c->PC = 0x200;

    memcpy(&c->memory[FONT_BASE],font,FONT_SIZE);

    return c;
}


void EmulateChip8(Chip8State* state){
    printf("\nEmulator ");

    uint8_t *op = &state->memory[state->PC];
    int highnib = op[0]>>4;    
    printf("%04x %02x %02x\n",state->PC,op[0],op[1]);

    switch (highnib){
        case 0x00:
            break;
        case 0x01:
            uint16_t target = ((op[0]&0xf)<<8) | op[1];  
            state->PC = target;
            break;
        case 0x02: break;
        case 0x03:
            uint8_t reg = op[0] & 0xf;
            if(state->V[reg]==op[1]){
                state->PC+=2;
            }
            state->PC+2;
            break;
        case 0x04:
            break;
        case 0x05:
            break;
        case 0x06:{
            uint8_t reg = op[0] & 0xf;
            state->V[reg] = op[1];
            state->PC+=2;
            break;
        }
        case 0x07:
            break;
        case 0x08:
            break;
        case 0x09:
            break;
        case 0x0a:
            state->I = (op[0]&0xf)<<8 ||op[1];
            state->PC+=2;
            break;
        case 0x0b:
            break;
        case 0x0c: 
            break;
        case 0x0d:
            break;
        case 0x0e:
            {
            int reg = op[0]&0xf;
            switch(op[1]){
                case 0x9e:
                    if(state->key_state[state->V[reg]]!=0){
                        state->PC+=2;
                    }break;
                case 0xa1:
                    if(state->key_state[state->V[reg]]!=0){
                        state->PC+=2;

                    }break;
                default: 
                    break;
            }
            }
            state->PC+=2;
            break;
        case 0x0f:
            switch(op[1]){
                case 0x33:
                    int reg = op[0]&0xf;
                    uint8_t ones,tens,hundreds;
                    uint8_t value = state->V[reg];
                    ones = value%10;
                    value/=10;
                    tens = value%10;
                    value/=10;
                    hundreds = value/10;
                    state->memory[state->I] = hundreds;
                    state->memory[state->I+1]=tens;
                    state->memory[state->I+2] = ones;
                    break;
                case 0x07:
                    state->V[reg] = state->delay;
                    break;
                case 0x15:
                    state->delay = state->V[reg];
                    break;
                case 0x18:
                    state->sound = state->V[reg];
                    break;
                case 0x29:
                    {
                        int reg = op[0]&0xf;
                        state->I = FONT_BASE+(state->V[reg]*5);
                        break;
                    }
            }
            break;

    }

}