#ifndef BATERIAS_H_
#define BATERIAS_H_

#include "raylib.h"

#define MAX_BATERIAS 50 //qtd de entidades maximas dropadas
#define TAM_BATERIA 50

typedef struct{
    int valor;
    bool ativo;
    Vector2 posicao;
    Rectangle hitbox;

}Bateria;

extern Bateria baterias_dropadas[MAX_BATERIAS];

void IniciarBaterias();

void SpawnarBaterias(Vector2 posicao, int valor);

bool VerificarColisaoBateria(Rectangle hitbox_jogador, int *valor_coletado);

void DesenharBaterias();

void DescarregarBateria();

#endif