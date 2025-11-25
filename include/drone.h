#ifndef DRONE_H_
#define DRONE_H_

#include "raylib.h"
#include "jogador.h"
#include "tiros.h"

extern bool drone_ativo;

void IniciarDrone();
void AtualizarDrone(Jogador *jogador);
void DesenharDrone();

#endif
