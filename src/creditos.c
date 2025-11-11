#include "raylib.h"
#include "creditos.h"
#include "fases_estados.h"

void Atualizar_Creditos(Estados_Jogo *estado) {
    // Volta ao menu
    if (IsKeyPressed(KEY_ENTER)) {
        *estado = estado_anterior;
    }
}

void Desenhar_Creditos(void) {
    // fundo escuro azulado
    ClearBackground((Color){25, 25, 40, 255});

    // título
    DrawText("CRÉDITOS", 80, 100, 80, WHITE);

    int y = 280;
    DrawText("Equipe:", 100, y, 40, LIGHTGRAY); 
    y += 60;

    DrawText("- Cairo Henrique Silva Thomaz de Aquino <chsta>", 120, y, 36, GRAY); y += 45;
    DrawText("- Jadson José da Silva Lins <jjsl2>", 120, y, 36, GRAY); y += 45;
    DrawText("- João Gustavo Guimarães Pires <jggp>", 120, y, 36, GRAY); y += 45;
    DrawText("- João Henrique dos Santos Silva <jhss2>", 120, y, 36, GRAY); y += 45;
    DrawText("- Rodrigo Pereira Ferreira <rpf2>", 120, y, 36, GRAY); 
    y += 120;

    DrawText("Professor:", 100, y, 40, LIGHTGRAY); 
    y += 60;
    DrawText("- Alexandre Cabral Mota <acm>", 120, y, 36, GRAY);
    y += 100;

    DrawText("Pressione ENTER para voltar ao menu", 100, y, 26, (Color){180, 180, 210, 255});
}
