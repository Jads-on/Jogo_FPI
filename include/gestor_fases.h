#ifndef GESTOR_FASES_H
#define GESTOR_FASES_H
#include "fases_estados.h"
#include "jogador.h"

void Atualizar_Jogo(Estados_Jogo *estado, Jogador *jogador);
void Desenhar_Jogo(Estados_Jogo estado, Jogador jogador);

#endif 