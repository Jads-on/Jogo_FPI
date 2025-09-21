#include <stdio.h>
#include "tiros.h"
#include "raymath.h"
#define MAX_BALAS 50

static Bala balas[MAX_BALAS];

void Tiro_Jogador(Vector2 Posicao_Jogador, Vector2 direcao_tiro, float angulo_rotacao_tiro, Vector2 alvo){
    int velocidade_balas = 500;
    

    for(int idx = 0; idx < MAX_BALAS; idx++){ //usa limitadas balas 
        if(balas[idx].ativo == false){ //procura uma bala inativa pra reutilizar
                
            balas[idx].posicao = Posicao_Jogador;
            balas[idx].angulo = angulo_rotacao_tiro;
            balas[idx].direcao= Vector2Normalize(direcao_tiro);
            balas[idx].velocidade = velocidade_balas;
            balas[idx].ativo = true;

           break;
        }
    }   
}

void AtualizarTiros(){
    float variacao_tempo = GetFrameTime(); 
    for (int idx = 0; idx < MAX_BALAS; idx++) {
        if (balas[idx].ativo) {
            balas[idx].posicao.x += balas[idx].direcao.x * balas[idx].velocidade * variacao_tempo;
            balas[idx].posicao.y += balas[idx].direcao.y * balas[idx].velocidade * variacao_tempo;

        // Desativa a bala se sair da tela
        if (balas[idx].posicao.x > GetScreenWidth() || balas[idx].posicao.x < 0 ||
            balas[idx].posicao.y > GetScreenHeight() || balas[idx].posicao.y < 0) {
            balas[idx].ativo = false;
            printf("bala sumiu");
        }
        }
    }
}
void Tiro_Imagem_Jogador(){
    for (int i = 0; i < MAX_BALAS; i++) {
        if (balas[i].ativo) {
            DrawCircleV(balas[i].posicao, 5, BLUE);
        }
    } 
}