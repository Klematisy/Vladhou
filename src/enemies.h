#pragma once

typedef struct Item {
    float x;
    float y;
    float width;
    float height;
    float y_speed;
} Item;

extern Item item_power[100];

void enemy_update();
void enemy_draw();
void enemy_init();
int get_enemy_count();
void delete_enemy();
void delete_item_power(int i);
float get_enemy_bullet_x(int j);
float get_enemy_bullet_y(int j);
int get_count_bullet();
int get_count_pows();
float get_enemy_bullet_width(int j);
float get_enemy_bullet_height(int j);
