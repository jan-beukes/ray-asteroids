#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

typedef struct{
    Vector2 position;
    Vector2 velocity;
    float rotation;
    float last_fire_time;

} Player;

void player_update(Player *player);
void player_draw(Player player, Texture2D player_texture);

#endif