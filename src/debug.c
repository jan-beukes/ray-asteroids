#include "debug.h"
#include "constants.h"
#include "raygui.h"
#include <raymath.h>
#include <stdio.h>
#include <raylib.h>

static bool _show_debug_menu = false;
static bool _show_asteroid_count = false;
static int _show_direction_state = 0;
static Vector2 line1[2] = {0};
static Vector2 line2[2] = {0};
static bool line_switch;

void show_debug_menu(){
    if (IsKeyPressed(KEY_LEFT_ALT)){
        _show_debug_menu = !_show_debug_menu;
    }

    if (_show_debug_menu){
        SetWindowTitle(TextFormat("%d", GetFPS()));
            
        Rectangle r = {10, SCREEN_HEIGHT - 80 , 30, 30};

        GuiCheckBox(r, "Show Asteroid Count", &_show_asteroid_count);
        
        r.y += 40;
        r.width = 100;

        GuiComboBox(r, "off;split;spawn", &_show_direction_state);
    }
}

// Drawing
void show_debug_visualizations(int asteroid_count){

    if (_show_direction_state > 0){
        DrawLineV(line1[0], line1[1], RED);
        DrawLineV(line2[0], line2[1], BLUE);

    }
    if (_show_asteroid_count){
        DrawRectangle(10, 10, 275, 52, Fade(BLACK, 0.6));
        DrawText(TextFormat("ASTEROIDS: %d", asteroid_count), 20, 20, 32, WHITE);
    }
}

void set_last_asteroid_direction(Vector2 position, float angle, bool spawn){
    if ((_show_direction_state != 2 && spawn) || (_show_direction_state == 2 && !spawn)) return;

    if (!line_switch){
        line1[0] = position;
        Vector2 dir = {cos(angle * DEG2RAD), sin(angle * DEG2RAD)};
        line1[1] = Vector2Add(position, Vector2Scale(dir, SCREEN_WIDTH*3/2));
    }else{
        line2[0] = position;
        Vector2 dir = {cos(angle * DEG2RAD), sin(angle * DEG2RAD)};
        line2[1] = Vector2Add(position, Vector2Scale(dir, SCREEN_WIDTH*3/2));
    }
    line_switch = !line_switch;
}


