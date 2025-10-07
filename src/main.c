#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"
#include "jogador.h"
#include "tiros.h"

int main(void){

//parametros 
    //tela (tela cheia de 24")
    const int largura_tela = 1920;
    const int altura_tela = 1080;
    //const int largura_tela = 1200;
    //const int altura_tela = 600;

    //pre configuracoes do jogador
    Jogador jogador;
    Vector2 Posicao_Inicial_Jogador = (Vector2){largura_tela/2, altura_tela/2};

    InitWindow(largura_tela, altura_tela, "JOGO");
    SetTargetFPS(60);
    
    //inicializacao
    IniciarJogador(&jogador, Posicao_Inicial_Jogador);
    IniciarTiros();

    //looping do jogo
    while(!WindowShouldClose()){
        
        //atualiza o jogador
        JogadorUpdate(&jogador);
        AtualizarTiros();
        
        //gera as imagens
        BeginDrawing();
            ClearBackground(RAYWHITE);

            //desenho do Jogador
            JogadorImagem(jogador);

            //barras de vida e habilidades
            JogadorEnergeticoImagem(jogador);
            JogadorVidaImagem(jogador);
            Tiro_Imagem_Jogador();
                
        EndDrawing();
    }
    //encerramento
    CloseWindow();
    return 0;
}