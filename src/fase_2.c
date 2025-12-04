#include "raylib.h"
#include "jogador.h"
#include "tiros.h"
#include "colisoes.h"
#include "fases_estados.h"
#include "gestor_fases.h"
#include "gestor_audio.h"
#include "boss.h"
#include "fase_2.h"

#define ALTURA_CHAO 370
#define LIMITE_MAPA_DIREITA 1270
#define LIMITE_MAPA_ESQUERDA 250

extern Estados_Jogo estado_anterior; // extern para ser usado em outras fontes 

//timer da intro
#define DURACAO_MENSAGEM_FASE 5.0f     
static float timer_mensagem_fase = 0.0f;

Boss juggernaut;
Texture2D mapa;

void Iniciar_Fase_2(Estados_Jogo *estado){
    Vector2 posicao_inicial_boss = (Vector2){LARGURA_TELA/2, ALTURA_CHAO};

    Iniciar_Boss(&juggernaut, posicao_inicial_boss);

    mapa = LoadTexture("assets/sprites/mapas/Fase_2/base_dos_lammers.png");
    timer_mensagem_fase = 0.0f;
    *estado = ESTADO_FASE_2;
}

void Atualizar_Fase_2(Estados_Jogo *estado, Jogador *jogador){

    if (timer_mensagem_fase < DURACAO_MENSAGEM_FASE) {
        timer_mensagem_fase += GetFrameTime();
    }

    JogadorUpdate(jogador);
    Atualizar_Boss(&juggernaut, jogador->posicao, 1.0);
    AtualizarTiros();
    ColisaoBalaBateria(jogador);

    if(jogador->vida <= 0){
        TocarSom(SOM_MORTE_JOGADOR);
        estado_anterior = ESTADO_INICIAR_FASE_2;
        *estado = ESTADO_GAMEOVER;
    }

     if(jogador->posicao.x < 0){
        jogador->posicao.x = 0;
    }
    if((jogador->posicao.x > LARGURA_TELA - 100  && !TodosInimigosMortos())){
        jogador->posicao.x = LARGURA_TELA - 100;
    }

    //edicao do volume no meio do jogo (substitui um pause)
    if(IsKeyPressed(KEY_V)){  
        estado_anterior = ESTADO_FASE_1; //salva o estado anterior
        *estado = ESTADO_VOLUME;
        TocarSom(SOM_MENU_SELECT);
    }
}

// ============================= DESENHO ======================================

void Desenhar_Mensagem_Fase_2(){
    if (timer_mensagem_fase >= DURACAO_MENSAGEM_FASE) {
        return; // Sai da função se o tempo acabou
    }

    //Fade Out (Efeito de esvanecer)
    float progresso = timer_mensagem_fase / DURACAO_MENSAGEM_FASE; // Vai de 0.0 a 1.0
    float alpha = 1.0f; //Opacidade total

    if (progresso > 0.7f) {
        alpha = 1.0f - ((progresso - 0.7f) / 0.3f);
    }

    // Cor do texto
    Color cor_texto = Fade(WHITE, alpha); 

    DrawRectangle(0, 80, GetScreenWidth(), 180, Fade(BLACK, 0.6f * alpha)); 

    DrawText("Fase 2: Base Lammer", 
             GetScreenWidth() / 2 - MeasureText("Fase 2: Base Lammer", 60) / 2, 
             100, 60, cor_texto);

    DrawText("Objetivo: Destrua o autômato Juggernaut", 
             GetScreenWidth() / 2 - MeasureText("Objetivo: Destrua o autômato Juggernaut", 45) / 2, 
             170, 45, cor_texto);
}

void DesenharFase2(Jogador jogador){
    DrawTexture(mapa, 0, 0, WHITE);

    // Desenho do Jogador
    JogadorImagem(jogador);
    Boss_Imagem(juggernaut);

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