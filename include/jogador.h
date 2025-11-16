#ifndef JOGADOR_H_
#define JOGADOR_H_

#include "raylib.h"
#include "baterias.h"

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
    int frame_atual_corpo,
        qtd_frames,
        direcao;
    float angulo_mira,
          timer_animacao;
    bool andando,
         atirando,
         pulando,
         morto,
         tem_chao;
}Animacoes;

typedef struct{ 
    //status do jogador
    int vida,
        velocidade,
        baterias,
        total_baterias;
    //fisica
    float deslocamento_vertical;
    Vector2 posicao;
    Rectangle hitbox;
    Animacoes animacoes;
    Tipo_Bala Tipo_Tiro;
    Efeitos efeitos;

}Jogador;

// Funcoes do jogador

void IniciarJogador(Jogador *jogador, Vector2 PosicaoInicial);

void JogadorUpdate(Jogador *jogador);

void JogadorImagem(Jogador jogador);

void JogadorVidaImagem(Jogador jogador);

void JogadorEnergeticoImagem(Jogador jogador);

void HudHabilidadesImagem(Jogador jogador);

bool CheckCollisionJogador(Rectangle hitbox, Rectangle elemento);

void DescarregarAssets();

#endif
