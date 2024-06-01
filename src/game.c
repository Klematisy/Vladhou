#include <stdio.h>
#include "raylib.h"
#include "game.h"
#include "enemies.h"
#include "player.h"
#include <math.h>

int back_animation = 0;

Texture2D tex_background;
Texture2D tex_game_field;

void update() {
    //enemy_update();
    back_animation--;
    player_update();
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
        
        //enemy_draw();
        player_draw();
    
    EndScissorMode();
}

void init() {
    player_init();
    //enemy_init();

    tex_background = LoadTexture("src/sprites/bg.png");
    tex_game_field = LoadTexture("src/sprites/bg2.png");
}

int game_loop() {
    ///*
    init();

    while(!WindowShouldClose()) {
        update();
        BeginDrawing();
            ClearBackground(RAYWHITE);
            draw();
        EndDrawing();
    }
    CloseWindow(); 
    //*/

    printf("flkjdsalkfj");
    return 0;
}