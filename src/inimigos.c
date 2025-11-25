#include "inimigos.h"
#include <math.h> 

#define ALTURA_FRAME_INIMIGO 100
#define LARGURA_FRAME_INIMIGO 100
#define QTD_FRAMES_INIMIGO 2
//Variáveis Globais
Inimigo inimigos[MAX_INIMIGOS];

static Texture2D texturaInimigo;
static Rectangle frame_inimigo[2];
int idx_frame_inimigo_atual = 0;

void IniciarInimigos(void) {

    texturaInimigo = LoadTexture("assets/sprites/inimigos/inimigo.png");

    for(int frame = 0; frame < QTD_FRAMES_INIMIGO; frame++){
        frame_inimigo[frame] = (Rectangle) {LARGURA_FRAME_INIMIGO * frame, ALTURA_FRAME_INIMIGO, LARGURA_FRAME_INIMIGO, ALTURA_FRAME_INIMIGO};
    }

    // Limpa array de inimigos
    for (int i = 0; i < MAX_INIMIGOS; i++) {
        inimigos[i].ativo = false;
    }
    // Limpa array de balas (Inicializa struct completa)
    for (int i = 0; i < MAX_BALAS_INIMIGOS; i++) {
        balasInimigos[i].ativo = false;
        balasInimigos[i].explodir = false;
        balasInimigos[i].tempo_explosao = 0;
        balasInimigos[i].tipo = Bala_Padrao; // Assumindo que você tem esse enum
    }
}

// Função interna para criar o tiro na direção do jogador
void AtirarNoJogador(Vector2 posicaoOrigem, Vector2 posicaoAlvo) {
    for (int i = 0; i < MAX_BALAS_INIMIGOS; i++) {
        if (!balasInimigos[i].ativo) {
            
            // 1. Posição Inicial
            balasInimigos[i].posicao = posicaoOrigem;
            
            // 2. Calcula vetor direção para o jogador
            Vector2 direcao = {
                posicaoAlvo.x - posicaoOrigem.x,
                posicaoAlvo.y - posicaoOrigem.y
            };
            
            // Normaliza o vetor
            float length = sqrtf(direcao.x * direcao.x + direcao.y * direcao.y);
            if (length > 0) {
                direcao.x /= length;
                direcao.y /= length;
            }
            balasInimigos[i].direcao = direcao;

            // 3. Configurações da Bala (Usando sua Struct)
            balasInimigos[i].velocidade = 800; // Pixels por segundo (mais rápido agora que usamos delta)
            balasInimigos[i].dano = 10;
            balasInimigos[i].ativo = true;
            balasInimigos[i].tipo = Bala_Padrao; // Ou crie um tipo Bala_Inimigo
            
            // 4. Hitbox e Raio
            balasInimigos[i].raio_bala = 5.0f;
            balasInimigos[i].hitbox = (Rectangle){
                posicaoOrigem.x, posicaoOrigem.y, 
                20, 10 // Tamanho da hitbox
            };

            // 5. Calcula o Ângulo (Rotação do sprite)
            // atan2f devolve em radianos, convertemos para graus
            balasInimigos[i].angulo = atan2f(direcao.y, direcao.x) * RAD2DEG;
            idx_frame_inimigo_atual = 1;

            break; 
        }
    }
}

void DescarregarTexturasInimigos(void) {
    UnloadTexture(texturaInimigo);
}

void SpawnarInimigoEm(Vector2 posicao) {
    for (int i = 0; i < MAX_INIMIGOS; i++) {
        if (!inimigos[i].ativo) {
            inimigos[i].pos = posicao;
            inimigos[i].hitbox.x = posicao.x;
            inimigos[i].hitbox.y = posicao.y;
            inimigos[i].vida = 50;
            inimigos[i].ativo = true;
            inimigos[i].tempoDisparo = 0; 
            inimigos[i].frameAtual = 0;
            inimigos[i].tempoFrame = 0;
            inimigos[i].hitbox.width = 100; 
            inimigos[i].hitbox.height = 100;
            break;
        }
    }
}

void CarregarInimigosDaTela(int indiceTela) {
    // Mata todos
    for (int i = 0; i < MAX_INIMIGOS; i++) {
        inimigos[i].ativo = false;
    }
    // Limpa balas (Opcional, mas bom para não levar tiros de uma tela pra outra)
    for (int i = 0; i < MAX_BALAS_INIMIGOS; i++) {
        balasInimigos[i].ativo = false;
    }
    
    // Level Design
    switch (indiceTela) {
        case 0: 
            SpawnarInimigoEm((Vector2){1170, 455}); 
            SpawnarInimigoEm((Vector2){270, 103}); 
            break;
        case 1: 
            SpawnarInimigoEm((Vector2){770, -5}); 
            
            break;
        case 2: 
            SpawnarInimigoEm((Vector2){1300, 490});
            break;
        default: break;
    }
}

