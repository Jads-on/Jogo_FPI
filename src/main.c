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
#include "menu.h"
#include "historia.h"
#include "creditos.h"
#include "gestor_audio.h"
#include "baterias.h"
#include "inimigos.h"

int main(void){

    //parametros 
        //tela (tela cheia de 24")
        const int largura_tela = 1920;
        const int altura_tela = 1080;
        //const int largura_tela = 1366;
        //const int altura_tela = 768;

        //pre configuracoes do jogador
        Jogador jogador;
        Vector2 Posicao_Inicial_Jogador = (Vector2){400, altura_tela};

        //inicializacao da maquina de estados
        static Estados_Jogo estado_atual = ESTADO_MENU,
                            estado_anterior = ESTADO_MENU;
       

    //inicializacao da janela e pre set do fps
    InitWindow(largura_tela, altura_tela, "SYNTHETIC");
    SetTargetFPS(60);
    Iniciar_Menu();
   
    //inicializacao
    IniciarJogador(&jogador, Posicao_Inicial_Jogador);
    IniciarInimigos();
    IniciarTiros();
    IniciarBaterias();
    Iniciar_Gestor_Audio();
    Transicao_musica(estado_atual); //inicia a musica quando no ocorreu transicao ainda
    
    //looping do jogo
    while (estado_atual != ESTADO_SAIR) {
   
    if (WindowShouldClose()) estado_atual = ESTADO_SAIR;

    //sempre atualiza a musica
    AtualizarMusica();

    // Atualização (controla teclas e estados) - basicamente controla o jogo
    switch (estado_atual) {
         
        case ESTADO_MENU:
            Atualizar_Menu(&estado_atual, &jogador);
            break;
        default:
    
            Atualizar_Jogo(&estado_atual, &jogador); //o jogo todo ocorre aqui
            break;
    }

    //detecta transicao de estados para mudar a musica
     if (estado_atual != estado_anterior) {
            Transicao_musica(estado_atual);
            estado_anterior = estado_atual;  
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
    DescarregarBateria();
    Encerrar_Gestor_Audio();
    CloseWindow();
    return 0;
}