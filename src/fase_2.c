#include "raylib.h"
#include "jogador.h"
#include "tiros.h"
#include "colisoes.h"
#include "fases_estados.h"
#include "gestor_fases.h"
#include "gestor_audio.h"
#include "boss.h"

#define ALTURA_CHAO 800

extern Estados_Jogo estado_anterior; // extern para ser usado em outras fontes 

//Variavies do tamanho da tela (torna o tamanho padrao)
int Largura_Tela, Altura_Tela;

Boss juggernaut;
Texture2D mapa;

void Iniciar_Fase_2(Estados_Jogo *estado){
    Largura_Tela = GetScreenWidth();
    Altura_Tela = GetScreenHeight();

    Vector2 posicao_inicial_boss = (Vector2){ALTURA_CHAO, Altura_Tela/2};

    Iniciar_Boss(&juggernaut, posicao_inicial_boss);

    mapa = LoadTexture("assets/sprites/mapas/Fase_2/base_dos_lammers.png");
    *estado = ESTADO_FASE_2;
}

void Atualizar_Fase_2(Estados_Jogo *estado, Jogador *jogador, Boss boss){

    JogadorUpdate(jogador);
    Atualizar_Boss(&boss, jogador->posicao, 1.0);
    AtualizarTiros();
    ColisaoBalaBateria(jogador);

    if(jogador->vida <= 0){
        TocarSom(SOM_MORTE_JOGADOR);
        *estado = ESTADO_GAMEOVER;
    }

     if(jogador->posicao.x < 0){
        jogador->posicao.x = 0;
    }
    if((jogador->posicao.x > Largura_Tela - 100  && !TodosInimigosMortos())){
        jogador->posicao.x = Largura_Tela - 100;
    }

    //edicao do volume no meio do jogo (substitui um pause)
    if(IsKeyPressed(KEY_V)){  
        estado_anterior = ESTADO_FASE_1; //salva o estado anterior
        *estado = ESTADO_VOLUME;
        TocarSom(SOM_MENU_SELECT);
    }
    


}

void DesenharFase2(Jogador jogador, Boss boss){
    DrawTexture(mapa, 0, 0, WHITE);

    // Desenho do Jogador
    JogadorImagem(jogador);
    Boss_Imagem(boss);

    //Assets
    DesenharBaterias();
    JogadorEnergeticoImagem(jogador);
    JogadorVidaImagem(jogador);
    HudHabilidadesImagem(jogador);

    //Tiros
    Tiro_Imagem_Jogador();
}

void Descarregar_Fase_2(){

    UnloadTexture(mapa);
}