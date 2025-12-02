#include "jogador.h"
#include "fases_estados.h"
#include "gestor_fases.h"
#include "menu.h"
#include "fase_1.h"
#include "fase_2.h"
#include "gameover.h"
#include "creditos.h"
#include "historia.h"
#include "gestor_audio.h"
#include "boss.h"

Estados_Jogo estado_anterior; 

void Atualizar_Jogo(Estados_Jogo *estado, Jogador *jogador){
    AtualizarMusica();

    switch (*estado){ //essas funcoes devem serd eclaradas nas bibliotecas de cada fase

    case ESTADO_MENU:
        Atualizar_Menu(estado, jogador);
        break;

    case ESTADO_INICIAR_FASE_1:
        Iniciar_Fase_1(estado);
        break;

    case ESTADO_INTRO_FASE_1:
        Atualizar_Intro_Fase_1(estado);
        break;
    
    case ESTADO_FASE_1:
        Atualizar_Fase_1(estado, jogador);
        break;

    case ESTADO_INICIAR_FASE_2:
        Descarregar_Fase_1();
        Iniciar_Fase_2(estado);
        break;

    case ESTADO_FASE_2:
        Boss boss;
        Atualizar_Fase_2(estado, jogador, boss);
        break;
    
    case ESTADO_GAMEOVER:
        Atualizar_GameOver(estado,jogador);
        break;
        
    case ESTADO_HISTORIA:
        Atualizar_Historia(estado);
        break;

    case ESTADO_CREDITOS:
        Atualizar_Creditos(estado);
        break;
    
    case ESTADO_VOLUME:
        AtualizarMusica();
        Controlar_Volume_Musica(estado);
        break;
    
    case ESTADO_SAIR:
        DescarregarAssets();
        DescarregarBateria();
        Descarregar_Fase_1();
        break;

    default:
        break;
    }
}

void Desenhar_Jogo(Estados_Jogo estado, Jogador jogador){

    switch (estado){ //essas funcoes devem serd eclaradas nas bibliotecas de cada fase
    case ESTADO_MENU:
        Desenhar_Menu();
        break;

    case ESTADO_INTRO_FASE_1:
        Desenhar_Intro_Fase1();
        break;
    
    case ESTADO_FASE_1:
        DesenharFase1(jogador);
        Desenhar_Mensagem_Fase_1();
        break;

    case ESTADO_FASE_2:
        //DesenharFase2(jogador);
        break;

    case ESTADO_GAMEOVER:
        Desenhar_GameOver(&jogador);
        break;
    
    case ESTADO_HISTORIA:
        Desenhar_Historia();
        break;
    
     case ESTADO_VOLUME:
        Desenha_Barra_Volume_Musica();
        break;
    
    case ESTADO_CREDITOS:
        Desenhar_Creditos();
        break;
        
    default:
        break;
    }
}

void Transicao_musica(Estados_Jogo estado){
    switch(estado) {
        case ESTADO_MENU:
            TocarMusica(MUSICA_MENU);
            break;

        case ESTADO_FASE_1:
            TocarMusica(MUSICA_FASE_1);
            break;

        case ESTADO_FASE_2:
            TocarMusica(MUSICA_FASE_2);
            break;

        case ESTADO_GAMEOVER:
            TocarMusica(MUSICA_GAMEOVER);
            break;

        case ESTADO_CREDITOS:
            TocarMusica(MUSICA_ENCERRAMENTO);
            break;

        default:
            break;
    }
}
