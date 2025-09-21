#ifndef TIROS_H_
#define TIROS_H_

#include "raylib.h"
#include "jogador.h"

typedef struct Bala {
    Vector2 posicao;
    Vector2 direcao;
    float angulo; 
    int velocidade;
    bool ativo;

} Bala;

void Tiro_Jogador(Vector2 Posicao_Jogador, Vector2 direcao_tiro, float angulo_rotacao_tiro, Vector2 alvo);

void AtualizarTiros();

void Tiro_Imagem_Jogador();

#endif