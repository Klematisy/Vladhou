#include <stdio.h>
#include <raylib.h>
#include "subjects.h"
#include "enemies.h"
#include "player.h"
#include <math.h>

float deg_to_rad(float x) { 
    return x / 180.0f * PI; 
}

float rad_to_deg(float x) { 
    return x / PI * 180.0f; 
}

float point_direction(float x1, float y1, float x2, float y2) {
    return rad_to_deg(atan2f(y1 - y2, x2 - x1));
}

void collision_calc() {
  //*
  for (int i = 0; i < pow_count; i++) {
        Player* p = &player;
        Item* pow = &item_power[i];

        if ((p->x - 100 < pow->x + pow->width / 2) &&
            (pow->x + pow->width / 2 < p->x + p->width + 100) &&
            (p->y - 100 < pow->y + pow->height / 2) && 
            (pow->y + pow->height / 2 < p->y + p->height + 100)) {
                
                pow->x += 10 * cosf(deg_to_rad(point_direction(pow->x, pow->y, p->x, p->y)));
                pow->y += 10 * -sinf(deg_to_rad(point_direction(pow->x, pow->y, p->x, p->y)));   
        }
        if ((p->x < pow->x + pow->width / 2) &&
            (pow->x + pow->width / 2 < p->x + p->width) &&
            (p->y < pow->y + pow->height / 2) &&
            (pow->y + pow->height / 2 < p->y + p->height)) {
                delete_item_power(i);
                PlaySound(LoadSound("sounds/power_item.wav"));
                i--;
                power+=0.05;
        }
  }

    if (opacity >= 255) {
        for (int i = 0; i < get_count_bullet(); i++) {
            Bullet* pb = &pixi_bullet[i];

            if ((player.x + 20 - 7.5 <= pb->x  + 10) &&
                (pb->x + 10 <= player.x + 20 - 7.5 + 10) &&
                (player.y + 27 - 7.5 < pb->y + 10) &&
                (pb->y + 10 < player.y + 27 - 7.5 + 10))
            {
                PlaySound(LoadSound("sounds/player_death.wav"));
                player.hit_points--;
                opacity = 0;
            }
        }
    } else {
        opacity+=5;
    }


    for (int i = 0; i < enemies_count; i++) {
        for (int j = 0; j < get_player_bullet_count(); j++) {
            Bullet* pb = &player_bullet[j];
            if ((pb->x < pixi[i].x + pixi[i].width) && 
                (pixi[i].x < pb->x) && 
                (pixi[i].y < pb->y) && 
                (pb->y < pixi[i].y + pixi[i].height)) {
                if (pixi[i].hit_points == 0) {
                   delete_enemy(i, "");    
                } else {
                    pixi[i].hit_points--;
                }
                delete_bullet(j, "pixi");
            }
        }

        for (int j = 0; j < power_bullet_count; j++) {
            Bullet* pb = &power_bullet[j];
            if ((pb->x < pixi[i].x + pixi[i].width) &&
                (pixi[i].x < pb->x) &&
                (pixi[i].y < pb->y) &&
                (pb->y < pixi[i].y + pixi[i].height)) {
                    if (pixi[i].hit_points == 0) {
                        delete_enemy(i, "");    
                    } else {
                        pixi[i].hit_points--;
                    }
                    delete_pow_bullet(j);
            }
        }
    }
 
    //*/
}
