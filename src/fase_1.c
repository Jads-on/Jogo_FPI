#include "jogador.h"
#include "tiros.h"
#include "fases_estados.h"
#include "gestor_fases.h"

void Atualizar_Fase_1(Estados_Jogo *estado, Jogador *jogador){
        //atualiza o jogador
        JogadorUpdate(jogador);
        AtualizarTiros();

        if(jogador->vida <= 0){
            *estado = ESTADO_GAMEOVER;
        }
}

void DesenharFase1(Jogador jogador){

    //desenho do Jogador
    JogadorImagem(jogador);

    //barras de vida e habilidades
    JogadorEnergeticoImagem(jogador);
    JogadorVidaImagem(jogador);
    Tiro_Imagem_Jogador();
    HudHabilidadesImagem(jogador);

}