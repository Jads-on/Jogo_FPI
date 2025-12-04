// src/menu.c
#include "raylib.h"
#include "menu.h"
#include "fases_estados.h"
#include "jogador.h"   // se o protótipo usa Jogador*
#include "gestor_audio.h"

// ---------- estado interno do menu ----------
static const char *itens[] = {"Jogar", "História", "Volume", "Créditos", "Sair"};
static const int TOTAL_OPCOES = 5;
static int opcao = 0;

static Texture2D fundo;
static bool carregado = false;

extern Estados_Jogo estado_anterior; // extern para ser usado em outras fontes    

// ---------- ciclo de vida ----------
void Iniciar_Menu(void) {
    if (carregado) return;
    fundo = LoadTexture("assets/sprites/menu/menu_1920x1080.png"); // <-- seu caminho
    carregado = true;
    
}

void Encerrar_Menu(void) {
    if (!carregado) return;
    UnloadTexture(fundo);
    carregado = false;
}

// ---------- lógica ----------
void Atualizar_Menu(Estados_Jogo *estado, Jogador *jogador) {
    (void)jogador; // não usado por enquanto

    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)){
        opcao = (opcao + TOTAL_OPCOES - 1) % TOTAL_OPCOES;
        TocarSom(SOM_SELECIONAR);
    }

    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)){
        opcao = (opcao + 1) % TOTAL_OPCOES;
        TocarSom(SOM_SELECIONAR);
    }

    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        estado_anterior = ESTADO_MENU; //salva o estado anterior
        switch (opcao) {
            case 0: *estado = ESTADO_INICIAR_FASE_2;   break; // Jogar
            case 1: *estado = ESTADO_HISTORIA; break; // História
            case 2: *estado = ESTADO_VOLUME;   break; //Volume
            case 3: *estado = ESTADO_CREDITOS; break; // Créditos
            case 4: *estado = ESTADO_SAIR;     break; // Sair
        }
        TocarSom(SOM_MENU_SELECT);
    }
}

// ---------- desenho ----------
void Desenhar_Menu(void) {
    // desenha o papel de parede ocupando a tela
    DrawTexturePro(
        fundo,
        (Rectangle){0, 0, (float)fundo.width, (float)fundo.height},
        (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
        (Vector2){0, 0},
        0.0f,
        WHITE
    );

    // lista de opções
    int fs = 42;
    int y0 = 300;    // y inicial
    int dy = 70;     // espaçamento vertical
    for (int i = 0; i < TOTAL_OPCOES; i++) {
        const char *txt = itens[i];
        int w = MeasureText(txt, fs);
        int x = (GetScreenWidth() - w)/2;
        int y = y0 + i*dy;

        // indicador de seleção
        if (i == opcao) {
            DrawRectangleLines(x - 24, y - 6, w + 48, fs + 12, YELLOW);
            DrawTriangle(
                (Vector2){x - 36, y + fs/2.0f},
                (Vector2){x - 24, y + fs/2.0f - 10},
                (Vector2){x - 24, y + fs/2.0f + 10},
                YELLOW
            );
        }
        DrawText(txt, x, y, fs, (i == opcao) ? YELLOW : RAYWHITE);
    }

    DrawText("Use W/S (ou setas) e ENTER", 40, GetScreenHeight() - 100, 24, GRAY);
}
