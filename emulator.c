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
    c->delay = 0;
    memcpy(&c->memory[FONT_BASE],font,FONT_SIZE);

    return c;
}

// uint8_t time = 0;

void EmulateChip8(Chip8State* state){
    printf("\nEmulator ");


    if(state->halt==1){
        printf("Emulation Finished\n");
        exit(1);

        // return ;
    }

    uint8_t *op = &state->memory[state->PC];
    int highnib = op[0]>>4;    
    printf("%04x %02x %02x, %d\n",state->PC,op[0],op[1],highnib);

    switch (highnib){
        case 0x00:
            {
                switch(op[1]){
                    case 0xE0:
                        memset(state->screen,0,64*32/8);
                        state->PC+=2;
                        break;
                    case 0xEE:
                        uint16_t target = (state->memory[state->SP]<<8) | state->memory[state->SP+1];
                        state->SP+=2;
                        state->PC = target;
                        break;
                    default:
                        printf("Unimplemented Instruction\n ");
                        exit(1);
                        // state->PC+=2;
                        break;
                }
            }
            break;
        case 0x01:
        {
            uint16_t target = ((op[0]&0xf)<<8) | op[1];  
            if(target==state->PC){
                printf("Infinite Loop Detected. Setting halt flag\n");
                state->halt = 1;
            }
            state->PC = target;
            break;
        }
        case 0x02: 
            {
                state->SP-=2;
                state->memory[state->SP] = ((state->PC+2)&0xff00)>>8;
                state->memory[state->SP+1] = (state->PC+2)&0xff;
                state->PC = ((op[0]&0xf)<<8) | op[1];
                break;
            }
        case 0x03:{
                uint8_t reg = op[0] & 0xf;
                if(state->V[reg]==op[1]){
                    state->PC+=2;
                }
                state->PC+=2;
                break;
            }
        case 0x04:
            {
                uint8_t reg = op[0] & 0xf;
                if(state->V[reg]!=op[1]){
                    state->PC+=2;
                }
                state->PC+=2;
                break;
            }
        case 0x05:
            {
                uint8_t reg1 = op[0]&0xf;
                uint8_t reg2 = op[1]>>4;
                if(state->V[reg1]==state->V[reg2]){
                    state->PC+=2;
                }
                state->PC+=2;
                break;
            }
        case 0x06:{
            uint8_t reg = op[0] & 0xf;
            state->V[reg] = op[1];
            state->PC+=2;
            break;
        }
        case 0x07:
            {
                uint8_t reg = op[0] & 0xf;
                state->V[reg] +=op[1];
                state->PC+=2;
                break;
            }
        case 0x08:
            {
                int lastnib = op[1]&0xf;
                uint8_t vx = op[0]&0xf;
                uint8_t vy = op[1]>>4;

                switch (lastnib)
                {
                    case 0:
                        state->V[vx] = state->V[vy];           
                        break;
                    case 1:
                        state->V[vx] |= state->V[vy];
                        break;
                    case 2:
                        state->V[vx]&=state->V[vy];
                        break;
                    case 3:
                        state->V[vx]^=state->V[vy];
                        break;
                    case 4:
                        {
                            uint16_t res = state->V[vx] + state->V[vy];
                            if( res & 0xff00) state->V[0xf] = 1;
                            else state->V[0xf] = 0;
                            state->V[vx]= res&0xff;
                            break;
                        }

                    case 5:
                        {
                            int borrow = (state->V[vx] > state->V[vy]);
                            state->V[vx] -= state->V[vy];
                            state->V[0xf]= borrow;
                            break; 
                        }
                    case 6:
                        {
                            uint8_t vf = state->V[vx] & 0x1;
                            state->V[vx] = state->V[vx] >>1;
                            state->V[0xf] = vf;
                            break;
                        }
                    case 7:
                        {
                            int borrow = (state->V[vy] > state->V[vx]);
                            state->V[vx] = state->V[vy] - state->V[vx];
                            state->V[0xf] = borrow; 
                            break;
                        }
                    case 8:
                        {
                            uint8_t vf = (0x80==(state->V[vx]&0x80));
                            state->V[vx] = state->V[vx]<<1;
                            state->V[0xf] =vf;
                            break;
                        }
                default:
                    break;
                }
            state->PC+=2;
            break;
            }
        case 0x09:
            {
                uint8_t reg1 = op[0]&0xf;
                uint8_t reg2 = op[1]>>4;
                if(state->V[reg1]!=state->V[reg2]){
                    state->PC+=2;
                }
                state->PC+=2;
                break;
            }
        case 0x0a:
        {
            state->I = (op[0]&0xf)<<8 |op[1];
            state->PC+=2;
            break;
        }
        case 0x0b:
            {
                state->PC = ((uint16_t)state->V[0])+(((op[0]&0xf)<<8)|op[1]);
                break;
            }
        case 0x0c: 
            {
                uint8_t reg = op[0]&0xf;
                state->V[reg] = rand() & op[1];
                state->PC+=2; 
                break;
            }
        case 0x0d:
            {
            int lines = op[1]&0xf;
            int x = state->V[op[0] & 0xf];
            int y = state->V[(op[1] & 0xf0) >> 4];	
            int i,j;
            
            state->V[0xf] = 0;
            for (i=0; i<lines; i++)
            {
                // get the current byte on sprite
                uint8_t *sprite = &state->memory[state->I+i];
                // iterate over each bit in a sprite byte 
                int spritebit=7;
                // iterating over width of sprite and also checking for screen bound
                for (j=x; j<(x+8) && j<64; j++)
                {
                    int jover8 = j / 8;     //picks the byte in the row
                    int jmod8 = j % 8;      //picks the bit in the byte
                    uint8_t srcbit = (*sprite >> spritebit) & 0x1;
                    
                    if (srcbit)
                    {
                        // y cord * 8(a byte){gives us correct row} + current x cordinate{correct column or byte}

                        uint8_t *destbyte_p = &state->screen[ (i+y) * (64/8) + jover8];
                        uint8_t destbyte = *destbyte_p;
                        // msb of destbyte (meaning the exact bit of destbyte corresponding to spritebyte)
                        uint8_t destmask = (0x80 >> jmod8);
                        uint8_t destbit = destbyte & destmask;

                        srcbit = srcbit << (7-jmod8);
                        
                        if (srcbit & destbit)
                            state->V[0xf] = 1;
                        
                        destbit ^= srcbit;
                        
                        // setting the only bit we want to modify
                        destbyte = (destbyte & ~destmask) | destbit;

                        *destbyte_p = destbyte;
                    }
                    spritebit--;
                }
            }
                state->PC+=2;
                break;
        }
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
            state->PC+=2;
            break;
            }
        case 0x0f:
            {
            int reg = op[0]&0xf;
            switch(op[1]){
                case 0x0A:

                    {
                        printf("Waiting for a key press\n");
                        if(state->waiting_for_key == 0){
                            memcpy(&state->save_key_state,&state->key_state,16);
                            state->waiting_for_key = 1;
                            return;
                        }else{
                            int i;
                            for(i =0;i<16;i++){
                                // comparing if the key was pressed and which one.
                                if(state->save_key_state[i]==0 && (state->key_state[i]==1)){
                                    state->waiting_for_key = 0;
                                    state->V[reg] = i;
                                    state->PC+=2;
                                    return ;
                                }
                                
                                state->save_key_state[i] = state->key_state[i];
                            }
                            return;
                        }
                    }
                case 0x33:{
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
                }
                case 0x07:
                {
                    state->V[reg] = state->delay;
                    break;

                }
                case 0x15:
                {
                    state->delay = state->V[reg];
                    break;
                }
                case 0x18:
                {
                    state->sound = state->V[reg];
                    break;
                }
                case 0x29:
                    {
                        int reg = op[0]&0xf;
                        state->I = FONT_BASE+(state->V[reg]*5);
                        break;
                    }
                case 0x55:
                    {
                        int i;
                        uint8_t reg = op[0]&0xf;
                        for(i=0;i<=reg;i++){
                            state->memory[state->I+i]= state->V[i];

                        }
                        state->I+=(reg+1);

                    break;
                    }
                case 0x65:
                    {
                        int i;
                        uint8_t reg = op[0]&0xf;
                        for(i =0;i<=reg;i++){
                            state->V[i] = state->memory[state->I+i];
                        }
                        state->I+=(reg+1);

                    break;
                    }
            }
            state->PC+=2;
            break;
        }   
        default:
            break;

    }

    

}