#include "disamb.h"
#include "emulator.h"
#include "raylib.h"
#include <stdio.h>
#include "string.h"

#define CHIP8_HEIGHT 32
#define CHIP8_WIDTH 64

#define SCALE_FACTOR 10

static inline void clear_key_state(Chip8State* c){
    memset(c->key_state,0,16);
}

static inline void updateChip8Timers(Chip8State *state, float deltaTime) {
    static float accumulator = 0.0f;
    accumulator += deltaTime;
    
    // CHIP-8 timers decrement at 60Hz
    while (accumulator >= 1.0f / 60.0f) {
        if (state->delay > 0) {
            state->delay--;
        }
        // if (state->sound_timer > 0) {
        //     state->sound_timer--;
        //     // Implement sound playing here if sound_timer > 0
        // }
        accumulator -= 1.0f / 60.0f;
    }
}

int main(int argc,const char* argv[]){
    Chip8State* c =  read_rom(argc,argv);
    if(argc>2 &&  strcmp(argv[2],"disamb")==0){
        return 1;
    }
    InitWindow(640,320,"Chip8 Emulator");
    SetTargetFPS(120);
    // double lastitme = GetTime()
    Texture2D displayTexture = LoadTexture("");
    while(!WindowShouldClose()){

        float deltatime = GetFrameTime();        

        updateChip8Timers(c,deltatime);
        // two cycles per frame 
        for(int i=0;i<2;i++){
            if (c->halt!=1) EmulateChip8(c);
        }

        
        if(IsKeyPressed(KEY_ONE)){
            PollInputEvents();
            printf("1 pressed\n");
            clear_key_state(c);
            c->key_state[0] = 1;
        }
        if(IsKeyPressed(KEY_TWO)){
            PollInputEvents();
            clear_key_state(c);
            c->key_state[1] = 1;
        }
        if(IsKeyPressed(KEY_THREE)){
            PollInputEvents();
            clear_key_state(c);
            c->key_state[2] = 1;
        }
        if(IsKeyPressed(KEY_FOUR)){
            PollInputEvents();
            clear_key_state(c);
            c->key_state[3] = 1;
        }
        if(IsKeyPressed(KEY_Q)){
            PollInputEvents();
            clear_key_state(c);
            c->key_state[4] = 1; 
        }
        if(IsKeyPressed(KEY_W)){
            PollInputEvents();
            clear_key_state(c);
            c->key_state[5] = 1;
        }
        if(IsKeyPressed(KEY_E)){
            PollInputEvents();
            clear_key_state(c);
            c->key_state[6] = 1;
        }
        if(IsKeyPressed(KEY_R)){
            PollInputEvents();
            clear_key_state(c);
            c->key_state[7] = 1;
        }
        if(IsKeyPressed(KEY_A)){
            PollInputEvents();
            clear_key_state(c);
            c->key_state[8] = 1;
        }
        if(IsKeyPressed(KEY_S)){
            PollInputEvents();
            clear_key_state(c);
            c->key_state[9] = 1;
        }
        if(IsKeyPressed(KEY_D)){
            PollInputEvents();
            clear_key_state(c);
            c->key_state[10] = 1;
        }
        if(IsKeyPressed(KEY_F)){
            PollInputEvents();
            clear_key_state(c);
            c->key_state[11] = 1;
        }
        if(IsKeyPressed(KEY_Z)){
            PollInputEvents();
            clear_key_state(c);
            c->key_state[12] = 1;
        }
        if(IsKeyPressed(KEY_X)){
            PollInputEvents();
            clear_key_state(c);
            c->key_state[13] = 1;
        }
        if(IsKeyPressed(KEY_C)){
            PollInputEvents();
            clear_key_state(c);
            c->key_state[14] = 1;
        }
        if(IsKeyPressed(KEY_V)){
            PollInputEvents();
            clear_key_state(c);
            c->key_state[15] = 1;
        }
        
        // printf("Key pressed %d",GetKeyPressed());

        BeginDrawing();
        ClearBackground(BLACK);

        for (int y = 0; y < CHIP8_HEIGHT; y++) {
        for (int x = 0; x < CHIP8_WIDTH; x++) {
            int byte_index = y * (CHIP8_WIDTH / 8) + (x / 8);
            int bit_index = 7 - (x % 8);  // Reverse bit order within byte
            
            if (c->screen[byte_index] & (1 << bit_index)) {
                DrawRectangle(x * SCALE_FACTOR, y * SCALE_FACTOR, 
                              SCALE_FACTOR, SCALE_FACTOR, WHITE);
            }
        }
    }
        EndDrawing();

    }
    CloseWindow(); //


    // EmulateChip8();
    return 0;
}