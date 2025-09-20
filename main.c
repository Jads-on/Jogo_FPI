#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "raymath.h"

int main(void){

//parametros 
    //tela (tela cheia de 24")
    const int largura_tela = 1920;
    const int altura_tela = 1080;

    //inicializacao
    InitWindow(largura_tela, altura_tela, "JOGO");
    
    //looping do jogo
    while(!WindowShouldClose()){
        
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
        EndDrawing();
    }

    //encerramento
    CloseWindow();
    return 0;
}