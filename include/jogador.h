#ifndef JOGADOR_H_
#define JOGADOR_H_

#include "raylib.h"

typedef enum{ //controle dos tipos de municao
    Bala_Padrao = 0,
    Bala_Explosiva = 1,
    Bala_Perfurante = 2
}Tipo_Bala;

typedef struct{ //efeitos recebidos pelos consumiveis
    //Energetico
    bool energetico_ativo;
    double energetico_duracao;
}Efeitos;

typedef struct{ 
    //status do jogador
    int vida,
        velocidade,
        baterias;
    //animacao 
    int frame_atual_corpo;
    float angulo_mira,
          timer_animacao;
    bool andando;
    Vector2 posicao;
    Tipo_Bala Tipo_Tiro;
    Efeitos efeitos;

}Jogador;

void IniciarJogador(Jogador *jogador, Vector2 PosicaoInicial);

void JogadorUpdate(Jogador *jogador);

void JogadorImagem(Jogador jogador);

void JogadorVidaImagem(Jogador jogador);

void JogadorEnergeticoImagem(Jogador jogador);

void HudHabilidadesImagem(Jogador jogador);

void DescarregarAssets();
#endif
