#include "raylib.h"
#include "jogador.h"
#include "tiros.h"
#include "inimigos.h"
#include "drone.h"
#include "gameover.h"

static const char *itens[] = {"Reiniciar", "Menu Principal", "Sair"};
static const int TOTAL_OPCOES = 3;
static int opcao = 0;

static int frame_atual_morte = 0;
static float timer_animacao_morte = 0.0f;
static float fade = 0.0f;

//importa os sprites da pose de derrota 
extern Texture2D sprite_jogador_corpo;
extern Rectangle jogador_morto[2];

void Iniciar_GameOver(void) {
    // Reseta variáveis quando entra na tela
    frame_atual_morte = 0;
    timer_animacao_morte = 0.0f;
    fade = 0.0f;
    opcao = 0;
}

void Atualizar_GameOver(Estados_Jogo *estado, Jogador *jogador) {
    // Transição fade in (esvanecer)
    if(fade < 1.0f) {
        fade += GetFrameTime() * 2.0f;
        if(fade > 1.0f) fade = 1.0f;
    }
    
    // Animação de morte
    timer_animacao_morte += GetFrameTime();
    if(timer_animacao_morte >= 0.4f) {
        timer_animacao_morte = 0.0f;
        frame_atual_morte = (frame_atual_morte + 1) % 2;
    }
    
    // Navegação no menu
    if(IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
        opcao = (opcao + TOTAL_OPCOES - 1) % TOTAL_OPCOES;
    
    if(IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
        opcao = (opcao + 1) % TOTAL_OPCOES;
    
    // Seleção
    if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        switch(opcao) {
            case 0: // Reiniciar fase
               
                IniciarJogador(jogador, (Vector2){100, 600});
                IniciarTiros();
                IniciarBaterias();
                IniciarInimigos();
                
            
                *estado = ESTADO_INTRO_FASE_1;
                break;
            case 1: // Menu
        
                IniciarJogador(jogador, (Vector2){100, 600});
                IniciarTiros();
                *estado = ESTADO_MENU;
                break;
            case 2: // Sair
                *estado = ESTADO_SAIR;
                break;
        }
    }
}

void Desenhar_GameOver(Jogador *jogador) {
 
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.85f * fade));
    
    const char* texto_principal = "- FATAL ERROR -";
    int tamanho_fonte = 70,
        altura_texto = MeasureText(texto_principal, tamanho_fonte);
    Color cor_texto = ((int)(GetTime() * 4) % 2 == 0) ? RED : (Color){200, 50, 50, 255};
    
    // Sombra
    DrawText(texto_principal, 
             GetScreenWidth()/2 - altura_texto/2 + 3, 
             103, 
             tamanho_fonte, 
             Fade(BLACK, 0.6f * fade));
    
    // Texto principal
    DrawText(texto_principal, 
             GetScreenWidth()/2 - altura_texto/2, 
             100, 
             tamanho_fonte, 
             Fade(cor_texto, fade));
    
    // Animacao da pose de derrota
    Vector2 pos_morte = {
        GetScreenWidth() / 2.0f - 200,
        200
    };
    DrawTextureRec(sprite_jogador_corpo, jogador_morto[frame_atual_morte], pos_morte, Fade(WHITE, fade));
    
    // estatisticas
   
    DrawText(TextFormat("Baterias Coletadas: %d", jogador->total_baterias), 
             GetScreenWidth()/2 - 180, 510, 28, 
             Fade(SKYBLUE, fade));
    
    // Menu
    int fs = 42;
    int y0 = 680;
    int dy = 70;
    
    for(int i = 0; i < TOTAL_OPCOES; i++) {
        const char *txt = itens[i];
        int w = MeasureText(txt, fs);
        int x = (GetScreenWidth() - w)/2;
        int y = y0 + i*dy;
        
        // Indicador de seleção
        if(i == opcao) {
            DrawRectangleLines(x - 24, y - 6, w + 48, fs + 12, Fade(YELLOW, fade));
            DrawTriangle(
                (Vector2){x - 36, y + fs/2.0f},
                (Vector2){x - 24, y + fs/2.0f - 10},
                (Vector2){x - 24, y + fs/2.0f + 10},
                Fade(YELLOW, fade)
            );
        }
        
        // Texto da opção
        Color cor_opcao = (i == opcao) ? YELLOW : RAYWHITE;
        DrawText(txt, x, y, fs, Fade(cor_opcao, fade));
    }
    
    // Instruções
    DrawText("Use W/S (ou setas) e ENTER", 40, GetScreenHeight() - 60, 24, Fade(GRAY, fade));
}