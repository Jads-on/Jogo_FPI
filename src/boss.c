#include "raylib.h"
#include "boss.h"
#include "jogador.h"
#include "gestor_fases.h"
#include "gestor_audio.h"
#include "fase_2.h"

#define VIDA_BOSS 300
#define VELOCIDADE_BOSS 500

#define ALTURA_FRAME_BOSS 400
#define LARGURA_FRAME_BOSS 400
#define QTD_FRAMES_BOSS 7

#define ALTURA_CHAO 370
#define LIMITE_MAPA_DIREITA 1270
#define LIMITE_MAPA_ESQUERDA 250

static Texture2D sprite_boss;

static Rectangle boss_parado[3],
                 boss_parado_danificado[3],
                 boss_derrotado;

void Iniciar_Boss(Boss *boss,  Vector2 Posicao_Inicial_Boss){

    //Status do boss
    boss->vida = VIDA_BOSS;
    boss->velocidade = VELOCIDADE_BOSS;
    boss->estado = BOSS_INTRO;
    boss->ultimo_ataque = 0,
    boss->progressao = 0; //vai liberar o uso de novos ataques
    boss->ativo = true;

    //Animacoes do boss
    boss->animacoes.frame_atual_corpo = 0;
    boss->animacoes.timer_animacao = 0.0f;
    boss->animacoes.andando = false;
    boss->animacoes.atirando = false;
    boss->animacoes.tem_chao = false;
    boss->animacoes.parado = false;
    boss->animacoes.morto = false;

    //Hitbox boss e alvo
    boss->posicao.x = Posicao_Inicial_Boss.x;
    boss->posicao.y = Posicao_Inicial_Boss.y; 
    boss->hitbox = (Rectangle){boss->posicao.x + 120, boss->posicao.y + 120, 150, 150};
    boss->alvo = (Vector2){0, 0}; //Quando for atacar o jogador salve a posição dele aqui

    //Insira abaixo o carregamento do spritesheet do boss
    sprite_boss = LoadTexture("assets/sprites/boss/juggernaut/juggernaut.png");

    for(int frame = 0; frame < 3; frame++){
        boss_parado[frame] = (Rectangle){LARGURA_FRAME_BOSS * frame, ALTURA_FRAME_BOSS * 0, LARGURA_FRAME_BOSS, ALTURA_FRAME_BOSS};
        boss_parado_danificado[frame] = (Rectangle){LARGURA_FRAME_BOSS * frame, ALTURA_FRAME_BOSS * 1, LARGURA_FRAME_BOSS, ALTURA_FRAME_BOSS};
    }

    boss_derrotado =  (Rectangle){LARGURA_FRAME_BOSS * 0, ALTURA_FRAME_BOSS * 2, LARGURA_FRAME_BOSS, ALTURA_FRAME_BOSS};
}

void Atualizar_Boss(Boss *boss, Vector2 Posicao_Jogador, float delta){
    
    // Decrementa o timer genérico
    if (boss->timer_estado > 0) {
        boss->timer_estado -= delta;
    }
    
    //Aumenta os tipos de ataque do boss conforme a vida cai
    if((boss->vida <= VIDA_BOSS/2) && (boss->progressao == 0)){ //vida cai na metade
        boss->progressao++;
    }
    else if(((boss->vida <= VIDA_BOSS/3) && (boss->progressao == 1))){ //vida cai pra um terco
        boss->progressao++;
    }

    //boss morreu
    if(boss->vida <= 0){
        boss->ativo = false;
        TocarSom(SOM_FINAL_BOSS);
    }

   
    
    // animacao do boss parado
   
    boss->animacoes.timer_animacao += GetFrameTime();
        
    if(boss->animacoes.timer_animacao >= 0.193f){
        boss->animacoes.timer_animacao = 0.0f;
        boss->animacoes.frame_atual_corpo = (boss->animacoes.frame_atual_corpo + 1) % 3;
    }
    
    //controle do limite do mapa que o boss pode estar
    if(boss->posicao.y < ALTURA_CHAO){
        boss->posicao.y = ALTURA_CHAO;
    }
    
    if(boss->posicao.x >= LIMITE_MAPA_DIREITA){
        boss->posicao.x = LIMITE_MAPA_DIREITA;
    }
    else if (boss->posicao.x <= LIMITE_MAPA_ESQUERDA){
        boss->posicao.x = LIMITE_MAPA_ESQUERDA;
    }
    
    //Comportamento do boss
    if(boss->ativo){
        switch(boss->estado) {
            
            // --- ESTADO 1: PENSANDO (IDLE) ---
            case BOSS_INTRO:
            
            break;
            
            case BOSS_IDLE:
            
            boss->animacoes.parado = true;
            
            // Quando o tempo de descanso acaba, escolhe um ataque
            if (boss->timer_estado <= 0) {
                int ataque_escolhido = GetRandomValue(0, boss->progressao); // Sorteia o ataque a ser feito
                
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
                    boss->animacoes.parado = false;
                    
                    break;
                    
                    // --- ESTADO 3: DASH (INVESTIDA) ---
                    case BOSS_ATK_DASH:
                    boss->animacoes.parado = false;
                    
                    break;
                    
                    case BOSS_ATK_BOMBA:
                    break;
                    
                    case BOSS_MORTO:
                    boss->ativo = false;
                    break;
                    
                    // ... outros casos ...
                }
            }
}
        
void Boss_Imagem(Boss boss){
    Rectangle frame_boss;
    
    if(boss.ativo){
        if(boss.vida <= VIDA_BOSS/2){
            frame_boss = boss_parado_danificado[boss.animacoes.frame_atual_corpo];
        }
        else{
            frame_boss = boss_parado[boss.animacoes.frame_atual_corpo];
        }
    }
    else{
        frame_boss = boss_derrotado;
        frame_boss.width = -boss_derrotado.width;
    }

    //Espelhamento invertido
    if (boss.alvo.x < boss.posicao.x && boss.estado != BOSS_IDLE) {
         frame_boss.width = -frame_boss.width; // Espelha horizontalmente
    }

    DrawTextureRec(sprite_boss, frame_boss, boss.posicao, WHITE);
    //Debug hitbox boss -> DrawRectangle(boss.hitbox.x,boss.hitbox.y, boss.hitbox.width, boss.hitbox.height, BLACK);
}