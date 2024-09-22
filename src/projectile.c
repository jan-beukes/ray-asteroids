#include "projectile.h"
#include <math.h>

#define PROJECTILE_LIFETIME 2.0
#define PROJECTILE_SPEED 600
#define PROJECTILE_THICK 8
#define PROJECTILE_LEN 25
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
    
    if (time > projectile->creation_time + PROJECTILE_LIFETIME){
        projectile->active = false;
        return false;
    }
    float radians = DEG2RAD * projectile->rotation;
    projectile->position.x += PROJECTILE_SPEED * cos(radians) * frametime;
    projectile->position.y += PROJECTILE_SPEED * sin(radians) * frametime;

    return true;
}

void draw_projectile(Projectile projectile){
    Rectangle rect = {projectile.position.x, projectile.position.y, PROJECTILE_LEN, PROJECTILE_THICK};
    Vector2 origin = {rect.width/2, rect.height/2};
    DrawRectanglePro(rect, origin, projectile.rotation, PROJECTILE_COLOUR);
}