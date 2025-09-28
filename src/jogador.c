#include "jogador.h"
#include "raymath.h"
#include "tiros.h"
#include <stdio.h>

Bala bala;

void IniciarJogador(Jogador *jogador, Vector2 PosicaoInicial, int vida_padrao, float velocidade_padrao){
    
    //inicia as variaveis pra evitar bugs com lixo de memoria
    jogador->posicao.x = PosicaoInicial.x;
    jogador->posicao.y = PosicaoInicial.y;
    jogador->velocidade = velocidade_padrao;
    jogador->vida = vida_padrao;
    jogador->energia = 0;
    jogador->escudo = 0;
    jogador->inventario.energetico = 0;
    jogador->inventario.municao_explosiva = 0;
    jogador->inventario.municao_perfurante = 0;    

}

void JogadorImagem(Jogador jogador){

    //aqui ficara as sprites do protagonista
    DrawRectangle(jogador.posicao.x, jogador.posicao.y, 200, 200, RED);
}

void JogadorUpdate(Jogador *jogador){
    
//movimentacao modelo: WASD)
    float variacao_tempo = GetFrameTime(); //captura a taxa de fps atual (evita limitar a 60)
    Vector2 mover = {0.0f, 0.0f};
    
    //no eixo x 
    mover.x = (int)IsKeyDown(KEY_D) - (int)IsKeyDown(KEY_A); // fonte (clear code (raylib) 1:05:00)
    
    //no eixo y
    mover.y = (int)IsKeyDown(KEY_S) - (int)IsKeyDown(KEY_W);
   
    //normalizar - evita aumento de velocidade quando se move na diagonal. Fonte (clear code (raylib) 45:00)
    mover = Vector2Normalize(mover);

    jogador->posicao.x += mover.x * jogador->velocidade * variacao_tempo;
    jogador->posicao.y += mover.y * jogador->velocidade * variacao_tempo;

//mecanica do tiro do jogador
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        float angulo_rotacao_tiro = 0.0;
        Vector2 alvo = {0.0f, 0.0f},
            direcao_tiro = {0.0f, 0.0f};

        alvo.x = GetMouseX(),
        alvo.y = GetMouseY();
        
        direcao_tiro = Vector2Subtract(alvo, jogador->posicao);

        angulo_rotacao_tiro = atan2f(direcao_tiro.y, direcao_tiro.x) * RAD2DEG; // Calcula a rotação da bala (converte O atan2f para graus com RAD2DEG)
        
        Tiro_Jogador(jogador->posicao, direcao_tiro, angulo_rotacao_tiro, alvo);
    }
    
}