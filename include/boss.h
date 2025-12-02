#ifndef BOSS_H_
#define BOSS_H_

#include "raylib.h"

typedef enum{ //controle dos tipos de tiro do boss (CASO ELE ATIRE)
    BOSS_INTRO,     // Animação de entrada
    BOSS_IDLE,      // Esperando para atacar (vulnerável)
    BOSS_ATK_TIRO,  
    BOSS_ATK_DASH,  
    BOSS_ATK_BOMBA,
    BOSS_MORTO      
}Estado_Boss;

typedef struct{
    int frame_atual_corpo,
        qtd_frames,
        direcao;

    float angulo_mira,
          timer_animacao;
          
    bool andando,
         atirando,
         parado,
         morto,
         tem_chao;
}Animacoes;

typedef struct{
    int vida,
    velocidade,
    ultimo_ataque,
    progressao;

    bool ativo,
         fase_2;
        
    float timer_estado;

    //fisica
    Vector2 posicao,
            alvo;

    Rectangle hitbox;
    Animacoes animacoes;
    Estado_Boss estado;

}Boss;

void Iniciar_Boss(Boss *boss, Vector2 Posicao_Inicial_Boss);

void Atualizar_Boss(Boss *boss, Vector2 Posicao_Jogador);

void Boss_Imagem(Boss boss);

#endif 