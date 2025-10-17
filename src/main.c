#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <menu.h>
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
        static Estados_Jogo estado_atual = ESTADO_MENU;

    //inicializacao da janela e pre set do fps
    InitWindow(largura_tela, altura_tela, "SYNTHETIC");
    SetTargetFPS(60);
    Iniciar_Menu();

    
    //inicializacao
    IniciarJogador(&jogador, Posicao_Inicial_Jogador);
    IniciarTiros();

    //looping do jogo
    while (estado_atual != ESTADO_SAIR) {

    if (WindowShouldClose()) estado_atual = ESTADO_SAIR;

    // Atualização (controla teclas e estados)
    switch (estado_atual) {
        case ESTADO_MENU:
            Atualizar_Menu(&estado_atual, &jogador);
            break;
        default:
            Atualizar_Jogo(&estado_atual, &jogador);
            break;
    }

    // Desenho (tudo dentro de um único Begin/End)
    BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (estado_atual) {
            case ESTADO_MENU:
                Desenhar_Menu();
                break;
            default:
                Desenhar_Jogo(estado_atual, jogador);
                break;
        }

    EndDrawing();
}


    //encerramento
    Encerrar_Menu();
    DescarregarAssets();
    CloseWindow();
    return 0;
}