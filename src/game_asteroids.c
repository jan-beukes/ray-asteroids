#include "game_asteroids.h"
#include "constants.h"

#include <raymath.h>
#include "debug.h"

static AsteroidSize sizes[] = { ASTEROID_SMALL, ASTEROID_MEDIUM, ASTEROID_LARGE };
static Asteroid asteroids[MAX_ASTEROIDS] = { 0 };
static float _last_asteroid_creation_time = -1.0;

void add_asteroid(Vector2 position, AsteroidSize size)
{
    bool created = false;

    Vector2 velocity = Vector2Subtract(SCREEN_CENTER, position);
    velocity = Vector2Normalize(velocity);
    velocity = Vector2Scale(velocity, GetRandomValue(ASTEROID_MIN_SPEED, ASTEROID_MAX_SPEED));
    
    set_last_cone(position, velocity);

    // Random Rotation
    velocity = Vector2Rotate(velocity, (float) GetRandomValue(-ASTEROID_RANDOM_ANGLE, ASTEROID_RANDOM_ANGLE));

    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (asteroids[i].active) {
            continue;
        }
        asteroids[i] = create_asteroid(position, velocity, size);
        created = true;
        break;
    }

    if (!created) {
        TraceLog(LOG_ERROR, "Failed to create asteroid, no inactive spots in array");
    }
}

Vector2 get_next_asteroid_position(){
    int padding = 128;
    Vector2 ret = {-padding, -padding};
    
    // Random X
    if (GetRandomValue(0,1)){
        // Bottom 
        if (GetRandomValue(0,1)){
            ret.y = SCREEN_HEIGHT + padding;
        }
        ret.x = GetRandomValue(-padding, SCREEN_WIDTH + padding);
    
    // Random Y
    }else{
        //Right
        if (GetRandomValue(0,1)){
            ret.x = SCREEN_WIDTH + padding;
        }
        ret.y = GetRandomValue(-padding, SCREEN_HEIGHT + padding);
    }
    return ret;
}

int update_asteroids(){
    int active_asteroids = 0;

    float frametime = GetFrameTime();
    float time = GetTime();

    for (int i = 0; i < MAX_ASTEROIDS; i++){
        if (asteroid_update(asteroids + i, frametime, time)){
            active_asteroids++;
        }
    }

    if (time > _last_asteroid_creation_time + ASTEROID_DELAY){
        AsteroidSize next_size = sizes[GetRandomValue(0, 2)];
        add_asteroid(get_next_asteroid_position(), next_size);
        _last_asteroid_creation_time = time;
    }

    return active_asteroids;
}

void draw_asteroids(){
    for (int i = 0; i < MAX_ASTEROIDS; i++){
        asteroid_draw(asteroids[i]);
    }
}