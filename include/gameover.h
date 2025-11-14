// include/gameover.h
#ifndef GAMEOVER_H_
#define GAMEOVER_H_

#include "fases_estados.h"
#include "jogador.h"

void Iniciar_GameOver(void);

void Atualizar_GameOver(Estados_Jogo *estado, Jogador *jogador);

void Desenhar_GameOver(Jogador *jogador);

#endif