#include "projectile.h"
#include "constants.h"
#include <math.h>

#define PROJECTILE_SPEED 600
#define PROJECTILE_THICK 4.5
//#define PROJECTILE_LEN 20
#define PROJECTILE_COLOUR PURPLE

Projectile create_projectile(Vector2 position, float rotation){
    return (Projectile){
        .active = true,
        .creation_time = GetTime(),
        .position = position,
        .rotation = rotation
    };
}

bool update_projectile(Projectile *projectile, float frametime, float time){
    if (!projectile->active){
        return false; 
    }
    if (time > projectile->creation_time + PROJECTILE_LIFETIME 
        || !CheckCollisionPointRec(projectile->position, SCREEN_RECT)){
        projectile->active = false;
        return false;
    }
    float radians = DEG2RAD * projectile->rotation;
    projectile->position.x += PROJECTILE_SPEED * cos(radians) * frametime;
    projectile->position.y += PROJECTILE_SPEED * sin(radians) * frametime;

    return true;
}

void draw_projectile(Projectile projectile){

    DrawCircleV(projectile.position, PROJECTILE_THICK, PROJECTILE_COLOUR);

    // DrawCircleSector(projectile.position, PROJECTILE_LEN, 180 + projectile.rotation + PROJECTILE_THICK,
    //                  180 + projectile.rotation - PROJECTILE_THICK, 10, PROJECTILE_COLOUR);

}
