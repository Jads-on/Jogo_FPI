#include "raylib.h"
#include "historia.h"
#include "fases_estados.h"

extern Estados_Jogo estado_anterior; // extern para ser usado em outras fontes  

void Atualizar_Historia(Estados_Jogo *estado) {
    if (IsKeyPressed(KEY_ENTER)) {
        *estado = estado_anterior;
    }
}

void Desenhar_Historia(void) {
    ClearBackground((Color){25, 25, 40, 255});

    DrawText("HISTORIA", 80, 100, 80, WHITE);

    const char *tag = "Metade humano. Metade maquina. So a vinganca e inteira.";
    int tagFontSize = 28;
    int tagWidth = MeasureText(tag, tagFontSize);
    DrawText(tag, (GetScreenWidth() - tagWidth)/2, 220, tagFontSize, (Color){220, 220, 245, 255});

    int x = 120;
    int y = 320;
    int fontSize = 28;
    Color corTexto = RAYWHITE;

    DrawText("Voce e um agente do Esquadrao de Defesa Cibernetica,", x, y, fontSize, corTexto); y += 45;
    DrawText("criado para eliminar androides usados pelo crime.", x, y, fontSize, corTexto); y += 60;
    DrawText("Depois de um acidente, parte do seu corpo foi", x, y, fontSize, corTexto); y += 45;
    DrawText("substituida por metal e circuitos.", x, y, fontSize, corTexto); y += 60;
    DrawText("Agora, metade humano, metade maquina,", x, y, fontSize, corTexto); y += 45;
    DrawText("voce busca vinganca e respostas.", x, y, fontSize, corTexto);

    DrawText("Pressione ENTER voltar ao menu", 100, GetScreenHeight() - 60, 24, (Color){180, 180, 210, 255});
}
