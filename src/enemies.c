#include <stdio.h>
#include "enemies.h"
#include <raylib.h>
#include <math.h>

#define array_length(a) (sizeof(a) / sizeof(a[0]))

typedef struct Objects {
    float x;
    float y;
    float radius;
    float frame_index;
} Objects;

Texture2D tex_pixi;

int timer_enemy = 0;
int enemies_count = 0;
int centre_x = 350;
float pi = 3.14159265359;

Objects pixi[25];

void create_enemy() {
    Objects* p  = &pixi[enemies_count];
    *p = (Objects) {0};

    enemies_count++;
}

void delete_enemy(int j) {
    for (int i = j; i < array_length(pixi); i++) {
        pixi[i] = pixi[i + 1];
    }
    
}

void enemy_update() {
    timer_enemy++;

    if ((timer_enemy % 30 == 0) && (enemies_count < array_length(pixi))) {
        create_enemy();
        create_enemy();
        timer_enemy = 0;
    }

    for (int i = 0; i < enemies_count; i++) {
        pixi[i].y++;

        if (i % 2 == 0) {
            pixi[i].x = (sin(pixi[i].y * pi / 180) * 200) + centre_x;
        } else {
            pixi[i].x = (cos((pixi[i].y * pi / 180) + pi / 2) * 200) + centre_x;
        }
    }

    printf("%d\n", enemies_count);
}

void enemy_draw() {
    for (int i = 0; i < enemies_count; i++) {
        DrawTexture(tex_pixi, pixi[i].x, pixi[i].y, WHITE);
    }
}

void enemy_init() {
    tex_pixi = LoadTexture("src/sprites/Enemy.png");
}