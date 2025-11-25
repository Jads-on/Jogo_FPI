#include "raylib.h"
#include "raymath.h"
#include "jogador.h"
#include "inimigos.h"
#include "tiros.h"
#include "gestor_audio.h"
#include "baterias.h"

#define RAIO_EXPLOSAO 100
#define DURACAO_EXPLOSAO 0.2f

void ColisoesMapa(){}

void Aplicar_Dano_em_Area(Vector2 centro_da_explosao, float raio, int dano){
    // Afeta BATERIAS 
    for(int i = 0; i < MAX_BATERIAS; i++) {
        if (baterias_dropadas[i].ativo) {
            if (CheckCollisionCircleRec(centro_da_explosao, raio, baterias_dropadas[i].hitbox)) {
                baterias_dropadas[i].ativo = false; // Destrói
            }
        }
    }

    //afeta inimigos
    for(int i = 0; i < MAX_INIMIGOS; i++) { 
        if (inimigos[i].ativo) {
            // Verifica se a hitbox do inimigo está dentro do círculo da explosão
            if (CheckCollisionCircleRec(centro_da_explosao, raio, inimigos[i].hitbox)){ // Ajuste o Rectangle se tiver hitbox na struct
                
                inimigos[i].vida -= dano; // Aplica o dano da explosão
                
              
            }
        }
    }
}

void ColisaoBalaBateria(Jogador *jogador){
    for(int bala = 0; bala < MAX_BALAS; bala++){ //verifica as balas que estao ativas
        if (balas[bala].ativo && !balas[bala].explodir) { 
            
            for(int bateria = 0; bateria < MAX_BATERIAS; bateria++){ //verifica as baterias que foram dropadas
                if (baterias_dropadas[bateria].ativo) {
                    if (CheckCollisionRecs(balas[bala].hitbox, baterias_dropadas[bateria].hitbox)){
                        jogador->baterias += baterias_dropadas[bateria].valor;
                        baterias_dropadas[bateria].ativo = false;

                        // CORREÇÃO (Bug 2): Lida com o timer da explosão
                        if (balas[bala].tipo == Bala_Explosiva) {
                            balas[bala].explodir = true; 
                            balas[bala].velocidade = 0;
                            // FORÇA o timer para a duração da explosão (inicia a limpeza)
                            balas[bala].tempo_explosao = DURACAO_EXPLOSAO; 
                            break; 
                        } 
                        else if (balas[bala].tipo != Bala_Perfurante) { 
                            balas[bala].ativo = false; 
                            break; 
                        }
                    }
                }
            } 
        }
    }
}

int Verificar_Dano_Balas_Alvo(Rectangle hitbox_alvo) {
    int dano_total = 0;
    for (int idx = 0; idx < MAX_BALAS; idx++) {
        Bala *bala = &balas[idx];
        if (bala->ativo && bala->tipo != Bala_Explosiva) {
            if (CheckCollisionRecs(bala->hitbox, hitbox_alvo)) {
                dano_total += bala->dano;
                if (bala->tipo != Bala_Perfurante) {
                    bala->ativo = false; 
                }
            }
        } 
    }
    return dano_total;
}

int Verificar_Dano_Explosao_Alvo(Rectangle hitbox_alvo) {
    int dano_explosao = 0;
    for (int idx = 0; idx < MAX_BALAS; idx++) {
        Bala *bala = &balas[idx];
        if (bala->ativo && bala->tipo == Bala_Explosiva && bala->explodir) {
            Vector2 centro_explosao = bala->posicao;
            if (CheckCollisionCircleRec(centro_explosao, RAIO_EXPLOSAO, hitbox_alvo)) {
                dano_explosao += bala->dano;
            }
        }
    }
    return dano_explosao;
}

void ColisaoBalaInimigo(){
    for(int i = 0; i < MAX_BALAS; i++){ // Balas do JOGADOR
        if (balas[i].ativo && !balas[i].explodir) { 
            
            for(int j = 0; j < MAX_INIMIGOS; j++){ // INIMIGOS
                if (inimigos[j].ativo) {
                    
                    // Cria retângulo temporário para o inimigo (ou use inimigos[j].hitbox se tiver)

                    if (CheckCollisionRecs(balas[i].hitbox, inimigos[j].hitbox)){
                        
                        // 1. Aplica Dano Direto
                        inimigos[j].vida -= balas[i].dano;
                        // TocarSom(SOM_HIT_INIMIGO);

                        // 2. Lógica por Tipo de Bala
                        if (balas[i].tipo == Bala_Explosiva) {
                            // Configura a explosão
                            //GatilhoExplosao(&balas[i]);
                            
                            // Chama o dano em área imediatamente
                            Aplicar_Dano_em_Area(balas[i].posicao, RAIO_EXPLOSAO, balas[i].dano);
                        } 
                        else if (balas[i].tipo != Bala_Perfurante) { 
                            // Se não for explosiva nem perfurante, a bala some
                            balas[i].ativo = false; 
                        }

                        // Se colidiu com um inimigo, sai do loop de inimigos (para esta bala)
                        // A menos que seja perfurante, aí continua!
                        if (balas[i].tipo != Bala_Perfurante) {
                            break; 
                        }
                    }
                }
            } 
        }
    }
}

// -----------------------------------------------------------------
// 2. COLISÃO: BALA DO INIMIGO x JOGADOR
// -----------------------------------------------------------------
void ColisaoBalaInimigoJogador(Jogador *jogador){
    for(int i = 0; i < MAX_BALAS_INIMIGOS; i++){ // Balas dos INIMIGOS
        if (balasInimigos[i].ativo) {
            
            // Verifica colisão com a hitbox do jogador
            if (CheckCollisionRecs(balasInimigos[i].hitbox, jogador->hitbox)){
                
                // 1. Dano ao Jogador
                jogador->vida -= balasInimigos[i].dano;
                TocarSom(SOM_DANO_JOGADOR); // Se tiver no gestor_audio

                // 2. Bala do inimigo é destruída
                balasInimigos[i].ativo = false;
            }
        }
    }
}
