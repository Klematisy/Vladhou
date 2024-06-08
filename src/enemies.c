#include <stdio.h>
#include <stdlib.h>
#include "enemies.h"
#include "player.h"
#include "minicoro.h"
#include <raylib.h>
#include <math.h>

#define array_length(a) (sizeof(a) / sizeof(a[0]))

typedef struct Objects {
    float x;
    float y;
    int number;
    float hit_points;
    float height;
    float width;
} Objects;

typedef struct Bullet {
    float x;
    float y;
    float k; //angle coefficient
    float b; //Oy
} Bullet;

//-------------CONSTANTS-----------------//

const float pi = 3.14159265359;
const int centre_x = 350;

//--------------------------------------//

Objects pixi[25];
Bullet pixi_bullet[100];

Texture2D tex_pixi;
Texture2D tex_pixi_bullet;
Sound pixi_death;

int timer_enemy = 0;
int enemies_count = 0;
int created_enemies = 0;

int created_bullets = 0;
int bullets_count = 0;

//----------------------Костыли-----------------------//



//----------------------------------------------------//

/*-------------------MAIN FUNCTIONS-------------------*/

void create_enemy_bullet(int i, int d) {
    Bullet* b = &pixi_bullet[bullets_count];
    *b = (Bullet){0};

    b->y = pixi[i].y;
    b->x = pixi[i].x;

    b->k = ((pixi[i].y - get_player_y()) / (pixi[i].x - get_player_x()));
    b->b = pixi[i].y - b->k * pixi[i].x;

    //printf("%d: y = %fx + %f\n", bullets_count, b->k, b->b);

    bullets_count++;
    created_bullets = bullets_count;
    //printf("\nCreate!");
}

void delete_enemy_bullet(int i) {
    for (int j = i; j < bullets_count - 1; j++) {
        pixi_bullet[j] = pixi_bullet[j + 1];
    }
    bullets_count--;
}

void create_enemy() {
    Objects* p  = &pixi[enemies_count];
    *p = (Objects) {0};
    p->y = 10;
    p->number = enemies_count;
    p->hit_points = 1;
    p->height = 45;
    p->width = 40.5;

    created_enemies++;
    enemies_count++;
}

void delete_enemy(int j) {
    for (int i = j; i < enemies_count - 1; i++) {
        pixi[i] = pixi[i + 1];
    }
    PlaySound(pixi_death);
    enemies_count--;
}

void enemy_collision_calc(int i) {
    for (int j = 0; j < get_player_bullet_count(); j++) {
        if ((get_x_bullet(j) < pixi[i].x + pixi[i].height) && (get_x_bullet(j) > pixi[i].x) && (get_y_bullet(j) > pixi[i].y) && (get_y_bullet(j) < pixi[i].y + pixi[i].width)) {
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

    if ((timer_enemy % 25 == 0) && (created_enemies < 25)) {
        create_enemy();
    }

    for (int i = 0; i < enemies_count; i++) {
        pixi[i].y++;
        
        if (pixi[i].number % 2 == 0) {
            pixi[i].x = (sin(pixi[i].y * pi / 180) * 200 + centre_x);
        } else {
            pixi[i].x = (cos(pi / 2 + pixi[i].y * pi / 180) * 200 + centre_x);
        }

        enemy_collision_calc(i);

        if (pixi[i].y > 768) {
            delete_enemy(i);
        }
    }

    if ((timer_enemy % 100 == 0) && (created_bullets < enemies_count)) {
        create_enemy_bullet(bullets_count, 0);
    }
    
    for (int i = 0; i < bullets_count; i++) {
        pixi_bullet[i].y+=5;
        pixi_bullet[i].x = (pixi_bullet[i].y - pixi_bullet[i].b) / pixi_bullet[i].k;

        if ((pixi_bullet[i].y > 768) && (pixi_bullet[i].y < -10)) {
            delete_enemy_bullet(i);
        }
    }
    //printf("\n");
}

void enemy_draw() {
    for (int i = 0; i < enemies_count; i++) {
        DrawTexturePro(tex_pixi, (Rectangle) {0, 0, 30, 27}, (Rectangle) {pixi[i].x, pixi[i].y, pixi[i].height, pixi[i].width}, (Vector2) {22.5, 20.25}, 0, WHITE);
    }
    for (int i = 0; i < bullets_count; i++) {
        DrawTexture(tex_pixi_bullet, pixi_bullet[i].x, pixi_bullet[i].y, WHITE);
    }
}

void enemy_init() {
    tex_pixi = LoadTexture("src/sprites/Enemy.png");
    tex_pixi_bullet = LoadTexture("src/sprites/enemy_bullet.png");

    pixi_death = LoadSound("src/sounds/pixi_death.wav");
}

/*----------------------------------------------------*/

/*----------------------Гетеры------------------------*/

float get_enemy_bullet_x(int j) {
    return pixi_bullet[j].x;
}

float get_enemy_bullet_y(int j) {
    return pixi_bullet[j].y;
}

int get_count_bullet() {
    return bullets_count;
}

/*----------------------------------------------------*/