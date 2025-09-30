#include "jogador.h"
#include "raymath.h"
#include "tiros.h"
#include <stdio.h>

#define VELOCIDADE_JOGADOR 500
#define VIDA_JOGADOR 100;
#define DURACAO_ENERGETICO 10.0  //duracao do efeito do energetico
#define AUMENTO_DE_VELOCIDADE 2.0 //escolha o multiplo de aumento de velocidade (2.0x, 3.0x, etc)

//procedimentos e funcoes
void IniciarJogador(Jogador *jogador, Vector2 PosicaoInicial){
    
    //inicia as variaveis pra evitar bugs com lixo de memoria
    jogador->posicao.x = PosicaoInicial.x;
    jogador->posicao.y = PosicaoInicial.y;
    jogador->velocidade = VELOCIDADE_JOGADOR;
    jogador->vida = VIDA_JOGADOR;
    jogador->inventario.energeticos = 2;
    jogador->inventario.escudo = 0;
    jogador->inventario.efeitos.energetico_ativo = false;
    jogador->inventario.efeitos.energetico_duracao = 0.0;
    jogador->inventario.municao_explosiva = 2;
    jogador->inventario.municao_perfurante = 2;   
    jogador->Tipo_Tiro = Bala_Padrao; 

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

    if(IsKeyPressed(KEY_Q)){ //botao que ativa o tiro explosivo

        if(jogador->inventario.municao_explosiva > 0){ //se o jogador tiver balas explosivas
            jogador->Tipo_Tiro = Bala_Explosiva; //dispara o tiro explosivo
            jogador->inventario.municao_explosiva -= 1; 
        }
        else{
             jogador->Tipo_Tiro = Bala_Padrao; //se nao, usa balas normais
        }

    }

    if(IsKeyPressed(KEY_R)){ //botao que ativa o tiro reforcado

        if(jogador->inventario.municao_perfurante > 0){ //se o jogador tiver balas reforcadas
            jogador->Tipo_Tiro = Bala_Perfurante; //dispara o tiro reforcado
            jogador->inventario.municao_perfurante -= 1;
        }
        else{
             jogador->Tipo_Tiro = Bala_Padrao; //se nao, usa balas normais
        }

    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        
        //inicializa o parametros usados no disparo
        float angulo_rotacao_tiro = 0.0;
        Vector2 alvo = {0.0f, 0.0f},
                direcao_tiro = {0.0f, 0.0f};

        //localiza as coordnadas do mouse
        alvo.x = GetMouseX(),
        alvo.y = GetMouseY();
        
        //calcula qual a direcao e qual o angulo o disparo deve seguir 
        direcao_tiro = Vector2Subtract(alvo, jogador->posicao);
        angulo_rotacao_tiro = atan2f(direcao_tiro.y, direcao_tiro.x) * RAD2DEG; // Calcula a rotação da bala (converte O atan2f para graus com RAD2DEG)

        //executa o disparo e retorna a usar a bala padrao
        Tiro_Jogador(jogador->posicao, direcao_tiro, angulo_rotacao_tiro, jogador->Tipo_Tiro);
        jogador->Tipo_Tiro = Bala_Padrao; //evita o uso de mais de uma bala especial sem ativar o botao
    }

// mecanicas de power-up

    //energetico
    if(IsKeyPressed(KEY_E)){
        if((jogador->inventario.energeticos > 0) && (jogador->inventario.efeitos.energetico_ativo == false)){
            jogador->inventario.efeitos.energetico_ativo = true;
            jogador->inventario.efeitos.energetico_duracao = GetTime() + DURACAO_ENERGETICO;
            
            // Aplica o efeito
            jogador->velocidade *= AUMENTO_DE_VELOCIDADE;
            jogador->inventario.energeticos -= 1;
        }
    }

    if(jogador->inventario.efeitos.energetico_ativo == true){
        if(GetTime() >= jogador->inventario.efeitos.energetico_duracao){

            jogador->velocidade /=AUMENTO_DE_VELOCIDADE;
            jogador->inventario.efeitos.energetico_duracao = 0.0;
            jogador->inventario.efeitos.energetico_ativo = false;

        }
    }

}