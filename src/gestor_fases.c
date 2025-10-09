#include "jogador.h"
#include "fases_estados.h"
#include "gestor_fases.h"
#include "fase_1.h"

void Atualizar_Jogo(Estados_Jogo *estado, Jogador *jogador){

    switch (*estado){ //essas funcoes devem serd eclaradas nas bibliotecas de cada fase

    case ESTADO_MENU:
        //Atualizar_Menu(estado);
        break;
    
    case ESTADO_FASE_1:
        Atualizar_Fase_1(estado, jogador);
        break;

    case ESTADO_FASE_2:
        //Atualizar_Fase_2(estado, jogador);
        break;

    case ESTADO_GAMEOVER:
        //Atualizar_GameOver(estado);
        break;

    default:
        break;
    }
}

void Desenhar_Jogo(Estados_Jogo estado, Jogador jogador){

    switch (estado){ //essas funcoes devem serd eclaradas nas bibliotecas de cada fase
    case ESTADO_MENU:
        //DesenharMenu();
        break;
    
    case ESTADO_FASE_1:
        DesenharFase1(jogador);
        break;

    case ESTADO_FASE_2:
        //DesenharFase2(jogador);
        break;

    case ESTADO_GAMEOVER:
        //DesenharGameOver(estado);
        break;
        
    default:
        break;
    }
}