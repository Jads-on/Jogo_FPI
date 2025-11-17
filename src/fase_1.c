#include "raylib.h"
#include "jogador.h"
#include "tiros.h"
#include "colisoes.h"
#include "fases_estados.h"
#include "gestor_fases.h"
#include "gestor_audio.h"

#define QTD_TELAS 5
#define LARGURA_TELA 1920
#define ALTURA_TELA 1080

extern Estados_Jogo estado_anterior; // extern para ser usado em outras fontes  

static Texture2D mapa[QTD_TELAS];

Texture2D textura_Tela_Atual;
Rectangle chao_Atual;        
int idx_area_atual = 0;

void Iniciar_Fase_1(Estados_Jogo *estado){// careega o mapa e os inimigos

    //carrega o mapa
    mapa[0] = LoadTexture("assets/sprites/mapas/Fase_1/area_1.png");
    mapa[1] = LoadTexture("assets/sprites/mapas/Fase_1/area_2.png");
    mapa[2] = LoadTexture("assets/sprites/mapas/Fase_1/area_3.png");
    mapa[3] = LoadTexture("assets/sprites/mapas/Fase_1/area_4.png");
    mapa[4] = LoadTexture("assets/sprites/mapas/Fase_1/area_5.png");


    *estado = ESTADO_FASE_1; //ao finalizar segue para a fase 1
}

void Atualizar_Fase_1(Estados_Jogo *estado, Jogador *jogador){
    // Atualiza o jogador
        JogadorUpdate(jogador);
        AtualizarTiros();
        ColisaoBalaBateria();

        if(jogador->vida <= 0){
            TocarSom(SOM_MORTE_JOGADOR);
            *estado = ESTADO_GAMEOVER;
        }

    //transição entre areas
        if(jogador->posicao.x > LARGURA_TELA){
            if(idx_area_atual < 4){
                idx_area_atual++;
                jogador->posicao.x = 0;
            }
        }

    //evita sair do limite do mapa
        if(jogador->posicao.x < 0){
            jogador->posicao.x = 0;
        }
        if(jogador->posicao.x > LARGURA_TELA - 180 && (idx_area_atual == 4)){
            jogador->posicao.x = LARGURA_TELA - 180;
        }

    //edicao do volume no meio do jogo (substitui um pause)
        if(IsKeyPressed(KEY_V)){  
            estado_anterior = ESTADO_FASE_1; //salva o estado anterior
            *estado = ESTADO_VOLUME;
            TocarSom(SOM_MENU_SELECT);
        }
}

void DesenharFase1(Jogador jogador){
        DrawTexture(mapa[idx_area_atual], 0, 0, WHITE);
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

void Descarregar_Fase_1(){

    UnloadTexture(mapa[0]);
 }