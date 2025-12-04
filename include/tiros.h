#ifndef TIROS_H_
#define TIROS_H_

#include "raylib.h"
#include "jogador.h"

#define MAX_BALAS 30
#define MAX_BALAS_INIMIGOS 50

typedef struct{

    //parametros para os tiros
    Vector2 posicao;
    Vector2 direcao;
    float angulo; 
    int velocidade;
    int dano;
    bool ativo;
    Tipo_Bala tipo;

    //parametros para as colisoes
    Rectangle hitbox;
    float raio_bala;
    bool explodir;
    double tempo_explosao;
} Bala;

extern Bala balas[MAX_BALAS];

void IniciarTiros();

void Tiro_Jogador(Vector2 Posicao_Jogador, Vector2 direcao_tiro, float angulo_rotacao_tiro, int Tipo_Tiro);

void AtualizarTiros();

void Tiro_Imagem_Jogador();

void Desativar_Tiros();

#endif