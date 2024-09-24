#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

typedef enum {
    PLAYER_DEFAULT,
    PLAYER_STUNNED,
    PLAYER_IFRAME,
    PLAYER_DEAD
} PlayerState;

typedef struct{
    Vector2 position;
    Vector2 velocity;
    float rotation;
    float last_fire_time;
    PlayerState state;
    float state_entered_time;
} Player;

void set_player_fire_delay(float delay);
void set_player_max_health(int max);
void init_player(Player *player);
void player_update(Player *player);
void player_draw(Player player, Texture2D player_texture);
void player_draw_health();

#endif