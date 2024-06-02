#include <stdio.h>
#include "enemies.h"
#include "player.h"
#include <raylib.h>
#include <math.h>

#define array_length(a) (sizeof(a) / sizeof(a[0]))

typedef struct Objects {
    float x;
    float y;
    int number;
    float hit_points;
    float radius;
    float frame_index;
    float height;
    float width;
} Objects;

//-------------CONSTANTS-----------------//

const float pi = 3.14159265359;
const int centre_x = 350;

//--------------------------------------//

Objects pixi[25];

Texture2D tex_pixi;
Sound pixi_death;

int timer_enemy = 0;
int enemies_count = 0;
int create_enemies = 0;

/*-------------------MAIN FUNCTIONS-------------------*/

void create_enemy() {
    Objects* p  = &pixi[enemies_count];
    *p = (Objects) {0};
    p->number = enemies_count;
    p->hit_points = 1;
    p->height = 45;
    p->width = 40.5;

    create_enemies++;
    enemies_count++;
}

void delete_enemy(int j) {
    for (int i = j; i < enemies_count - 1; i++) {
        pixi[i] = pixi[i + 1];
    }
    PlaySound(pixi_death);
    enemies_count--;
}

void collision_calc(int i) {
    for (int j = 0; j < get_player_bullet_count(); j++) {
        if ((get_x_bullet(j) < pixi[i].x + 30) && (get_x_bullet(j) > pixi[i].x) && (get_y_bullet(j) > pixi[i].y) && (get_y_bullet(j) < pixi[i].y + 27)) {
            if (pixi[i].hit_points == 0) {
                delete_enemy(i);
            } else {
                pixi[i].hit_points--;
            }
            delete_bullet(j);
        }
    }
}

void enemy_update() {
    timer_enemy++;

    if ((timer_enemy == 25) && (create_enemies < 25)) {
        timer_enemy = 0;
        create_enemy();
    }

    for (int i = 0; i < enemies_count; i++) {
        pixi[i].y++;
        
        if (pixi[i].number % 2 == 0) {
            pixi[i].x = (sin(pixi[i].y * pi / 180) * 200 + centre_x);
        } else {
            pixi[i].x = (cos(pi / 2 + pixi[i].y * pi / 180) * 200 + centre_x);
        }

        collision_calc(i);

        if (pixi[i].y > 768) {
            delete_enemy(i);
        }
    }

}

void enemy_draw() {
    for (int i = 0; i < enemies_count; i++) {
        //DrawTexture(tex_pixi, pixi[i].x, pixi[i].y, WHITE);
        DrawTexturePro(tex_pixi, (Rectangle) {0, 0, 30, 27}, (Rectangle) {pixi[i].x, pixi[i].y, pixi[i].height, pixi[i].width}, (Vector2) {22.5, 20.25}, 0, WHITE);
    }
}

void enemy_init() {
    tex_pixi = LoadTexture("src/sprites/Enemy.png");
    pixi_death = LoadSound("src/sounds/pixi_death.wav");
}

/*----------------------------------------------------*/