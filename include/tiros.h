#ifndef TIROS_H_
#define TIROS_H_

#include "raylib.h"
#include "jogador.h"

typedef enum Tipo_Bala{
    Bala_Padrao = 0,
    Bala_Explosiva = 1,
    Bala_Perfurante = 2
}Tipo_Bala;

typedef struct Bala {
    Vector2 posicao;
    Vector2 direcao;
    float angulo; 
    int velocidade;
    int dano;
    bool ativo;
    Tipo_Bala tipo;
} Bala;

void IniciarTiros();

void Tiro_Jogador(Vector2 Posicao_Jogador, Vector2 direcao_tiro, float angulo_rotacao_tiro, Vector2 alvo);

void AtualizarTiros();

void Tiro_Imagem_Jogador();

#endif