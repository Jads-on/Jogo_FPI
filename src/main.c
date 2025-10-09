#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"
#include "jogador.h"
#include "tiros.h"
#include "fases_estados.h"
#include "gestor_fases.h"

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

        //inicializacao da maquina de estados
        static Estados_Jogo estado_atual = ESTADO_FASE_1; //mudar para menu apos implementar

    //inicializacao da janela e pre set do fps
    InitWindow(largura_tela, altura_tela, "SYNTHETIC");
    SetTargetFPS(60);
    
    //inicializacao
    IniciarJogador(&jogador, Posicao_Inicial_Jogador);
    IniciarTiros();

    //looping do jogo
    while(estado_atual != ESTADO_SAIR){
        
        if(WindowShouldClose()){ //otimizacao pra fechar o jogo
            estado_atual = ESTADO_SAIR;
        }

        Atualizar_Jogo(&estado_atual, &jogador); //controlado pela maquina de estados
       
        //gerar as imagens
        BeginDrawing();

            ClearBackground(RAYWHITE);
            Desenhar_Jogo(estado_atual, jogador);

        EndDrawing();
    }

    //encerramento
    DescarregarAssets();
    CloseWindow();
    return 0;
}