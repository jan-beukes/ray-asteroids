#ifndef DEBUG_H
#define DEBUG_H

#include <raylib.h>

void show_debug_menu();
void show_debug_visualizations(int asteroid_count);
void set_last_asteroid_direction(Vector2 position, float angle, bool spawn);
#endif