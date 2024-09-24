#include "game.h"
#include "debug.h"
#include "asteroid.h"
#include "game_asteroids.h"
#include "game_projectiles.h"
#include "constants.h"
#include "score.h"
#include "player.h"

#include <stdio.h>
#include <string.h>
#include "raygui.h"

int active_asteroids = 0;
static Player player = {0};
static Texture2D player_texture;
Music *_music;
char *high_score = NULL;

extern bool _quit_game;

// Settings
typedef enum {
    EASY,
    REGULAR,
    PRO
}Difficulty;

typedef enum{
    GAME_MENU,
    GAME_PLAYING,
    GAME_OVER
} GameState;

static GameState _state;
static Difficulty _difficulty;

static void set_state(GameState state){
    switch(state){
        case GAME_MENU:
        GuiSetStyle(DEFAULT, TEXT_SIZE, 28);
            break;

        case GAME_PLAYING:
            GuiSetStyle(DEFAULT, TEXT_SIZE, 12);
            GuiSetStyle(BUTTON, BASE_COLOR_NORMAL , 0);
            PlayMusicStream(*_music);

            reset_asteroids();
            init_player(&player); 
            reset_projectiles();
            reset_score();
            break;

        case GAME_OVER:
            GuiSetStyle(DEFAULT, TEXT_SIZE, 28);
            StopMusicStream(*_music);
            break;
    }

    _state = state; 
}

void init_game(Music *music){
    // GUI
    GuiSetStyle(COMBOBOX, BASE_COLOR_NORMAL, 0);
    GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, 0xf5f5f5ff);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xf5f5f5ff);

    _music = music;
    SetMusicVolume(*music, 0.4);
    set_state(GAME_MENU);
    player_texture = LoadTexture("resources/ship.png");
}

static void set_difficulty(Difficulty difficulty){
    _difficulty = difficulty; 
    switch (difficulty)
    {
    case EASY:
        set_player_max_health(8);
        set_player_fire_delay(0.3);
        set_asteroid_delay(2);
        break;
    
    case REGULAR:
        set_player_max_health(5);
        set_player_fire_delay(0.4);
        set_asteroid_delay(1.2);
        break;
    
    case PRO:
        set_player_max_health(3);
        set_player_fire_delay(0.5);
        set_asteroid_delay(0.8);
        break;
    
    }
}

void update_game(){
    if (_state != GAME_MENU){
        active_asteroids = update_asteroids();
    }
    if (_state == GAME_PLAYING){
        update_projectiles();       
        player_update(&player);    
    } 
}

void draw_game(){
    const int button_width = 200;
    const int button_height = 80;
    Rectangle top_button = {SCREEN_CENTER.x - button_width/2, SCREEN_CENTER.y - button_height - 5, button_width, button_height};
    Rectangle quit_button = {SCREEN_CENTER.x - button_width/2, SCREEN_CENTER.y + button_height, button_width, button_height};
    
    switch(_state){
        case GAME_MENU:
            GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0xff6dc2ff);
            if (GuiButton(top_button, "EASY")){
                set_difficulty(EASY);
                set_state(GAME_PLAYING);
                break;
            }
            GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0xc87affff);
            top_button.y = top_button.y + button_height + 5;
            if (GuiButton(top_button, "REGULAR")){
                set_difficulty(REGULAR);
                set_state(GAME_PLAYING);
                break;
            }
            GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0xe62937ff);
            top_button.y = top_button.y + button_height + 5;
            if (GuiButton(top_button, "PRO")){
                set_difficulty(PRO);
                set_state(GAME_PLAYING);
                break;
            }
            
            break;

        case GAME_PLAYING:
            draw_projectiles();
            draw_asteroids();
            player_draw(player, player_texture);    
            draw_score(true);
            player_draw_health();
            show_debug_menu();
            show_debug_visualizations(active_asteroids);
            break;

        case GAME_OVER:
            draw_asteroids();
            DrawRectangleRec(SCREEN_RECT, Fade(BLACK,0.5));

            char *text = "SKILL ISSUE";
            float length = MeasureText(text, 64);
            DrawText(text, SCREEN_CENTER.x - length/2, 32, 64 , WHITE);

            draw_score(false);

            const Color white = Fade(WHITE, 0.8);
            const Color black = Fade(BLACK, 0.8);
            char high_score_text[32];
            sprintf(high_score_text, "HIGHSCORE:\n%s", high_score);
 
            DrawText(high_score_text, SCREEN_CENTER.x+50, 150, 32, black);
            DrawText(high_score_text, SCREEN_CENTER.x+54, 154, 32, white);

            if (GuiButton(top_button, "PLAY AGAIN")){
                set_state(GAME_PLAYING);
            }

            if (GuiButton(quit_button, " QUIT")){
                
                _quit_game = true;
            }

            break;
    }

}

void game_over(){
    if (!FileExists("high.score")){
        SaveFileData("high.score", NULL, 0);
    }

    char *score_data = LoadFileText("high.score");
    if (score_data == NULL) {
        score_data = "";
    }

    if (high_score != NULL ) free(high_score);

    char score_data_cpy[1024];
    strncpy(score_data_cpy, score_data, 1023);
    int score = get_score();
    
    char *easy_score, *reg_score, *pro_score;
    if (strlen(score_data_cpy) != 0){
        easy_score = strtok(score_data_cpy, "\n");
        reg_score = strtok(NULL, "\n");
        pro_score = strtok(NULL, "\n");

    }else{
        easy_score = "0";
        reg_score = "0";
        pro_score = "0";
    }
    switch (_difficulty){
        case EASY:
            easy_score = score > atoi(easy_score) ? TextFormat("%d", score) : easy_score;
            high_score = strdup(easy_score);
            break;
        case REGULAR:
            reg_score = score > atoi(reg_score) ? TextFormat("%d", score) : reg_score; 
            high_score = strdup(reg_score);

            break;
        case PRO:
            pro_score = score > atoi(pro_score) ? TextFormat("%d", score) : pro_score; 
            high_score = strdup(pro_score);
            break;
    }
    char save_buffer[1024];
    sprintf(save_buffer, "%s\n%s\n%s", easy_score, reg_score, pro_score);
    printf("\nSAVED:\n%s", save_buffer);
    SaveFileText("high.score", save_buffer);
    
    set_state(GAME_OVER);
}