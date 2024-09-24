#include "asteroid.h"
#include <raymath.h>
#include "constants.h"

Asteroid create_asteroid(Vector2 position, Vector2 velocity, AsteroidSize size)
{
    return (Asteroid) {
        .active = true,
        .position = position,
        .velocity = velocity,
        .size = size,
        .creation_time = GetTime(),
        .rotation = rand() % 360,
        .rotation_speed = GetRandomValue(ASTEROID_MIN_ROT_SPEED, ASTEROID_MAX_ROT_SPEED)
    };
}

bool asteroid_update(Asteroid* asteroid, float frametime, float time)
{
    if (!asteroid->active) {
        return false;
    }
    // Check if asteroid should despawn
    if (asteroid->has_entered_screen && 
        !CheckCollisionCircleRec(asteroid->position, asteroid->size * 20, SCREEN_RECT)){
        asteroid->active = false;
        return false;
    }

    asteroid->position = Vector2Add(asteroid->position,
        Vector2Scale(asteroid->velocity, frametime));
    asteroid->rotation += asteroid->rotation_speed * frametime;

    if (CheckCollisionPointRec(asteroid->position, SCREEN_RECT)){
        asteroid->has_entered_screen = true;
    }

    return true;
}

void asteroid_draw(Asteroid asteroid)
{
    if (!asteroid.active) {
        return;
    }
    DrawPolyLines(asteroid.position, 3, 20 * (int)(asteroid.size), asteroid.rotation, WHITE);
}

float asteroid_radius(Asteroid asteroid){
    return 20.0 * asteroid.size - asteroid.size*5;
}