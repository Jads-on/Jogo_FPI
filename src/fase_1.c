#include "jogador.h"
#include "tiros.h"
#include "fases_estados.h"
#include "gestor_fases.h"

void Atualizar_Fase_1(Estados_Jogo *estado, Jogador *jogador){
    // Atualiza o jogador
        JogadorUpdate(jogador);
        AtualizarTiros();

        if(jogador->vida <= 0){
            *estado = ESTADO_GAMEOVER;
        }
}

void DesenharFase1(Jogador jogador){

    // Desenho do Jogador
        JogadorImagem(jogador);

    //Assets
        // Baterias Dropadas
            DesenharBaterias();
    
        // Barras e Hud
            JogadorEnergeticoImagem(jogador);
            JogadorVidaImagem(jogador);
            HudHabilidadesImagem(jogador);
        
        //Tiros
            Tiro_Imagem_Jogador();
}