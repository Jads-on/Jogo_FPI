// include/menu.h
#ifndef MENU_H_
#define MENU_H_

#include "fases_estados.h"
#include "jogador.h"

void Iniciar_Menu(void);
void Encerrar_Menu(void);
void Atualizar_Menu(Estados_Jogo *estado, Jogador *jogador);
void Desenhar_Menu(void);

#endif
