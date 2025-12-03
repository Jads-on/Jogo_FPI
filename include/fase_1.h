#ifndef FASE_1_H
#define FASE_1_H

#include "jogador.h"
#include "gestor_fases.h"

void Iniciar_Fase_1(Estados_Jogo *estado);

void Atualizar_Intro_Fase_1(Estados_Jogo *estado);

void Atualizar_Fase_1(Estados_Jogo *estado, Jogador *jogador);

void Desenhar_Intro_Fase1();

void Desenhar_Mensagem_Fase_1();

void DesenharFase1(Jogador Jogador);

void Desenhar_Encerramento_Fase_1();

void Descarregar_Fase_1();

#endif