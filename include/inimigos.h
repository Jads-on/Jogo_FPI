#ifndef INIMIGOS_H
#define INIMIGOS_H

#include "raylib.h"
#include "tiros.h" 

//Constantes 
#define MAX_INIMIGOS 10
#define MAX_BALAS_INIMIGOS 50
#define FRAMES_INIMIGO 4
#define VELOCIDADE_FRAME 0.1f

//Estruturas 
typedef struct {
    Vector2 pos;
    Rectangle hitbox;
    float vida;
    bool ativo;
    float tempoDisparo;
    int frameAtual;
    float tempoFrame;
} Inimigo;


extern Inimigo spiderlith[MAX_INIMIGOS];
extern Bala balasInimigos[MAX_BALAS_INIMIGOS]; 
extern bool drone_ativo;

void DescarregarTexturasInimigos(void);

void IniciarInimigos(void);
void CarregarInimigosDaTela(int indiceTela);
void SpawnarInimigoEm(Vector2 posicao);

void AtualizarInimigos(float delta, Vector2 posicaoJogador);

void AtualizarBalasInimigos(float delta, int larguraTela, int alturaTela); 

void DesenharInimigos(void);
void DesenharBalasInimigos(void);

void IniciarDrone();
void AtualizarDrone(Jogador *jogador);
void DesenharDrone();

bool TodosInimigosMortos();

#endif