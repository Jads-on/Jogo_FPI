#ifndef JOGADOR_H_
#define JOGADOR_H_

#include "raylib.h"

typedef enum Tipo_Bala{
    Bala_Padrao = 0,
    Bala_Explosiva = 1,
    Bala_Perfurante = 2
}Tipo_Bala;

typedef struct Efeitos{
    //Energetico
    bool energetico_ativo;
    double energetico_duracao;
}Efeitos;

typedef struct Inventario{
    int municao_explosiva,
        municao_perfurante,
        energeticos,
        escudo;
    Efeitos efeitos;

} Inventario;

typedef struct Jogador{
    int vida,
        velocidade;
    Vector2 posicao;
    Inventario inventario;
    Tipo_Bala Tipo_Tiro;

}Jogador;

void IniciarJogador(Jogador *jogador, Vector2 PosicaoInicial,int vida_padrao, float velocidade_padrao);

void JogadorUpdate(Jogador *jogador);

void JogadorImagem(Jogador jogador);


#endif
