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

//saida da fase
#define INICIO_SAIDA 900
#define FIM_SAIDA 1000

//timer da intro
#define DURACAO_MENSAGEM_FASE 7.0f        // 5 segundos para o título sumir

extern Estados_Jogo estado_anterior; // extern para ser usado em outras fontes  
      
static float timer_mensagem_fase = 0.0f; 
static Texture2D mapa[QTD_TELAS];      
int idx_area_atual = 0;

void Iniciar_Fase_1(Estados_Jogo *estado){// careega o mapa e os inimigos

    //carrega o mapa
        mapa[0] = LoadTexture("assets/sprites/mapas/Fase_1/area_1.png");
        mapa[1] = LoadTexture("assets/sprites/mapas/Fase_1/area_2.png");
        mapa[2] = LoadTexture("assets/sprites/mapas/Fase_1/area_3.png");
        mapa[3] = LoadTexture("assets/sprites/mapas/Fase_1/area_4.png");
        mapa[4] = LoadTexture("assets/sprites/mapas/Fase_1/area_5.png");


    *estado = ESTADO_INTRO_FASE_1; //ao finalizar segue para a fase 1
}

void Atualizar_Intro_Fase_1(Estados_Jogo *estado){
    if(IsKeyPressed(KEY_ENTER)){
        *estado = ESTADO_FASE_1;
    }
    timer_mensagem_fase = 0.0f;
}

void Atualizar_Fase_1(Estados_Jogo *estado, Jogador *jogador){

    //controle da mensagem inicial da fase
    if (timer_mensagem_fase < DURACAO_MENSAGEM_FASE) {
        timer_mensagem_fase += GetFrameTime();
    }

    // Atualiza o jogador
        JogadorUpdate(jogador);
        AtualizarTiros();
        ColisaoBalaBateria();

        if(jogador->vida <= 0){
            TocarSom(SOM_MORTE_JOGADOR);
            *estado = ESTADO_GAMEOVER;
        }

        //detecta que chegou ao final da fase
        if(idx_area_atual == 4){
            if((jogador->hitbox.x >= INICIO_SAIDA) &&(jogador->hitbox.x <= FIM_SAIDA)){
                *estado = ESTADO_INICIAR_FASE_2;
            }
        }

    //transição entre areas
        if(jogador->posicao.x > LARGURA_TELA){
            if(idx_area_atual < 4){
                switch (idx_area_atual){
                case 0:
                    jogador->posicao.x = 0;
                    jogador->posicao.y = 545;
                    break;

                case 1:
                    jogador->posicao.x = 0;
                    jogador->posicao.y = 380;
                    break;

                case 2:
                    jogador->posicao.x = 0;
                    jogador->posicao.y = 630;
                    break;

                case 3:
                    jogador->posicao.x = 0;
                    jogador->posicao.y = 805;
                    break;
                default:
                    break;
                }
                idx_area_atual++;
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

void Desenhar_Intro_Fase1(){
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), DARKGRAY);
    
    //Menu de instruções
    DrawText("Instruções:", 100, 50, 80, WHITE);
    DrawText(" - As imagens indicam quando é possivel usar aquela habilidade", 150, 330, 40, WHITE);
    DrawText(" - Use Q/R/E para usar as habilidades", 150, 430, 40, WHITE);
    DrawText(" - Se mova com A/D e atire com o cursor", 150, 530, 40, WHITE);
    DrawText("Aperte ENTER para continuar", 1200, 900, 40, WHITE);
    
}

void Desenhar_Mensagem_Fase_1() {
    
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

    // 2. Desenho do Fundo Semi-Transparente
    // Criamos um retângulo semi-transparente para o texto (se desejado)
    DrawRectangle(0, 80, GetScreenWidth(), 180, Fade(BLACK, 0.6f * alpha)); 

    // 3. Desenho do Conteúdo
    DrawText("Fase 1: O Distrito", 
             GetScreenWidth() / 2 - MeasureText("Fase 1: O distrito", 60) / 2, 
             100, 60, cor_texto);

    DrawText("Objetivo: Derrote os robôs e chegue a entrada da base dos Lammers", 
             GetScreenWidth() / 2 - MeasureText("Objetivo: Derrote os robôs e chegue a entrada da base dos Lammers", 45) / 2, 
             170, 45, cor_texto);
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