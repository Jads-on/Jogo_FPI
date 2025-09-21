#ifndef JOGADOR_H_
#define JOGADOR_H_

#include "raylib.h"

typedef struct Inventario{
    int municao_explosiva,
        municao_perfurante,
        energetico;

} Inventario;

typedef struct Jogador{
    int vida,
        velocidade,
        escudo,
        energia;
    Vector2 posicao;
    Inventario inventario;

}Jogador;

void IniciarJogador(Jogador *jogador, Vector2 PosicaoInicial,int vida_padrao, float velocidade_padrao);

void JogadorUpdate(Jogador *jogador);

void JogadorImagem(Jogador jogador);


#endif
