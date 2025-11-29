#include "raylib.h"
#include "boss.h"
#include "jogador.h"

#define VIDA_BOSS 500
#define VELOCIDADE_BOSS 500

void Iniciar_Boss(Boss *boss,  Vector2 Posicao_Inicial_Boss){
    //Status do boss
    boss->vida = VIDA_BOSS;
    boss->velocidade = VELOCIDADE_BOSS;
    boss->estado = BOSS_INTRO;
    boss->ultimo_ataque = 0;

    //Animacoes do boss
    boss->animacoes.andando = false;
    boss->animacoes.atirando = false;
    boss->animacoes.tem_chao = false;
    boss->animacoes.pulando = false;
    boss->animacoes.morto = false;

    //Hitbox boss e alvo
    boss->hitbox = (Rectangle){boss->posicao.x, boss->posicao.y, 200, 200};
    boss->alvo = (Vector2){0, 0}; //Quando for atacar o jogador salve a posição dele aqui

    //Insira abaixo o carregamento do spritesheet do boss
}

void Atualizar_Boss(Boss *boss, Vector2 Posicao_Jogador, float delta){

    // Decrementa o timer genérico
    if (boss->timer_estado > 0) {
        boss->timer_estado -= delta;
    }

    if(boss->ativo){
        switch (boss->estado) {
            
            // --- ESTADO 1: PENSANDO (IDLE) ---
            case BOSS_IDLE:
                // Boss pode seguir o player devagar ou ficar parado
                
                // Quando o tempo de descanso acaba, escolhe um ataque
                if (boss->timer_estado <= 0) {
                    int ataque_escolhido = GetRandomValue(0, 2); // Sorteia entre 0, 1 e 2
                    
                    switch (ataque_escolhido){
                        case 0:
                            boss->estado = BOSS_ATK_TIRO;
                            boss->timer_estado = 3.0f; // Vai atirar por 3 segundos
                            break;
                        case 1:
                            boss->estado = BOSS_ATK_DASH;
                            boss->alvo = Posicao_Jogador; // Mira onde o player está AGORA
                            boss->timer_estado = 1.5f; // Dash dura 1.5s
                            break;
                        case 2:
                            boss->estado = BOSS_ATK_BOMBA;
                            boss->timer_estado = 2.0f;
                            break;
                    }
                }
                break;

            // --- ESTADO 2: ATAQUE DE TIRO ---
            case BOSS_ATK_TIRO:
               
                break;

            // --- ESTADO 3: DASH (INVESTIDA) ---
            case BOSS_ATK_DASH:
                
                break;
                
            // ... outros casos ...
        }
    }
}