#include "debug.h"
#include "constants.h"
#include "raygui.h"
#include <raymath.h>
#include <stdio.h>

static bool _show_debug_menu = false;
static bool _debug_use_checkbox = true;
static bool _show_asteroid_count = false;
static bool _show_angle_cone = false;
static Vector2 line0[2] = {0};
static Vector2 line1[2] = {0};

static Vector2 _player_position = {0};
static Vector2 _player_velocity = {0};
static float _player_rotation = 0;

void show_debug_menu(){
    if (IsKeyPressed(KEY_M)){
        _show_debug_menu = !_show_debug_menu;
    }

    if (_show_debug_menu){
        if (!_debug_use_checkbox){
            Rectangle r = {10, SCREEN_HEIGHT - 80, 180, 60};
            GuiToggle(r, "Show Asteroid Count", &_show_asteroid_count);

            r.x += 180 + 10;
            GuiToggle(r, "Show Angle Cone", &_show_angle_cone);

            r.x += 180 + 10;
            bool toggle = false;
            GuiToggle(r, "Switch Menu Style", &toggle);
            if (toggle){
                _debug_use_checkbox = !_debug_use_checkbox;
            }
        }else{
            Rectangle r = {10, SCREEN_HEIGHT - 40, 20, 20};
            GuiCheckBox(r, "Show Asteroid Count", &_show_asteroid_count);
            r.y -= 30;
            GuiCheckBox(r, "Show Angle Cone", &_show_angle_cone);

            r.y -= 30;
            if (GuiCheckBox(r, "Switch Menu Style", false)){
                _debug_use_checkbox = !_debug_use_checkbox;
            }
        }
       
    }
}

// Drawing
void show_debug_visualizations(int asteroid_count){
    if (_show_angle_cone){
        DrawLineV(line0[0], line0[1], RED);
        DrawLineV(line1[0], line1[1], BLUE);
    }

    if (_show_asteroid_count){
        DrawRectangle(10, 10, 275, 52, Fade(BLACK, 0.6));
        DrawText(TextFormat("ASTEROIDS: %d", asteroid_count), 20, 20, 32, WHITE);
    }
}

void set_last_cone(Vector2 position, Vector2 velocity){
    line0[0] = position;
    line1[0] = position;
    line0[1] = Vector2Add(position, Vector2Rotate(Vector2Scale(velocity, 10), ASTEROID_RANDOM_ANGLE));
    line1[1] = Vector2Add(position, Vector2Rotate(Vector2Scale(velocity, 10), -ASTEROID_RANDOM_ANGLE));
}

void set_player_info(Vector2 position, Vector2 velocity, float rotation){
    _player_position = position;
    _player_velocity = velocity;
    _player_rotation = rotation;

}
