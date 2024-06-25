#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <math.h>
#include "enemies.h"
#include "player.h"
#include "subjects.h"

#define array_length(a) (sizeof(a) / sizeof(a[0]))

//-------------CONSTANTS-----------------//

const float pi = 3.14159265359;
const int centre_x = 350;

//--------------------------------------//

Objects pixi[25];
Bullet pixi_bullet[100];
Item item_power[100];

Texture2D tex_pixi;
Texture2D tex_item_power;
Texture2D tex_pixi_bullet;

Sound pixi_death;
Sound pick_item;
Sound enemy_shoot;

int timer_enemy = 0;
int enemies_count = 0;
int created_enemies = 0;
int shooting_enemy = 0;

int created_bullets = 0;
int bullets_count = 0;

int pow_count = 0;

//----------------------Костыли-----------------------//



//----------------------------------------------------//

/*-------------------MAIN FUNCTIONS-------------------*/

void delete_item_power(int i) {
    for (int j = i; j < pow_count - 1; j++) {
        item_power[j] = item_power[j + 1];
    }
    pow_count--;
}

void create_pow_item(int j) {
    item_power[pow_count].x = pixi[j].x;
    item_power[pow_count].y = pixi[j].y;
    
    item_power[pow_count].y_speed = -5;

    item_power[pow_count].width = 24;
    item_power[pow_count].height = 24;

    pow_count++;
}

void create_enemy_bullet(int i, int delta_x, int delta_y, int d_x, int d_y) {
    Bullet* b = &pixi_bullet[i];
    *b = (Bullet){0};
    
    int enemy = shooting_enemy;
    
    b->y = pixi[enemy].y + d_y;
    b->x = pixi[enemy].x + d_x;

    b->k = ((pixi[enemy].y - player.y + delta_y) / (pixi[enemy].x - player.x + delta_x));
    b->b = pixi[enemy].y - b->k * (pixi[enemy].x);

    pixi_bullet[i].width = 31,25;
    pixi_bullet[i].height = 31,25;

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
    p->height = 47,5;
    p->width = 43.75;

    created_enemies++;
    enemies_count++;
}

void delete_enemy(int j, char str[]) {
    create_pow_item(j);
    for (int i = j; i < enemies_count - 1; i++) {
        pixi[i] = pixi[i + 1];
    }
    if (shooting_enemy > j) {
        shooting_enemy--;
    }
    if (str != "down") {
        PlaySound(pixi_death);
    }
    enemies_count--;
    
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

        if (pixi[i].y > 900) {
            
            delete_enemy(i, "down");
        }
    }

    
    if ((created_enemies > 3) && (shooting_enemy < enemies_count) && (timer_enemy % 25 == 0)) {
        create_enemy_bullet(bullets_count, 0, 0, 0, 0);
        create_enemy_bullet(bullets_count, -30, -10, 5, 10);
        create_enemy_bullet(bullets_count, 10, 0, 5, 15);
        create_enemy_bullet(bullets_count, -50, 0, -5, 15);
        PlaySound(enemy_shoot);
        shooting_enemy++;
    }

    
    for (int i = 0; i < bullets_count; i++) {
        pixi_bullet[i].y+=2;
        pixi_bullet[i].x = (pixi_bullet[i].y - pixi_bullet[i].b) / pixi_bullet[i].k;

        if ((pixi_bullet[i].y > 768) && (pixi_bullet[i].y < -10)) {
            delete_enemy_bullet(i);
        }
    }
    
    float k;
    float b;

    for (int i = 0; i < pow_count; i++) {
        Player* p = &player;
        Item* pow = &item_power[i];
        
        pow->y += pow->y_speed;
        if (pow->y_speed < 3.5) {
            pow->y_speed += 0.1;
        }
        if (pow->y > 768) {
            delete_item_power(i);
        } 
    }
}

void enemy_draw() {
    for (int i = 0; i < bullets_count; i++) {
        DrawTexturePro(tex_pixi_bullet, (Rectangle) {0, 0, 16, 16}, (Rectangle) {pixi_bullet[i].x, pixi_bullet[i].y, pixi_bullet[i].width, pixi_bullet[i].height}, (Vector2) {8, 8}, 0, WHITE);
    }

    for (int i = 0; i < enemies_count; i++) {
        DrawTexturePro(tex_pixi, (Rectangle) {0, 0, 30, 27}, (Rectangle) {pixi[i].x, pixi[i].y, pixi[i].height, pixi[i].width}, (Vector2) {22.5, 20.25}, 0, WHITE);
    }

    for (int i = 0; i < pow_count; i++) {
        DrawTexturePro(tex_item_power, (Rectangle) {0, 0, 12, 12}, (Rectangle) {item_power[i].x, item_power[i].y, item_power[i].width, item_power[i].height}, (Vector2) {12, 12}, 0, WHITE);
    }
}

void enemy_init() {
    tex_pixi = LoadTexture("sprites/Enemy.png");
    tex_pixi_bullet = LoadTexture("sprites/enemy_bullet.png");
    tex_item_power = LoadTexture("sprites/item_power.png");

    pixi_death = LoadSound("sounds/pixi_death.wav");
    pick_item = LoadSound("sounds/power_item.wav");
    enemy_shoot = LoadSound("sounds/enemy_bullet_sound.wav");
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

int get_count_pows() {
    return pow_count;
}

/*----------------------------------------------------*/
