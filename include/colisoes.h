#ifndef COLISOES_H_
#define COLISOES_H_

#include "raylib.h"
#include "jogador.h"
#include "tiros.h"
#include "inimigos.h"
#include "baterias.h"

void ColisaoMapa();

void ColisaoBalaBateria(Jogador *jogador);

void Aplicar_Dano_em_Area(Vector2 centro_da_explosao, float raio, int dano);

void ColisaoBalaInimigo();
        
void ColisaoBalaInimigoJogador(Jogador *jogador);

#endif