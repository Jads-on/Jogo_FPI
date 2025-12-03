#include "raylib.h"
#include "jogador.h"
#include "tiros.h"
#include "colisoes.h"
#include "fases_estados.h"
#include "gestor_fases.h"
#include "gestor_audio.h"
#include "drone.h"
#include "fase_1.h"

#define QTD_TELAS 5
#define ALTURA_CHAO 850.0

//saida da fase
#define INICIO_SAIDA 900
#define FIM_SAIDA 1000

//timer da intro
#define DURACAO_MENSAGEM_FASE 7.0f        

extern Estados_Jogo estado_anterior; // extern para ser usado em outras fontes  

static bool drone_spawnou[QTD_TELAS] = { false, false, false, false, false };
static bool tocou_som_vida_baixa = false;
static float timer_mensagem_fase = 0.0f,
             fade = 0.0f; ; 
static Texture2D mapa[QTD_TELAS],
                 tela_encerramento;      
int idx_area_atual = 0;

void Iniciar_Fase_1(Estados_Jogo *estado){// careega o mapa e os inimigos
    //carrega o mapa
    mapa[0] = LoadTexture("assets/sprites/mapas/Fase_1/area_1.png");
    mapa[1] = LoadTexture("assets/sprites/mapas/Fase_1/area_2.png");
    mapa[2] = LoadTexture("assets/sprites/mapas/Fase_1/area_3.png");
    mapa[3] = LoadTexture("assets/sprites/mapas/Fase_1/area_4.png");
    mapa[4] = LoadTexture("assets/sprites/mapas/Fase_1/area_5.png");
    tela_encerramento = LoadTexture("assets/sprites/mapas/Fase_1/fase1_concluida.png");
    fade = 0.0f;

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

    // Atualiza o jogo
    float delta = GetFrameTime();

    JogadorUpdate(jogador);
    AtualizarTiros();

    // --- coisas novas da main ---
    AtualizarBalasInimigos(delta, LARGURA_TELA, ALTURA_TELA);
    AtualizarInimigos(delta, jogador->posicao);
    ColisaoBalaBateria(jogador);          
    ColisaoBalaInimigo();
    ColisaoBalaInimigoJogador(jogador);

    // --- drone ---
    AtualizarDrone(jogador);

    if(jogador->vida <= 20 && !tocou_som_vida_baixa){ //alerta sonoro de vida baixa
        TocarSom(SOM_VIDA_BAIXA);
        tocou_som_vida_baixa = true;

    }
    else if(tocou_som_vida_baixa && jogador->vida > 20){ //reseta o alerta
        tocou_som_vida_baixa = false;
    }
    
    if(jogador->vida <= 0){
        TocarSom(SOM_MORTE_JOGADOR);
        idx_area_atual = 0;

        //resreta a flag dos drones
        for(int idx = 0; idx < QTD_TELAS; idx++){
            drone_spawnou[idx] = false;
        }

        Desativar_Baterias();
        Desativar_Inimigos();
        Desativar_Drone();
        *estado = ESTADO_GAMEOVER;
    }
    
    //detecta que chegou ao final da fase
    if(idx_area_atual == 4){
        if((jogador->hitbox.x >= INICIO_SAIDA) &&(jogador->hitbox.x <= FIM_SAIDA)){
            *estado = ESTADO_INICIAR_FASE_2;
        }
    }

    //transição entre areas
    if(TodosInimigosMortos()){
        if(jogador->posicao.x > LARGURA_TELA){
            Desativar_Baterias(); //desativa baterias não coletadas
            idx_area_atual++;
            
            CarregarInimigosDaTela(idx_area_atual);

            if(idx_area_atual < QTD_TELAS){
                switch (idx_area_atual){
                case 1:
                    jogador->posicao.x = 0;
                    jogador->posicao.y = 545;
                    break;

                case 2:
                    jogador->posicao.x = 0;
                    jogador->posicao.y = 380;
                    break;

                case 3:
                    jogador->posicao.x = 0;
                    jogador->posicao.y = 630;
                    break;

                case 4:
                    jogador->posicao.x = 0;
                    jogador->posicao.y = 805;
                    break;

                default:
                    break;
                }

               

                // --- drone: spawn por sala (sala 2 e 3 = idx 1 e 2) ---
                if ((idx_area_atual == 2 || idx_area_atual == 3) && !drone_spawnou[idx_area_atual]){
                    drone_ativo = true;
                    IniciarDrone();
                    drone_spawnou[idx_area_atual] = true;
                }

                // se saiu das salas do drone, garante que fica desligado
                if (idx_area_atual != 2 && idx_area_atual != 3) {
                    drone_ativo = false;
                }
            }
        }
    }

    //evita sair do limite do mapa
    if(jogador->posicao.x < 0){
        jogador->posicao.x = 0;
    }
    if((jogador->posicao.x > LARGURA_TELA - 100  && !TodosInimigosMortos()) || (idx_area_atual > 4)){
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

void Desenhar_Intro_Fase1(){
    DrawTexture(mapa[4], 0, 0, WHITE);

    //Menu de instruções
    DrawText("Instruções:", 100, 50, 80, WHITE);
    DrawText(" - As imagens indicam quando é possivel usar aquela habilidade", 150, 330, 40, WHITE);
    DrawText(" - Use Q/R/E para usar as habilidades", 150, 430, 40, WHITE);
    DrawText(" - Se mova com A/D e atire com o cursor", 150, 530, 40, WHITE);
    DrawText("Aperte ENTER para continuar", 1200, 900, 40, WHITE);
}

void Desenhar_Encerramento_Fase_1(){
    
    // Aumenta a opacidade aos poucos até chegar em 1.0 (100% visível)
    if (fade < 1.0f) {
        fade += GetFrameTime(); 
    }

    DrawTexture(tela_encerramento ,0 ,0 ,WHITE);

    // --- TEXTO PRINCIPAL ---
    const char* texto_principal = "- MISSÃO CONCLUIDA -";
    int tamanho_fonte = 70;
    
    // CORREÇÃO: MeasureText retorna a LARGURA (width), não a altura.
    int largura_texto = MeasureText(texto_principal, tamanho_fonte);
    
    // Efeito de piscar (mantido)
    Color cor_texto = ((int)(GetTime() * 4) % 2 == 0) ? DARKGREEN : GREEN;
    
    // Sombra (Desenhada primeiro)
    DrawText(texto_principal, 
             GetScreenWidth()/2 - largura_texto/2 + 5, // Pequeno deslocamento para a sombra
             105, 
             tamanho_fonte, 
             Fade(BLACK, 0.5f * fade)); // Sombra sempre um pouco mais transparente
    
    // Texto Principal
    DrawText(texto_principal, 
             GetScreenWidth()/2 - largura_texto/2, // Centraliza no eixo X
             100, 
             tamanho_fonte, 
             Fade(cor_texto, fade));
            
    // --- TEXTO DE INSTRUÇÃO ---
    const char* texto_sub = "Pressione ENTER para iniciar a Fase 2";
    int tamanho_sub = 40;
    int largura_sub = MeasureText(texto_sub, tamanho_sub);

    DrawText(texto_sub, 
             GetScreenWidth()/2 - largura_sub/2, // Centralização calculada
             510, 
             tamanho_sub, 
             Fade(WHITE, fade));
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

    DrawRectangle(0, 80, GetScreenWidth(), 180, Fade(BLACK, 0.6f * alpha)); 

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

    // Inimigos
    DesenharInimigos();

    // Drone (só aparece se ativo)
    DesenharDrone();

    //Assets
    DesenharBaterias();

    JogadorEnergeticoImagem(jogador);
    JogadorVidaImagem(jogador);
    HudHabilidadesImagem(jogador);

    //Tiros
    Tiro_Imagem_Jogador();
    DesenharBalasInimigos();
}

void Descarregar_Fase_1(){

    DescarregarTexturasInimigos();
    Descarregar_Drone();
    Desativar_Baterias();
    Desativar_Inimigos();
    Desativar_Drone();
    
    for(int i = 0; i < QTD_TELAS; i++){
        UnloadTexture(mapa[i]);
    }

}