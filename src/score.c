#include "score.h"
#include <raylib.h>
#include <stdlib.h>
#include "constants.h"

static int score = 0;

int get_score(){
    return score;
}

void reset_score(){
    score = 0;
}

void add_points(int points){
    score += points;
}

void draw_score(bool playing){
    const int font_size = 32;
    const Color white = Fade(WHITE, 0.8);
    const Color black = Fade(BLACK, 0.8);

    char *text = TextFormat("SCORE\n%d", score);

    float length = MeasureText(text, font_size);

    if(playing){
        DrawText(text, SCREEN_WIDTH-length-12, 12, font_size, black);
        DrawText(text, SCREEN_WIDTH-length-16, 8, font_size, white);
    }else{
        DrawText(text, SCREEN_CENTER.x-160, 150, font_size, black);
        DrawText(text, SCREEN_CENTER.x-156, 154, font_size, white);
    }
}