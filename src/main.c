#include <raylib.h>
#include <raymath.h>
#include "raygui.h"

#include "asteroid.h"
#include "game_asteroids.h"
#include "game_projectiles.h"
#include "constants.h"
#include "debug.h"
#include "player.h"

#include <stdlib.h>
#include <time.h>
#include <stdio.h>

static Player player;
static Texture2D player_texture;

void update_draw_frame(){
    int active_asteroids = update_asteroids();
    int active_projectiles = update_projectiles();
    player_update(&player);

    BeginDrawing();
    {
        ClearBackground(NEARBLACK);
    
        draw_projectiles();
        draw_asteroids();
        player_draw(player, player_texture);    

        show_debug_visualizations(active_asteroids);
        show_debug_menu();
    }
    EndDrawing();
}

int main(){
    srand(time(NULL));

    InitWindow(SCREEN_WIDTH, SCREEN_WIDTH, "Asteroids");

    player = (Player){
        .position = SCREEN_CENTER,
        .velocity = (Vector2){0},
        .rotation = 270,
        .last_fire_time = -1.0
    };
    player_texture = LoadTexture("resources/ship.png");

    while (!WindowShouldClose()) {
        update_draw_frame();
    }

    CloseWindow();
    return 0;
}
