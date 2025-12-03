#ifndef GESTOR_FASES_H
#define GESTOR_FASES_H
#include "fases_estados.h"
#include "jogador.h"

#define LARGURA_TELA 1920
#define ALTURA_TELA 1080

void Atualizar_Jogo(Estados_Jogo *estado, Jogador *jogador);

void Desenhar_Jogo(Estados_Jogo estado, Jogador jogador);

void Transicao_musica(Estados_Jogo estado);


#endif 