#include "player.h"
#include "debug.h"
#include "constants.h"
#include "game_projectiles.h"
#include "projectile.h"
#include "game_asteroids.h"
#include "asteroid.h"
#include "game.h"
#include <raymath.h>
#include <stdio.h>

#define PLAYER_ROT_SPEED 250
#define PLAYER_ACCEL 700
#define PLAYER_MAX_SPEED 300
#define PLAYER_FRICTION 100

#define PLAYER_SIZE 32

#define PLAYER_STUN_DURATION 0.3
#define IFRAME_DURATION 1

static float fire_delay = 0.4;
static int max_health = 5;
int _health = 5;


void set_player_max_health(int max){
    max_health = max;
    _health = max_health;
}

void set_player_fire_delay(float delay){
    fire_delay = delay;
    set_projectile_max((int)PROJECTILE_LIFETIME/fire_delay + 1);
}

void init_player(Player *player){
    _health = max_health;
      
    *player = (Player){
        .position = SCREEN_CENTER,
        .velocity = (Vector2){0},
        .rotation = 270,
        .last_fire_time = -1.0,
        .state = PLAYER_DEFAULT
    };
}


static void rocket_draw(Player player){
    Vector2 rocker_pos = Vector2Subtract(player.position, 
    Vector2Scale((Vector2){cos(player.rotation*DEG2RAD), sin(player.rotation*DEG2RAD)}, 15));

    Color boost_color = SKYBLUE;
    if (player.state == PLAYER_IFRAME || player.state == PLAYER_STUNNED){
        float seconds = GetTime() - player.state_entered_time;
        int value = (int)(seconds * 6);

        if (value % 2){
            boost_color = Fade(boost_color, 0.2);
        }
    }
    
    DrawPoly(rocker_pos, 3, 8, player.rotation + 180, boost_color); // Boost Drawing    
}

static void update_movement(Player *player){
    float frametime = GetFrameTime();
    
    if(player->state != PLAYER_STUNNED && player->state != PLAYER_DEAD){
        
        int x_input = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
        player->rotation += (x_input * PLAYER_ROT_SPEED * frametime);
        player->rotation = Wrap(player->rotation,0, 360);
        
        Vector2 dir = Vector2Rotate((Vector2){1,0}, player->rotation * DEG2RAD);
        if(IsKeyDown(KEY_W)){

            player->velocity = Vector2Add(player->velocity, Vector2Scale(dir, PLAYER_ACCEL * frametime));
            float magnitude = Vector2Length(player->velocity);

            if (magnitude >= PLAYER_MAX_SPEED){
                player->velocity = Vector2Scale(player->velocity, PLAYER_MAX_SPEED/magnitude);
            }

            rocket_draw(*player);

        }
    }

    player->position = Vector2Add(player->position, Vector2Scale(player->velocity, frametime));
    
    // Resistance
    Vector2 friction_dir = Vector2Negate(Vector2Normalize(player->velocity));
    player->velocity = Vector2Add(player->velocity, Vector2Scale(friction_dir, PLAYER_FRICTION * frametime));

}

void update_state(Player *player, PlayerState state){
    player->state = state;
    player->state_entered_time = GetTime();
}

static void tick_state(Player *player){
    const float player_death_delay = 0.8;

    switch (player->state){
        case PLAYER_DEFAULT:
            break;
        
        case PLAYER_STUNNED:
            if (GetTime() - player->state_entered_time > PLAYER_STUN_DURATION){
                update_state(player, PLAYER_IFRAME);
            }   
            break;
        case PLAYER_IFRAME:
            if (GetTime() - player->state_entered_time > IFRAME_DURATION){
                update_state(player, PLAYER_DEFAULT);
            }
            break;
            
        case PLAYER_DEAD:
            if (GetTime() - player->state_entered_time > player_death_delay){
                game_over();
            }
            break;
    }
}

static void on_death(Player *player){
    update_state(player, PLAYER_DEAD);
}

static void player_colision(Player *player){
    Asteroid *asteroids = get_asteroids();
    for (int i = 0; i < MAX_ASTEROIDS; i++){
        Asteroid *asteroid = asteroids + i;

        if (!asteroid->active) continue;
        if (!CheckCollisionCircles(player->position, PLAYER_SIZE - PLAYER_SIZE/3.5, asteroid->position, asteroid_radius(*asteroid))){
            continue;
        }
        // Collision
        const float bounce_magnitude = 150;
        const float asteroid_slow = 0.6;

        Vector2 bounce_dir = Vector2Normalize(Vector2Subtract(player->position, asteroid->position));
        player->velocity = Vector2Scale(bounce_dir, bounce_magnitude);
        asteroid->velocity = Vector2Scale(asteroid->velocity, asteroid_slow);

        update_state(player, PLAYER_STUNNED);

        _health--;
        if (_health <= 0){
            on_death(player);
        }

 
        break;
    }
}

void player_update(Player *player){
    float time = GetTime();
    update_movement(player);
    tick_state(player);

    // Wrap
    if (player->position.x > SCREEN_WIDTH + PLAYER_SIZE/2) {player->position.x = -PLAYER_SIZE/2;}
    else if (player->position.x < -PLAYER_SIZE/2) {player->position.x = SCREEN_WIDTH + PLAYER_SIZE/2;}
    
    if (player->position.y > SCREEN_HEIGHT + PLAYER_SIZE/2) {player->position.y = -PLAYER_SIZE/2;}
    else if (player->position.y < -PLAYER_SIZE/2) {player->position.y = SCREEN_HEIGHT+ PLAYER_SIZE/2;}

    if (player->state == PLAYER_STUNNED || player->state == PLAYER_DEAD) {
        return;
    }

    if (IsKeyDown(KEY_SPACE) && time > player->last_fire_time + fire_delay){
        Vector2 dir = Vector2Scale(
            (Vector2){cos(player->rotation * DEG2RAD), sin(player->rotation * DEG2RAD)}, PLAYER_SIZE/2);
        add_projectile(Vector2Add(player->position, dir), player->rotation);
        player->last_fire_time = time;
    }

    if (player->state == PLAYER_IFRAME) {
        return;
    }

    player_colision(player);

}

void player_draw(Player player, Texture2D player_texture){

    const Rectangle source = {0, 0, 32, 32};
    Rectangle dest = {player.position.x, player.position.y, 48, 48}; //The rectangle space to draw into 
    Vector2 origin = {dest.width/2, dest.height/2};
    float rotation = player.rotation + 90;

    Color player_color = WHITE;
    if (player.state == PLAYER_IFRAME || player.state == PLAYER_STUNNED){
        float seconds = GetTime() - player.state_entered_time;
        int value = (int)(seconds * 6);

        if (value % 2){
            player_color = Fade(player_color, 0.2);
        }
    }
    DrawTexturePro(player_texture, source, dest, origin, rotation, player_color);
}

void player_draw_health(){
    const int gap = 5;
    const int radius = 10;
    const int starting_gap_sum = gap * (int)(max_health/2.0 - 0.5);
    const int first_circle_x = SCREEN_CENTER.x - radius * (max_health-1) - starting_gap_sum;
    const int circle_y = 25;

    for (int i = 0; i < max_health; i++){
        int x = first_circle_x + i * (2*radius + gap);
        if (i + 1 > _health){  
            DrawCircleLines(x, circle_y, radius, WHITE);      
        }else{
            DrawCircle(x, circle_y, radius, PINK);
        }
    }

}
