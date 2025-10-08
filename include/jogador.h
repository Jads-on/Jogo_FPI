#ifndef JOGADOR_H_
#define JOGADOR_H_

#include "raylib.h"

typedef enum Tipo_Bala{ 
    Bala_Padrao = 0,
    Bala_Explosiva = 1,
    Bala_Perfurante = 2
}Tipo_Bala;

typedef struct Efeitos{ //efeitos recebidos pelos consumiveis
    //Energetico
    bool energetico_ativo;
    double energetico_duracao;
}Efeitos;

typedef struct Inventario{ //controle da quantidade de itens consumives que o jogador possui
    int municao_explosiva,
        municao_perfurante,
        energeticos;
    Efeitos efeitos;

} Inventario;

typedef struct Jogador{
    int vida,
        velocidade;
    Vector2 posicao;
    Inventario inventario;
    Tipo_Bala Tipo_Tiro;

}Jogador;

void IniciarJogador(Jogador *jogador, Vector2 PosicaoInicial);

void JogadorUpdate(Jogador *jogador);

void JogadorImagem(Jogador jogador);

void JogadorVidaImagem(Jogador jogador);

void JogadorEnergeticoImagem(Jogador jogador);

void HudHabilidadesImagem(Jogador jogador);

void DescarregarAssets();
#endif
