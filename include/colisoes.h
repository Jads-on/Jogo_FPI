#ifndef COLISOES_H_
#define COLISOES_H_

#include "raylib.h"
#include "jogador.h"
#include "tiros.h"
#include "inimigos.h"
#include "baterias.h"

void ColisaoMapa();

void ColisaoBalaBateria();

int Verificar_Dano_Balas_Alvo(Rectangle hitbox_alvo);

int Verificar_Dano_Explosao_Alvo(Rectangle hitbox_alvo);

#endif