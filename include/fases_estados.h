#ifndef FASES_ESTADOS_H
#define FASES_ESTADOS_H

//estados da maquina de estados que controla as fases e o menu
typedef enum{
    ESTADO_MENU = 0,
    ESTADO_FASE_1 = 1,
    ESTADO_FASE_2 = 2,
    ESTADO_GAMEOVER = 3,
    ESTADO_SAIR = 4,
    ESTADO_CREDITOS = 5,
    ESTADO_VOLUME = 6,
    ESTADO_HISTORIA = 7
    
}Estados_Jogo;

#endif