#include <stdio.h>
#include <raylib.h>
#include "game.h"

void init_window() {

    InitWindow(1100, 768, "Vladhou 11 (название перманентное)");
    SetTargetFPS(60);

}

int main() {
    
    init_window(); //initialise window 
    game_loop(); //main game loop in game.c
    CloseWindow();
    
    return 0;
}
