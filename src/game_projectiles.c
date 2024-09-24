#include "game_projectiles.h"
#include "asteroid.h"
#include "constants.h"
#include "game_asteroids.h"


static int projectile_max = 5;
static Projectile *projectiles = NULL;

void set_projectile_max(int max){
    projectile_max = max;

    if (projectiles != NULL){
        free(projectiles);
    }

    projectiles = calloc(projectile_max, sizeof(Projectile));

}
void add_projectile(Vector2 position, float rotation){

    for (int i = 0; i < projectile_max; i++){
        if (projectiles[i].active){
            continue;
        }

        projectiles[i] = create_projectile(position, rotation);
        return;
    }

    TraceLog(LOG_ERROR, "projectile could not be created array is full");

}

static bool check_projectile_collision(Projectile *projectile, Asteroid *asteroid){
    return asteroid->active && 
    CheckCollisionPointCircle(projectile->position, asteroid->position, asteroid_radius(*asteroid));
}

void update_projectiles(){
    float frametime = GetFrameTime();
    float time = GetTime();

    Asteroid *asteroids = get_asteroids();
    for (int i = 0; i < projectile_max; i++){
        if (update_projectile(&projectiles[i], frametime, time)){             
            for (int j = 0; j < MAX_ASTEROIDS; j++){
                if (check_projectile_collision(&projectiles[i], &asteroids[j])){
                    
                    destroy_asteroid_at(j, projectiles[i].rotation);
                    projectiles[i].active = false;

                }
            }
        }
    }

}

void reset_projectiles(){
    for (int i = 0; i < projectile_max; i++){
        projectiles[i] = (Projectile){0};
    }
}

void draw_projectiles(){
    for (int i = 0; i < projectile_max; i++){
        if (!projectiles[i].active) continue;
        draw_projectile(projectiles[i]);
    }
}
