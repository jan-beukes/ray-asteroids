#ifndef ASTEROID_H
#define ASTEROID_H

#include <raylib.h>
#include <stdlib.h>

#define ASTEROID_MIN_ROT_SPEED 70
#define ASTEROID_MAX_ROT_SPEED 240
#define ASTEROID_MIN_SPEED 100
#define ASTEROID_MAX_SPEED 300

typedef enum {
    ASTEROID_SMALL = 1,
    ASTEROID_MEDIUM = 2,
    ASTEROID_LARGE = 4
} AsteroidSize;

typedef struct{
    bool active;
    bool has_entered_screen;
    Vector2 position;
    Vector2 velocity;
    AsteroidSize size;

    float creation_time;
    float rotation;
    float rotation_speed;

}Asteroid;

Asteroid create_asteroid(Vector2 position, Vector2 velocity, AsteroidSize size);
bool asteroid_update(Asteroid *asteroid, float frametime, float time);
void asteroid_draw(Asteroid asteroid);
float asteroid_radius(Asteroid asteroid);

#endif