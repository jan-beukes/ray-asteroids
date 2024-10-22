#include <raylib.h>

#include "game.h"
#include "constants.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

bool _quit_game = false;

void update_draw_frame(){
    update_game();
    BeginDrawing();
    {
        ClearBackground(NEARBLACK);
        draw_game();
    }
    EndDrawing();
}

int main(){
    srand(time(NULL));

    InitWindow(SCREEN_WIDTH, SCREEN_WIDTH, "Asteroids");
    
    InitAudioDevice();
    Music music = LoadMusicStream("resources/tetris.mp3");
    init_game(&music);
    
    while (!WindowShouldClose() && !_quit_game) {
        
        UpdateMusicStream(music);
        update_draw_frame();
    }
    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
