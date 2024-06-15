#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "raylib.h"
#include "game.h"
#include "enemies.h"
#include "player.h"

int back_animation = 0;

Texture2D tex_background_rigth;
Texture2D tex_background_updown;
Texture2D tex_background_left;
Texture2D tex_game_field;
Texture2D tex_diff;
Texture2D tex_hi_score;
Texture2D tex_score;
Texture2D tex_hit_points_text;
Texture2D tex_power;
Texture2D tex_graze;
Texture2D tex_num;
Texture2D tex_hit_points;

int score = 0;
// mco_coro* co;

// void coro_entry(mco_coro* co) {
//     printf("coroutine 1\n");
//     for (int i = 0; i < 60; i++) {
//         mco_yield(co);
//     }
//     printf("coroutine 2\n");
// }

int int_len() {
    int b = score;
    int len = 0;
    while (b > 0) {
        b/=10;
        len++;
    }

    return len;
}

void update() {
    player_update();
    enemy_update();
    back_animation--;
    
    score += get_points();
    //printf("%d\n", score);
    // if (mco_status(co) == MCO_SUSPENDED) {
    //     mco_resume(co);
    // }
}

void draw() {
    DrawTexturePro(tex_background_updown,                              //Drawing Background
        (Rectangle){0, 0, 384, 32}, 
        (Rectangle){380, 40, 647, 60},      
        (Vector2) {383, 41.5}, 
        0,
        WHITE
    );

    DrawTexturePro(tex_background_updown,                              //Drawing Background
        (Rectangle){0, 0, 384, 32}, 
        (Rectangle){72, 710, 766.315789, 83.8596},      
        (Vector2) {192, 16}, 
        0,
        WHITE
    );

    DrawTexturePro(tex_background_rigth,                              //Drawing Background
        (Rectangle){0, 0, 228, 480}, 
        (Rectangle){870, 120, 459, 770},                         
        (Vector2) {227.5, 120}, 
        0,
        WHITE
    );

    DrawTexturePro(tex_background_left,                              //Drawing Background
        (Rectangle){0, 0, 36, 480}, 
        (Rectangle){18, 384, 68.8421053, 788},      
        (Vector2) {34, 385}, 
        0,
        WHITE
    );   

    BeginScissorMode(50, 39, 600, 690);

        DrawTexturePro(tex_game_field,                          //Drawing Game field
            (Rectangle){0, back_animation, 512, 512},
            (Rectangle){50, 39, 600, 690},      
            (Vector2) {0, 0},
            0,
            WHITE
        );
        
        enemy_draw();
        player_draw();
    
    EndScissorMode();
    
    DrawTexturePro(tex_diff, (Rectangle) {0, 0, 65, 16}, (Rectangle) {865, 60, 130, 32}, (Vector2) {65, 16}, 0, WHITE);

    DrawTexturePro(tex_hi_score, (Rectangle) {0, 0, 74, 18}, (Rectangle) {735, 110, 130, 32}, (Vector2) {65, 16}, 0, WHITE);
    
    DrawTexturePro(tex_score, (Rectangle) {0, 0, 56, 16}, (Rectangle) {768, 145, 98.378378, 28.444444}, (Vector2) {65, 16}, 0, WHITE);

    DrawTexturePro(tex_hit_points_text, (Rectangle) {0, 0, 63, 20}, (Rectangle) {761, 200, 110.675676, 35.5555}, (Vector2) {65, 16}, 0, WHITE);

    DrawTexturePro(tex_power, (Rectangle) {0, 0, 61, 18}, (Rectangle) {761, 240, 107.162162, 32}, (Vector2) {65, 16}, 0, WHITE);

    DrawTexturePro(tex_graze, (Rectangle) {0, 0, 58, 18}, (Rectangle) {761, 278, 101.891892, 32}, (Vector2) {65, 16}, 0, WHITE);


    char table_score[16];
    snprintf(table_score, sizeof(table_score), "%09d", score);

    int x;
    int d = 0;
    for (int i = 0; i < strlen(table_score); i++) {
        x = (table_score[i] - '0') * 16;
        DrawTexturePro(tex_num, (Rectangle) {x, 0, 16, 16}, (Rectangle) {840 + d, 111, 28, 28}, (Vector2) {14, 14}, 0, WHITE);
        DrawTexturePro(tex_num, (Rectangle) {x, 0, 16, 16}, (Rectangle) {840 + d, 144, 28, 28}, (Vector2) {14, 14}, 0, WHITE);
        d+=24;
    }
    d = 0;
    for (int i = 0; i < get_hit_points(); i++) {
        DrawTexturePro(tex_hit_points, (Rectangle) {0, 0, 16, 15}, (Rectangle) {840 + d, 200, 32, 32}, (Vector2) {16, 16}, 0, WHITE);
        d = 28;
    }
    
    char table_power[16];
    sprintf(table_power, "%0.2lf", get_power());

    d = 0;
    
    bool after_dot = false;
    for (int i = 0; i < 4; i++) {
        if (table_power[i] == '.') {
            x = 11 * 16;
            after_dot = true;
        } else {
            x = (table_power[i] - '0') * 16;
        }
        
        if (after_dot == false) {
            DrawTexturePro(tex_num, (Rectangle) {x, 0, 16, 16}, (Rectangle) {840 + d, 240, 28, 28}, (Vector2) {14, 14}, 0, WHITE);
        } else {
            DrawTexturePro(tex_num, (Rectangle) {x, 0, 16, 16}, (Rectangle) {840 + d / 2,  246, 16, 16}, (Vector2) {8, 8}, 0, WHITE);

        }
        d += 28;
    }
    
    d = 28;
    DrawTexturePro(tex_num, (Rectangle) {160, 0, 16, 16}, (Rectangle) {850 + d * 2, 240, 28, 28}, (Vector2) {14, 14}, 0, WHITE);
    
    DrawTexturePro(tex_num, (Rectangle) {16 * 4, 0, 16, 16}, (Rectangle) {840 + d * 3, 240, 28, 28}, (Vector2) {14, 14}, 0, WHITE);

    DrawTexturePro(tex_num, (Rectangle) {16 * 11, 0, 16, 16}, (Rectangle) {840 + d * 4 - 12, 246, 16, 16}, (Vector2) {8, 8}, 0, WHITE);

    DrawTexturePro(tex_num, (Rectangle) {0, 0, 16, 16}, (Rectangle) {840 + d * 4 + 2, 246, 16, 16}, (Vector2) {8, 8}, 0, WHITE);

    DrawTexturePro(tex_num, (Rectangle) {0, 0, 16, 16}, (Rectangle) {840 + d * 4 + 16, 246, 16, 16}, (Vector2) {8, 8}, 0, WHITE);


  

}

