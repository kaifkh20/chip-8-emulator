#include "disamb.h"
#include "emulator.h"
#include "raylib.h"

#define CHIP8_HEIGHT 32
#define CHIP8_WIDTH 64

#define SCALE_FACTOR 10

int main(int argc,const char* argv[]){
    Chip8State* c =  read_rom(argc,argv);
    InitWindow(640,320,"Chip8 Emulator");
    Texture2D displayTexture = LoadTexture("");
    while(!WindowShouldClose()){
        if (c->halt!=1) EmulateChip8(c);

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