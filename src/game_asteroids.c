#include "game_asteroids.h"
#include "constants.h"
#include "score.h"
#include "game.h"

#include <raylib.h>
#include <raymath.h>
#include "debug.h"

static AsteroidSize sizes[] = { ASTEROID_SMALL, ASTEROID_MEDIUM, ASTEROID_LARGE };
static Asteroid asteroids[MAX_ASTEROIDS] = {0};
static float _last_asteroid_creation_time = -1.0;
static float asteroid_delay = 1;

void set_asteroid_delay(float delay){
    asteroid_delay = delay;
}

void add_asteroid(Vector2 position, AsteroidSize size, bool spawn, float proj_angle) // Proj angle for split asteroids
{
    bool created = false;
    Vector2 velocity;
    float speed_mod = 1;
    if (spawn){
        velocity = Vector2Subtract(SCREEN_CENTER, position);
        velocity = Vector2Normalize(velocity);
    }else{
        float angle = GetRandomValue(proj_angle-90, proj_angle+90);
        velocity = (Vector2){cos(angle * DEG2RAD), sin(angle * DEG2RAD)};
        speed_mod = SPLIT_SPEED_MOD;
    }
    velocity = Vector2Scale(velocity, GetRandomValue(ASTEROID_MIN_SPEED, ASTEROID_MAX_SPEED) * speed_mod);
    
    //Debug
    set_last_asteroid_direction(position, atan2(velocity.y, velocity.x) * RAD2DEG, spawn);
    
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

    if (time > _last_asteroid_creation_time + asteroid_delay){
        AsteroidSize next_size = sizes[GetRandomValue(0, 2)];
        add_asteroid(get_next_asteroid_position(), next_size, true, 0);
        _last_asteroid_creation_time = time;
    }

    return active_asteroids;
}

void draw_asteroids(){
    for (int i = 0; i < MAX_ASTEROIDS; i++){
        asteroid_draw(asteroids[i]);
    }
}

void destroy_asteroid_at(int index, float proj_angle){
    asteroids[index].active = false;
    Asteroid asteroid = asteroids[index];

    int points = asteroid.size * 10;
    switch (asteroid.size){
        case ASTEROID_LARGE: 
            add_asteroid(asteroid.position, ASTEROID_MEDIUM, false, proj_angle);
            add_asteroid(asteroid.position, ASTEROID_MEDIUM, false, proj_angle);
            break;

        case ASTEROID_MEDIUM:
            add_asteroid(asteroid.position, ASTEROID_SMALL, false, proj_angle);
            add_asteroid(asteroid.position, ASTEROID_SMALL, false, proj_angle);
            break;

        default:
            break;
    }
    play_sound("asteroid_sound");
    add_points(points);
}

void reset_asteroids(){
    for (int i = 0; i < MAX_ASTEROIDS; i++){
        asteroids[i] = (Asteroid){0};
    }

    _last_asteroid_creation_time = -1;

}

Asteroid *get_asteroids(){
    return asteroids;
}
