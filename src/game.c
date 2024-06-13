#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "raylib.h"
#include "game.h"
#include "enemies.h"
#include "player.h"
#include "minicoro.h"

int back_animation = 0;

Texture2D tex_background;
Texture2D tex_game_field;
Texture2D tex_diff;
Texture2D tex_hi_score;
Texture2D tex_score;
Texture2D tex_hit_points;
Texture2D tex_power;
Texture2D tex_graze;
Texture2D tex_num;

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
    DrawTexturePro(tex_background,                              //Drawing Background
        (Rectangle){0, 0, 640, 480}, 
        (Rectangle){0, 0, GetScreenWidth() * 1.5, GetScreenHeight() * 1.5},      
        (Vector2) {320, 240}, 
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

    DrawTexturePro(tex_hit_points, (Rectangle) {0, 0, 63, 20}, (Rectangle) {761, 200, 110.675676, 35.5555}, (Vector2) {65, 16}, 0, WHITE);

    DrawTexturePro(tex_power, (Rectangle) {0, 0, 61, 18}, (Rectangle) {761, 240, 107.162162, 32}, (Vector2) {65, 16}, 0, WHITE);

    DrawTexturePro(tex_graze, (Rectangle) {0, 0, 58, 18}, (Rectangle) {761, 278, 101.891892, 32}, (Vector2) {65, 16}, 0, WHITE);


    char table_score[16];
    snprintf(table_score, sizeof(table_score), "%09d", score);

    printf("%s\n", table_score);

    int x;
    int d = 0;
    for (int i = 0; i < strlen(table_score); i++) {
        x = 16;
        switch (table_score[i]) {
            case '0': x*=0;
                break;
            case '1': x*=1;
                break;
            case '2': x*=2;
                break;
            case '3': x*=3;
                break;
            case '4': x*=4;
                break;
            case '5': x*=5;
                break;
            case '6': x*=6;
                break;
            case '7': x*=7;
                break;
            case '8': x*=8;
                break;
            case '9': x*=9;
                break;
        }
        DrawTexturePro(tex_num, (Rectangle) {x, 0, 16, 16}, (Rectangle) {828 + d, 111, 28, 28}, (Vector2) {14, 14}, 0, WHITE);
        DrawTexturePro(tex_num, (Rectangle) {x, 0, 16, 16}, (Rectangle) {828 + d, 144, 28, 28}, (Vector2) {14, 14}, 0, WHITE);
        d+=24;
    }

    //DrawText("NORMAL", 720, 50, 40, WHITE);
}

void init() {
    SetMasterVolume(0.1);
    player_init();
    enemy_init();

    // mco_desc desc = mco_desc_init(coro_entry, 0);
    // mco_create(&co, &desc);

    tex_background = LoadTexture("src/sprites/bg.png");
    tex_game_field = LoadTexture("src/sprites/bg2.png");
    tex_diff = LoadTexture("src/sprites/difficult.png");
    tex_hi_score = LoadTexture("src/sprites/hi_score.png");
    tex_score = LoadTexture("src/sprites/score.png");
    tex_hit_points = LoadTexture("src/sprites/player.png");
    tex_power = LoadTexture("src/sprites/power.png");
    tex_graze = LoadTexture("src/sprites/graze.png");
    tex_num = LoadTexture("src/sprites/lannumbers_font.png");
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
