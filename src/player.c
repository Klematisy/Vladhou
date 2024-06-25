#include <stdio.h>
#include <math.h>
#include "raylib.h"
#include "enemies.h"
#include "player.h"
#include "subjects.h"

Player player;
Bullet player_bullet[1000];
Bullet power_bullet[1000];
Objects player_power[4]; 

int player_bullet_count = 0;
int rotation_k_right = 0;
int rotation_k_left = 0;
int animation_start = 0;
int timer = 0;
int opacity = 255;
int count_power = 0;
int power_delta = 100;
int power_bullet_count = 0;

//Game-counts---------------------//

int points = 0;
double power = 2;

//--------------------------------//

Texture2D tex_player;
Texture2D tex_hud;
Texture2D tex_bullet;
Texture2D tex_pow_bullet;
Texture2D p_pow;

Sound bullet_sound;
Sound player_death;

void delete_pow_bullet(int i) {
    for (int j = i; j < power_bullet_count - 1; j++) {
        power_bullet[j] = power_bullet[j + 1];
    }
    power_bullet_count--;
}

void create_pow_bullet() {
    //Bullet* pb = &power_bullet[power_bullet_count];

    for (int i = 0; i < (int) floor(power); i++) {
        power_bullet[power_bullet_count].x = player_power[i].x - 5;
        power_bullet[power_bullet_count++].y = player_power[i].y - 10;

        power_bullet[power_bullet_count].x = player_power[i].x + 5;
        power_bullet[power_bullet_count++].y = player_power[i].y - 10;
    }
}

void create_bullet(int delta) {
    Bullet* b = &player_bullet[player_bullet_count];
    *b = (Bullet){0};
    b->x = player.x + delta;
    b->y = player.y;
    b->x_speed = 20;
    b->y_speed = -20;
    b->height = 16;
    b->width = 14;

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

    Objects* pS = &player_power[0];
    pS->x_speed += 4;
    pS->y_speed += 4;

    for (int i = 0; i < (int) floor(power); i++) {
        Objects* pp = &player_power[i];
                
        switch ((int) floor(power)) { 
            case 2:
                pp->y = sin((i + 1) * (PI / 2 + PI / 2) + pS->y_speed * PI / 180) * power_delta + player.y + player.height / 2 - 20;
                pp->x = cos((i + 1) * (PI / 2 + PI / 2) + pS->x_speed * PI / 180) * power_delta + player.x + player.width / 2 - 15;
                break;
            case 3:
                pp->y = sin((i + 1) * (PI / 2 + PI / 6) + pS->y_speed * PI / 180) * power_delta + player.y + player.height / 2 - 20;
                pp->x = cos((i + 1) * (PI / 2 + PI / 6) + pS->x_speed * PI / 180) * power_delta + player.x + player.width / 2 - 15;
                break;
            default:
                pp->y = sin((i + 1) * (PI / 2) + pS->y_speed * PI / 180) * power_delta + player.y + player.height / 2 - 20;
                pp->x = cos((i + 1) * (PI / 2) + pS->x_speed * PI / 180) * power_delta + player.x + player.width / 2 - 15;
        } 
    }   

    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        spd = 3;
        rotation_k_right+=3;
        rotation_k_left-=3;
        if (power_delta > 40)
            power_delta-=10;
    } else {
        if (power_delta < 100) {
            power_delta+=10;
        }
    }

    if (IsKeyDown(KEY_RIGHT)) {
        if (animation_start == 0) {player.frame_index = 8;}

        if (player.x + 35 < 650) {
            player.x_speed = spd;
        }

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
            create_pow_bullet();
        }
    }
    

    for (int i = 0; i < player_bullet_count; i++) {
        player_bullet[i].y += player_bullet[i].y_speed;

        if (player_bullet[i].y < 10) {
            delete_bullet(i, "");
            i--;
        }        
    }

    for (int i = 0; i < power_bullet_count; i++) {
        power_bullet[i].y -= 20;

        if (power_bullet[i].y < -20) {
            delete_pow_bullet(i);
        }
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

        DrawTexturePro(tex_player, src, dest, (Vector2) {16, 24}, 0, (Color) {255, 255, 255, opacity});
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
                120,
                114.375
            };

            DrawTexturePro(tex_hud, src_hud, dest_hud, (Vector2) {60, 57.1875}, rotation_k_right, WHITE);
            DrawTexturePro(tex_hud, src_hud, dest_hud, (Vector2) {60, 57.1875}, rotation_k_left, WHITE);
        }
      //DrawRectangle(player.x + 20 - 7.5, player.y + 27 - 7.5, 10, 10, WHITE);
    }

    { //Player power
        for (int i = 0; i < power_bullet_count; i++) {
            DrawTexturePro(tex_pow_bullet, (Rectangle) {0, 0, 54, 6}, (Rectangle) {power_bullet[i].x, power_bullet[i].y, 54, 6}, (Vector2) {27, 3}, -90,  WHITE);
        }

        for (int i = 0; i < (int) floor(power); i++) {
            DrawTexturePro(p_pow, (Rectangle) {0, 0, 15, 15}, (Rectangle) {player_power[i].x, player_power[i].y, 30, 30}, (Vector2) {15, 15}, 0,  WHITE);
        }
    }

}

void player_init() {

    player = (Player) {0};
    player.x = 350;
    player.y = 662;
    player.hit_points = 2;
    player.frame_index = 0.25;
    player.height = 94;
    player.width = 60;

    player_power[0].y_speed = 0;
    player_power[0].x_speed = 0;

    tex_player = LoadTexture("sprites/REIMU_SPRITES.png");
    tex_hud = LoadTexture("sprites/reimu_shift.png");
    tex_bullet = LoadTexture("sprites/reimu_bullet.png");
    p_pow = LoadTexture("sprites/reimu_power.png");
    tex_pow_bullet = LoadTexture("sprites/power_bullet.png");

    bullet_sound = LoadSound("sounds/bullet_sound.wav");
    player_death = LoadSound("sounds/player_death.wav");
    
}

int get_player_bullet_count() {
    return player_bullet_count;
}

int get_points() {
    return points;
}