#ifndef DEBUG_H
#define DEBUG_H

#include <raylib.h>

void show_debug_menu();
void show_debug_visualizations();
void set_last_cone(Vector2 position, Vector2 velocity);
void set_player_info(Vector2 position, Vector2 velocity, float rotation);
#endif