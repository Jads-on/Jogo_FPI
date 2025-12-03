#ifndef FASE_2_H
#define FASE_2_H

#include "jogador.h"
#include "boss.h"
#include "gestor_fases.h"

void Iniciar_Fase_2(Estados_Jogo *estado);

void Atualizar_Fase_2(Estados_Jogo *estado, Jogador *jogador);

void DesenharFase2(Jogador jogador);

void Descarregar_Fase_2();

void Desenhar_Mensagem_Fase_2();

#endif