void init() {
    SetMasterVolume(0.1);
    player_init();
    enemy_init();

    // mco_desc desc = mco_desc_init(coro_entry, 0);
    // mco_create(&co, &desc);

    tex_background_rigth = LoadTexture("src/sprites/right_chapter_bg.png");
    tex_background_updown = LoadTexture("src/sprites/updown_chapter_bg.png");
    tex_background_left = LoadTexture("src/sprites/left_chapter_bg.png");

    tex_game_field = LoadTexture("src/sprites/bg2.png");
    tex_diff = LoadTexture("src/sprites/difficult.png");
    tex_hi_score = LoadTexture("src/sprites/hi_score.png");
    tex_score = LoadTexture("src/sprites/score.png");
    tex_hit_points_text = LoadTexture("src/sprites/player.png");
    tex_power = LoadTexture("src/sprites/power.png");
    tex_graze = LoadTexture("src/sprites/graze.png");
    tex_num = LoadTexture("src/sprites/lannumbers_font.png");
    tex_hit_points = LoadTexture("src/sprites/reimu_hit_points.png");
}

void game_loop() {
    ///*
    InitAudioDevice();
    init();

    while(!WindowShouldClose()) {
        //PlaySound(LoadSound("src/sounds/bullet_sound.mp3"));
        update();
        BeginDrawing(); 
        {
            ClearBackground(RAYWHITE);
            draw();
        } 
        EndDrawing();
    }

    CloseAudioDevice();
    //*/
}
