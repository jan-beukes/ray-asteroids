#ifndef GAME_ASTEROIDS_H
#define GAME_ASTEROIDS_H

#include "asteroid.h"

#define SPLIT_SPEED_MOD 0.5

void set_asteroid_delay(float delay);
void add_asteroid(Vector2 position, AsteroidSize size, bool spawn, float proj_angle);
Vector2 get_next_asteroid_position();
int update_asteroids();
void draw_asteroids(); 
void destroy_asteroid_at(int index, float proj_angle);
void reset_asteroids();
Asteroid *get_asteroids();

#endif