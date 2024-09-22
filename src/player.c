#include "player.h"
#include "debug.h"
#include "constants.h"
#include "game_projectiles.h"
#include <raymath.h>

#define PLAYER_ROT_SPEED 250
#define PLAYER_ACCEL 700
#define PLAYER_MAX_SPEED 300
#define PLAYER_FRICTION 100
#define PLAYER_SIZE 32
#define PLAYER_FIRE_DELAY 0.4

// Return direction
static void update_movement(Player *player){
    float frametime = GetFrameTime();

    int x_input = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
    player->rotation += (x_input * PLAYER_ROT_SPEED * frametime);
    player->rotation = Wrap(player->rotation,0, 360);
    
     Vector2 dir = Vector2Rotate((Vector2){1,0}, player->rotation * DEG2RAD);
    if(IsKeyDown(KEY_W)){

        player->velocity = Vector2Add(player->velocity, Vector2Scale(dir, PLAYER_ACCEL * frametime));
        float magnitude = Vector2Length(player->velocity);

        if (magnitude >= PLAYER_MAX_SPEED){
            player->velocity = Vector2Scale(player->velocity, PLAYER_MAX_SPEED/magnitude);
        }

        Vector2 rocker_pos = Vector2Subtract(player->position, Vector2Scale(dir, 15));

        // Boost Drawing
        DrawPoly(rocker_pos, 3, 8, player->rotation + 180, SKYBLUE);   
    }
    player->position = Vector2Add(player->position, Vector2Scale(player->velocity, frametime));
    
    // Resistance
    Vector2 friction_dir = Vector2Negate(Vector2Normalize(player->velocity));
    player->velocity = Vector2Add(player->velocity, Vector2Scale(friction_dir, PLAYER_FRICTION * frametime));

}

void player_update(Player *player){
    float time = GetTime();

    update_movement(player);

    // Wrap
    if (player->position.x > SCREEN_WIDTH + PLAYER_SIZE/2) {player->position.x = -PLAYER_SIZE/2;}
    else if (player->position.x < -PLAYER_SIZE/2) {player->position.x = SCREEN_WIDTH + PLAYER_SIZE/2;}
    
    if (player->position.y > SCREEN_HEIGHT + PLAYER_SIZE/2) {player->position.y = -PLAYER_SIZE/2;}
    else if (player->position.y < -PLAYER_SIZE/2) {player->position.y = SCREEN_HEIGHT+ PLAYER_SIZE/2;}

    if (IsKeyDown(KEY_SPACE) && time > player->last_fire_time + PLAYER_FIRE_DELAY){
        Vector2 dir = Vector2Scale(
            (Vector2){cos(player->rotation * DEG2RAD), sin(player->rotation * DEG2RAD)}, PLAYER_SIZE/2);
        add_projectile(Vector2Add(player->position, dir), player->rotation);
        player->last_fire_time = time;
    }

    set_player_info(player->position, player->velocity, player->rotation);
}

void player_draw(Player player, Texture2D player_texture){
    //TDrawPolyLines(player.position, 3, 28, player.rotation, RAYWHITE);

    const Rectangle source = {0, 0, 32, 32};
    Rectangle dest = {player.position.x, player.position.y, 48, 48}; //The rectangle space to draw into 
    Vector2 origin = {dest.width/2, dest.height/2};
    float rotation = player.rotation + 90;
    DrawTexturePro(player_texture, source, dest, origin, rotation, WHITE);
}