void AtualizarInimigos(float delta, Vector2 posicaoJogador) {
    for (int i = 0; i < MAX_INIMIGOS; i++) {
        if (inimigos[i].ativo) {
            inimigos[i].hitbox.x = inimigos[i].pos.x;
             inimigos[i].hitbox.y = inimigos[i].pos.y;
            // Animação
            inimigos[i].tempoFrame += delta;
            if (inimigos[i].tempoFrame >= VELOCIDADE_FRAME) {
                inimigos[i].tempoFrame = 0;
                inimigos[i].frameAtual = (inimigos[i].frameAtual + 1) % FRAMES_INIMIGO;
            }
            
            // Tiro
            inimigos[i].tempoDisparo += delta;
            if (inimigos[i].tempoDisparo > 1.5f) { 
                // Origem do tiro um pouco à frente do inimigo
                Vector2 origemTiro = { inimigos[i].pos.x, inimigos[i].pos.y };
                AtirarNoJogador(origemTiro, posicaoJogador);
                inimigos[i].tempoDisparo = 0;
            }
            
            // Morte/Despawn (Exemplo)
            if (inimigos[i].vida <= 0){
                if( inimigos[i].ativo){
                    Vector2 pos = {inimigos[i].hitbox.x, inimigos[i].hitbox.y};
                    SpawnarBaterias(pos, 1);
                }
                inimigos[i].ativo = false;
            }
        }
    }
}

void AtualizarBalasInimigos(float delta, int larguraTela, int alturaTela) {
    idx_frame_inimigo_atual = 0;
    for (int i = 0; i < MAX_BALAS_INIMIGOS; i++) {
        if (balasInimigos[i].ativo) {
            
            // 1. Movimento (Posição += Direção * Velocidade * Delta)
            balasInimigos[i].posicao.x += balasInimigos[i].direcao.x * balasInimigos[i].velocidade * delta;
            balasInimigos[i].posicao.y += balasInimigos[i].direcao.y * balasInimigos[i].velocidade * delta;
            
            // 2. Sincronizar Hitbox com a nova Posição
            balasInimigos[i].hitbox.x = balasInimigos[i].posicao.x - 5;
            balasInimigos[i].hitbox.y = balasInimigos[i].posicao.y + 5;

            // 3. Remover se sair da tela
            if (balasInimigos[i].posicao.x < -20 || balasInimigos[i].posicao.x > larguraTela + 20 ||
                balasInimigos[i].posicao.y < -20 || balasInimigos[i].posicao.y > alturaTela + 20) {
                balasInimigos[i].ativo = false;
            }
        }
    }
}

void DesenharInimigos(void) {
    for (int i = 0; i < MAX_INIMIGOS; i++) {
        if (inimigos[i].ativo) {
            Vector2 posicao_desenho = { inimigos[i].pos.x - 25, inimigos[i].pos.y - 50};

            DrawTextureRec(texturaInimigo, frame_inimigo[idx_frame_inimigo_atual], posicao_desenho, WHITE);
            //DrawRectangle(inimigos[i].hitbox.x, inimigos[i].hitbox.y,50,50, BLACK);
            
        }
    }
}

void DesenharBalasInimigos(void) {
    for (int i = 0; i < MAX_BALAS_INIMIGOS; i++) {
        if (balasInimigos[i].ativo) {
            
            // Usando DrawTexturePro para aproveitar o 'angulo' da bala
            Vector2 source = {20, 10};
            Rectangle dest = { 
                balasInimigos[i].posicao.x + 10, // Ajuste fino para centro (+5 se a bala for 10x10)
                balasInimigos[i].posicao.y + 15, 
                20,
                10
            };
            
            DrawRectanglePro(dest, source, balasInimigos[i].angulo, RED);
            /* Debug hitbox tiro_inimigo -> DrawRectangle(
                (int)balasInimigos[i].hitbox.x,
                (int)balasInimigos[i].hitbox.y,
                (int)balasInimigos[i].hitbox.width,
                (int)balasInimigos[i].hitbox.height,
                BLACK
            );
            */
        }
    }
}