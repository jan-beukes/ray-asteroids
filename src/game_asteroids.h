#ifndef GAME_ASTEROIDS_H
#define GAME_ASTEROIDS_H

#include "asteroid.h"

void add_asteroid(Vector2 position, AsteroidSize size);
Vector2 get_next_asteroid_position();
int update_asteroids();
void draw_asteroids(); 

#endif