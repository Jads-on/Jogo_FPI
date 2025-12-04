#ifndef FASES_ESTADOS_H
#define FASES_ESTADOS_H

//estados da maquina de estados que controla as fases e o menu
typedef enum{
    ESTADO_MENU,
    ESTADO_INICIAR_FASE_1,
    ESTADO_INTRO_FASE_1,
    ESTADO_FASE_1,
    ESTADO_ENCERRAR_FASE_1,
    ESTADO_INICIAR_FASE_2,
    ESTADO_INTRO_FASE2,
    ESTADO_FASE_2,
    ESTADO_GAMEOVER,
    ESTADO_SAIR,
    ESTADO_CREDITOS,
    ESTADO_VOLUME,
    ESTADO_HISTORIA,
    
}Estados_Jogo;

#endif