#include "inimigos.h"
#include <math.h> 

#define ALTURA_FRAME_INIMIGO 100
#define LARGURA_FRAME_INIMIGO 100
#define QTD_FRAMES_INIMIGO 2

#define VIDA_INIMIGO 50

//Variáveis Globais
Inimigo spiderlith[MAX_INIMIGOS];

static Texture2D texturaInimigo;
static Rectangle frame_inimigo[2];
int idx_frame_spiderlith_atual = 0;

void IniciarInimigos(void) {

    texturaInimigo = LoadTexture("assets/sprites/inimigos/spiderlith.png");

    for(int frame = 0; frame < QTD_FRAMES_INIMIGO; frame++){
        frame_inimigo[frame] = (Rectangle) {LARGURA_FRAME_INIMIGO * frame, ALTURA_FRAME_INIMIGO, LARGURA_FRAME_INIMIGO, ALTURA_FRAME_INIMIGO};
    }

    // Limpa array de spiderlith
    for (int i = 0; i < MAX_INIMIGOS; i++) {
        spiderlith[i].ativo = false;
        spiderlith->vida = VIDA_INIMIGO;
        spiderlith->tempoDisparo = 0;
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
            idx_frame_spiderlith_atual = 1;

            break; 
        }
    }
}

void DescarregarTexturasInimigos(void) {
    UnloadTexture(texturaInimigo);
}

void SpawnarInimigoEm(Vector2 posicao) {
    for (int i = 0; i < MAX_INIMIGOS; i++) {
        if (!spiderlith[i].ativo) {
            spiderlith[i].vida = 50;
            spiderlith[i].ativo = true;
            spiderlith[i].tempoDisparo = 0; 
            spiderlith[i].frameAtual = 0;
            spiderlith[i].tempoFrame = 0;
            spiderlith[i].pos = posicao;
            spiderlith[i].hitbox.x = posicao.x;
            spiderlith[i].hitbox.y = posicao.y;
            spiderlith[i].hitbox.width = 50; 
            spiderlith[i].hitbox.height = 100;
            break;
        }
    }
}

void CarregarInimigosDaTela(int indiceTela) {
    // Mata todos
    for (int i = 0; i < MAX_INIMIGOS; i++) {
        spiderlith[i].ativo = false;
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
        if (spiderlith[i].ativo) {
            spiderlith[i].hitbox.x = spiderlith[i].pos.x;
             spiderlith[i].hitbox.y = spiderlith[i].pos.y - 50;
            // Animação
            spiderlith[i].tempoFrame += delta;
            if (spiderlith[i].tempoFrame >= VELOCIDADE_FRAME) {
                spiderlith[i].tempoFrame = 0;
                spiderlith[i].frameAtual = (spiderlith[i].frameAtual + 1) % FRAMES_INIMIGO;
            }
            
            // Tiro
            spiderlith[i].tempoDisparo += delta;
            if (spiderlith[i].tempoDisparo > 1.5f) { 
                // Origem do tiro um pouco à frente do inimigo
                Vector2 origemTiro = { spiderlith[i].pos.x, spiderlith[i].pos.y };
                AtirarNoJogador(origemTiro, posicaoJogador);
                spiderlith[i].tempoDisparo = 0;
            }
            
            // Morte/Despawn (Exemplo)
            if (spiderlith[i].vida <= 0){
                if( spiderlith[i].ativo){
                    Vector2 pos = {spiderlith[i].hitbox.x, spiderlith[i].hitbox.y};
                    SpawnarBaterias(pos, 1);
                }
                spiderlith[i].ativo = false;
            }
        }
    }
}

void AtualizarBalasInimigos(float delta, int larguraTela, int alturaTela) {
    idx_frame_spiderlith_atual = 0;
    for (int i = 0; i < MAX_BALAS_INIMIGOS; i++) {
        if (balasInimigos[i].ativo) {
            
            // 1. Movimento (Posição += Direção * Velocidade * Delta)
            balasInimigos[i].posicao.x += balasInimigos[i].direcao.x * balasInimigos[i].velocidade * delta;
            balasInimigos[i].posicao.y += balasInimigos[i].direcao.y * balasInimigos[i].velocidade * delta;
            
            // 2. Sincronizar Hitbox com a nova Posição
            balasInimigos[i].hitbox.x = balasInimigos[i].posicao.x - 5;
            balasInimigos[i].hitbox.y = balasInimigos[i].posicao.y + 10;

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
        if (spiderlith[i].ativo) {
            Vector2 posicao_desenho = { spiderlith[i].pos.x - 25, spiderlith[i].pos.y - 50};

            DrawTextureRec(texturaInimigo, frame_inimigo[idx_frame_spiderlith_atual], posicao_desenho, WHITE);
            //Debug da hitbox do inimigo -> DrawRectangle(spiderlith[i].hitbox.x, spiderlith[i].hitbox.y, spiderlith[i].hitbox.width, spiderlith[i].hitbox.height, BLACK);
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
            /* Debug da hitbox do tiro do inimigo -> DrawRectangle(
                (int)balasInimigos[i].hitbox.x,
                (int)balasInimigos[i].hitbox.y,
                (int)balasInimigos[i].hitbox.width,
                (int)balasInimigos[i].hitbox.height,
                BLACK
            );*/
            
        }
    }
}