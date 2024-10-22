#ifndef GAME_H
#define GAME_H

#include <raylib.h>

void init_game(Music *music);
void update_game();
void draw_game();
void play_sound(char *sound_name); //sussy
void game_over();

#endif
