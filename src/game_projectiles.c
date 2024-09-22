#include "game_projectiles.h"

#define PROJECTILE_MAX 5
static Projectile projectiles[PROJECTILE_MAX];

void add_projectile(Vector2 position, float rotation){
    for (int i = 0; i < PROJECTILE_MAX; i++){
        if (projectiles[i].active){
            continue;
        }

        projectiles[i] = create_projectile(position, rotation);
        return;
    }

    TraceLog(LOG_ERROR, "projectile could not be created array is full");

}

int update_projectiles(){
    float frametime = GetFrameTime();
    float time = GetTime();

    int projectile_count = 0;
    for (int i = 0; i < PROJECTILE_MAX; i++){
        if (update_projectile(&projectiles[i], frametime, time)){ projectile_count++;}
    }

    return projectile_count;

}

void draw_projectiles(){
    for (int i = 0; i < PROJECTILE_MAX; i++){
        if (!projectiles[i].active) continue;
        draw_projectile(projectiles[i]);
    }
}
