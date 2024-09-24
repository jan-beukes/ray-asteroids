#ifndef SCORE_H
#define SCORE_H

#include <raylib.h>

int get_score();
void add_points(int points);
void draw_score(bool playing);
void reset_score();

#endif