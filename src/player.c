#include <stdio.h>
#include "raylib.h"
#include "enemies.h"
#include "player.h"
#include <math.h>

typedef struct Player {
    float x;
    float y;
    float x_speed;
    float y_speed;
    float frame_index;
} Player;

typedef struct Collision {
    float x;
    float y;
    float height;
    float width;
} Collision;

typedef struct Bullet {
    float x;
    float y;
    float x_speed;
    float y_speed;
} Bullet;

Player player;
Bullet player_bullet[1000];
Collision player_col;
Collision bullet_col[100];

int player_bullet_count = 0;
int rotation_k_right = 0;
int rotation_k_left = 0;
int animation_start = 0;
int points = 0;
int timer = 0;

Texture2D tex_player;
Texture2D tex_hud;
Texture2D tex_bullet;
Sound bullet_sound;
Sound player_death;

void create_bullet(int delta) {
    Bullet* b = &player_bullet[player_bullet_count];
    *b = (Bullet){0};
    b->x = player.x + delta;
    b->y = player.y;
    b->x_speed = 20;
    b->y_speed = -20;

    bullet_col[delta].height = 16;
    bullet_col[delta].width = 14;

    player_bullet_count++;
}

void delete_bullet(int i, char enemy_type[]) {
    for (int j = i; j < player_bullet_count - 1; j++) {
        player_bullet[j] = player_bullet[j + 1];
    }
    player_bullet_count--;
    if (enemy_type == "pixi") {
        points+=200;
    }
}

void player_update() {
    player.x_speed = 0;
    player.y_speed = 0;

    float spd = 7;
    points = 0;
 
    for (int i = 0; i < get_count_bullet(); i++) {
        if ((player_col.x + 20 - 7.5 <= get_enemy_bullet_x(i) + 10) &&
            (get_enemy_bullet_x(i) + 10 <= player_col.x + 20 - 7.5 + 10) &&
            (player_col.y + 27 - 7.5 < get_enemy_bullet_y(i) + 10) &&
            (get_enemy_bullet_y(i) + 10 < player_col.y + 27 - 7.5 + 10))
        {
            PlaySound(player_death);
        }
    }

    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        spd = 3;
        rotation_k_right+=3;
        rotation_k_left-=3;
    }

    if (IsKeyDown(KEY_RIGHT)) {
        if (animation_start == 0) {player.frame_index = 8;}

        if (player.x + 35 < 650) {
            player.x_speed = spd;
        }

        player_col.x = player.x;
        player_col.y = player.y;

        player.frame_index += 0.25;
        if (player.frame_index > 7) {
            player.frame_index = animation_start;
            animation_start = 4;
        }
    } else if (IsKeyDown(KEY_LEFT)) {
        if (animation_start == 0) {player.frame_index = 8;}

        if (player.x > 52) {
            player.x_speed = -spd;
        }

        player_col.x = player.x;
        player_col.y = player.y;

        player.frame_index += 0.25;
        if (player.frame_index > 7) {
            player.frame_index = animation_start;
            animation_start = 4;
        }
    } else {
        player.frame_index += 0.25;
        if (player.frame_index > 7) {
            player.frame_index = 0;
            animation_start = 0;
        }
    }
    
    if (IsKeyDown(KEY_UP)) {
        if (player.y > 60) {
            player.y_speed = -spd;
        }
    }
    if (IsKeyDown(KEY_DOWN)) {
        if (player.y + 75 < 729) {
            player.y_speed = spd;
        }
    }


    player.x += player.x_speed;
    player.y += player.y_speed;

    timer++;
    
    if (timer % 4 == 0) {
        if (IsKeyDown(KEY_Z)) {
            PlaySound(bullet_sound);
            create_bullet(0);
            create_bullet(38);
        }
    }


    for (int i = 0; i < player_bullet_count; i++) {
        player_bullet[i].y += player_bullet[i].y_speed;

        if (player_bullet[i].y < 10) {
            delete_bullet(i, "");
            i--;
        }
        bullet_col[i].y = player_bullet[i].y;
        bullet_col[i].x = player_bullet[i].x;
    }
}

void player_draw() {
    for (int i = 0; i < player_bullet_count; i++) {                              //Drawing Bullets
        DrawTexture(tex_bullet, 
            player_bullet[i].x - 8,
            player_bullet[i].y + 24 - 2.5, 
            WHITE);
    }

    { //Player animation
        int y = 3;
        int x = ((int) player.frame_index) * 32 + 2;

        {
            if (IsKeyDown(KEY_LEFT)) {
                y = 48;
            }

            if (IsKeyDown(KEY_RIGHT)) {
                y = 96;
            }
        }

        Rectangle src = (Rectangle){
            x,
            y,
            32,
            48
        };
        
        Rectangle dest = (Rectangle) {
            player.x,
            player.y,
            60,
            94
        };

        DrawTexturePro(tex_player, src, dest, (Vector2) {16, 24}, 0, WHITE);
    }

    { //Hud
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            Rectangle src_hud = (Rectangle){
                0,
                0,
                64,
                61
            };
            
            Rectangle dest_hud = (Rectangle) {
                player.x + 17,
                player.y + 24,
                96,
                91.5
            };

            DrawTexturePro(tex_hud, src_hud, dest_hud, (Vector2) {48, 45.75}, rotation_k_right, WHITE);
            DrawTexturePro(tex_hud, src_hud, dest_hud, (Vector2) {48, 45.75}, rotation_k_left, WHITE);
        }
      //DrawRectangle(player.x + 20 - 7.5, player.y + 27 - 7.5, 10, 10, WHITE);
    }
}

void player_init() {

    player = (Player) {0};
    player.x = 350;
    player.y = 662;
    player.frame_index = 0.25;

    player_col.height = 94;
    player_col.width = 60;

    tex_player = LoadTexture("src/sprites/REIMU_SPRITES.png");
    tex_hud = LoadTexture("src/sprites/reimu_shift.png");
    tex_bullet = LoadTexture("src/sprites/reimu_bullet.png");
    bullet_sound = LoadSound("src/sounds/bullet_sound.wav");
    player_death = LoadSound("src/sounds/player_death.wav");
    
}

float get_x_bullet(int j) {
    return player_bullet[j].x;
}

float get_y_bullet(int j) {
    return player_bullet[j].y;
}

int get_player_bullet_count() {
    return player_bullet_count;
}

float get_player_x() {
    return player.x;
}

float get_player_y() {
    return player.y;
}

int get_points() {
    return points;
}
