#ifndef PROJECTILE_H
#define PROJECTILE_H

#define PROJECTILE_LIFETIME 2.0

#include <raylib.h>

typedef struct {
    bool active;
    float creation_time;
    Vector2 position;  
    float rotation;
} Projectile;

Projectile create_projectile(Vector2 position, float rotation);
bool update_projectile(Projectile *projectile, float frametime, float time); 
void draw_projectile(Projectile projectile);

#endif