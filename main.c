#include "disamb.h"
#include "emulator.h"
#include "raylib.h"

int main(int argc,const char* argv[]){
    Chip8State* c =  read_rom(argc,argv);
    // InitWindow(640,320,"Chip8 Emulator");
    // Texture2D displayTexture = LoadTexture("");
    // while(!WindowShouldClose()){
    //     // EmulateChip8(c);

    //     UpdateTexture(displayTexture,(void*)c->screen);
    //     BeginDrawing();
    //     ClearBackground(RAYWHITE);
    //     DrawText("Hi",50,50,10,WHITE);
    //     // DrawTexture(displayTexture,0,0,WHITE);
    //     EndDrawing();

    // }
    // CloseWindow();


    // EmulateChip8();
    return 0;
}