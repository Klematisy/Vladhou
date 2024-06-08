#include <stdio.h>
#include "raylib.h"
#include "game.h"
#include "enemies.h"
#include "player.h"
#include <math.h>
#include "minicoro.h"

int back_animation = 0;

Texture2D tex_background;
Texture2D tex_game_field;
// mco_coro* co;

// void coro_entry(mco_coro* co) {
//     printf("coroutine 1\n");
//     for (int i = 0; i < 60; i++) {
//         mco_yield(co);
//     }
//     printf("coroutine 2\n");
// }

void update() {
    enemy_update();
    back_animation--;
    player_update();

    // if (mco_status(co) == MCO_SUSPENDED) {
    //     mco_resume(co);
    // }
}

void draw() {
    DrawTexturePro(tex_background,                                               //Drawing Background
        (Rectangle){0, 0, 640, 480}, 
        (Rectangle){0, 0, GetScreenWidth() * 1.5, GetScreenHeight() * 1.5},      
        (Vector2) {320, 240}, 
        0,
        WHITE
    );

    BeginScissorMode(50, 39, 600, 690);

        DrawTexturePro(tex_game_field,                                               //Drawing Game field
            (Rectangle){0, back_animation, 512, 512},
            (Rectangle){50, 39, 600, 690},      
            (Vector2) {0, 0},
            0,
            WHITE
        );
        
        enemy_draw();
        player_draw();
    
    EndScissorMode();
}

void init() {
    SetMasterVolume(0.1);
    player_init();
    enemy_init();

    // mco_desc desc = mco_desc_init(coro_entry, 0);
    // mco_create(&co, &desc);

    tex_background = LoadTexture("src/sprites/bg.png");
    tex_game_field = LoadTexture("src/sprites/bg2.png");
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