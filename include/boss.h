#ifndef BOSS_H_
#define BOSS_H_

#include "raylib.h"

typedef enum{ //controle dos tipos de tiro do boss (CASO ELE ATIRE)
    BOSS_INTRO = 0,     // Animação de entrada
    BOSS_IDLE = 1,      // Esperando para atacar (vulnerável)
    BOSS_ATK_TIRO = 2,  
    BOSS_ATK_DASH = 3,  
    BOSS_ATK_BOMBA = 4,
    BOSS_MORTO = 5     
}Estado_Boss;

typedef struct Animacoes{
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
} animacoes;

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
    struct Animacoes animacoes;
    Estado_Boss estado;

}Boss;

void Iniciar_Boss(Boss *boss, Vector2 Posicao_Inicial_Boss);

void Atualizar_Boss(Boss *boss, Vector2 Posicao_Jogador, float delta);

void Boss_Imagem(Boss boss);

#endif 