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
    float width;
    float height;
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
int shooting_enemy = 0;

int created_bullets = 0;
int bullets_count = 0;

//----------------------Костыли-----------------------//



//----------------------------------------------------//

/*-------------------MAIN FUNCTIONS-------------------*/

void create_enemy_bullet(int i, int delta_x, int delta_y, int d_x, int d_y) {
    Bullet* b = &pixi_bullet[i];
    *b = (Bullet){0};
    
    int enemy = shooting_enemy;
    
    b->y = pixi[enemy].y + d_y;
    b->x = pixi[enemy].x + d_x;

    b->k = ((pixi[enemy].y - get_player_y() + delta_y) / (pixi[enemy].x - get_player_x() + delta_x));
    b->b = pixi[enemy].y - b->k * (pixi[enemy].x);

    pixi_bullet[i].width = 25;
    pixi_bullet[i].height = 25;

    bullets_count++;
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
    p->hit_points = 2;
    p->height = 45;
    p->width = 40.5;

    created_enemies++;
    enemies_count++;
}

void delete_enemy(int j) {
    for (int i = j; i < enemies_count - 1; i++) {
        pixi[i] = pixi[i + 1];
    }
    if (shooting_enemy > j) {
        shooting_enemy--;
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
            delete_bullet(j, "pixi");
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

    
    if ((created_enemies > 2) && (shooting_enemy < enemies_count) && (timer_enemy % 25 == 0)) {
        create_enemy_bullet(bullets_count, 0, 0, 0, 0);
        create_enemy_bullet(bullets_count, -30, -10, 10, 10);
        create_enemy_bullet(bullets_count, 10, 0, 5, 15);
        create_enemy_bullet(bullets_count, -50, 0, -5, 15);
        shooting_enemy++;
    }

    
    for (int i = 0; i < bullets_count; i++) {
        pixi_bullet[i].y+=2;
        pixi_bullet[i].x = (pixi_bullet[i].y - pixi_bullet[i].b) / pixi_bullet[i].k;

        if ((pixi_bullet[i].y > 768) && (pixi_bullet[i].y < -10)) {
            delete_enemy_bullet(i);
        }
    }
}

void enemy_draw() {
      for (int i = 0; i < bullets_count; i++) {
        //DrawTexture(tex_pixi_bullet, pixi_bullet[i].x, pixi_bullet[i].y, WHITE);
        DrawTexturePro(tex_pixi_bullet, (Rectangle) {0, 0, 16, 16}, (Rectangle) {pixi_bullet[i].x, pixi_bullet[i].y, pixi_bullet[i].width, pixi_bullet[i].height}, (Vector2) {8, 8}, 0, WHITE);
      }

      for (int i = 0; i < enemies_count; i++) {
        DrawTexturePro(tex_pixi, (Rectangle) {0, 0, 30, 27}, (Rectangle) {pixi[i].x, pixi[i].y, pixi[i].height, pixi[i].width}, (Vector2) {22.5, 20.25}, 0, WHITE);
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

float get_enemy_bullet_width(int j) {
    return pixi_bullet[j].width;
}

float get_enemy_bullet_height(int j) {
    return pixi_bullet[j].height;
}

int get_count_bullet() {
    return bullets_count;
}

/*----------------------------------------------------*/
