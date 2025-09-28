#include <stdio.h>
#include "tiros.h"
#include "raymath.h"
#define MAX_BALAS 30
#define Dano_Bala_Padrao 10
#define Dano_Bala_Perfurante 20
#define Dano_Bala_Explosiva 5

static Bala balas[MAX_BALAS];

void IniciarTiros(){
    for(int i = 0; i < MAX_BALAS; i++){
        balas[i].ativo = false;
        balas[i].posicao = (Vector2){0, 0};
        balas[i].tipo = 0;
        balas[i].dano = 0;
    }
}

void Tiro_Jogador(Vector2 Posicao_Jogador, Vector2 direcao_tiro, float angulo_rotacao_tiro, Vector2 alvo){
    int velocidade_balas = 500;
    bool bala_usada = false;
    
    for(int idx = 0; idx < MAX_BALAS; idx++){ //usa limitadas balas 
        
        if((balas[idx].ativo == false) && (bala_usada == false) ){ //procura uma bala inativa pra reutilizar
            
            switch (balas[idx].tipo){
            case Bala_Padrao:
                balas[idx].dano = Dano_Bala_Padrao;
                break;
            
            case Bala_Perfurante:
                balas[idx].dano = Dano_Bala_Perfurante;
                break;
            
            case Bala_Explosiva:
                balas[idx].dano = Dano_Bala_Explosiva;
                break;
            
            default:
                balas[idx].dano = Dano_Bala_Padrao;
                break;
            }

            balas[idx].posicao = Posicao_Jogador;
            balas[idx].angulo = angulo_rotacao_tiro;
            balas[idx].direcao= Vector2Normalize(direcao_tiro);
            balas[idx].velocidade = velocidade_balas;
            balas[idx].ativo = true; //flag que identifique se a bala foi usada ou nao

            bala_usada = true; //flag pra evitar uso de mais bala no mesmo disparo
        }
    }   
}

void AtualizarTiros(){
    float variacao_tempo = GetFrameTime(); 
    for(int idx = 0; idx < MAX_BALAS; idx++){ // procura quais balas estao aivas e atualiza
        if (balas[idx].ativo) {
            balas[idx].posicao.x += balas[idx].direcao.x * balas[idx].velocidade * variacao_tempo;
            balas[idx].posicao.y += balas[idx].direcao.y * balas[idx].velocidade * variacao_tempo;

        // Desativa a bala se sair da tela
        if (balas[idx].posicao.x > GetScreenWidth() || balas[idx].posicao.x < 0 ||
            balas[idx].posicao.y > GetScreenHeight() || balas[idx].posicao.y < 0) {
            balas[idx].ativo = false;
        }
        }
    }
}

void Tiro_Imagem_Jogador(){
    for (int i = 0; i < MAX_BALAS; i++) {
        if (balas[i].ativo) {
            switch (balas[i].tipo){
            case Bala_Padrao:
                DrawCircleV(balas[i].posicao, 5, BLUE);
                break;
            
            case Bala_Perfurante:
                DrawCircleV(balas[i].posicao, 5, YELLOW);
                break;
                
            case Bala_Explosiva:
                DrawCircleV(balas[i].posicao, 5, GREEN);
                break;
            
            default:
                DrawCircleV(balas[i].posicao, 5, BLUE);
                break;
            }
        }
    } 
}