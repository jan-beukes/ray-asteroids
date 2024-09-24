#ifndef GAME_PROJECTILE_H
#define GAME_PROJECTILE_H

#include "projectile.h"

void set_projectile_max(int max);
void add_projectile(Vector2 position, float rotation);
void update_projectiles();
void reset_projectiles();
void draw_projectiles();

#endif