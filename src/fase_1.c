#include "jogador.h"
#include "tiros.h"
#include "fases_estados.h"
#include "gestor_fases.h"

extern Estados_Jogo estado_anterior; // extern para ser usado em outras fontes    

void Atualizar_Fase_1(Estados_Jogo *estado, Jogador *jogador){
    // Atualiza o jogador
        JogadorUpdate(jogador);
        AtualizarTiros();

        if(jogador->vida <= 0){
            *estado = ESTADO_GAMEOVER;
        }

        if(IsKeyPressed(KEY_V)){  
            estado_anterior = ESTADO_FASE_1; //salva o estado anterior
            *estado = ESTADO_VOLUME;
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