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

    /*
    // Afeta INIMIGOS 
    for(int i = 0; i < MAX_INIMIGOS; i++) { // Supondo que você tenha esse array
        if (inimigos[i].ativo) {
            if (CheckCollisionCircleRec(centroExplosao, raio, inimigos[i].hitbox)) {
                inimigos[i].vida -= dano; // Aplica o dano
                
               
                // TocarSom(SOM_INIMIGO_1);
            }
        }
    }
    */
}

void ColisaoBalaBateria(){
    for(int bala = 0; bala < MAX_BALAS; bala++){ //verifica as balas que estao ativas
        if (balas[bala].ativo && !balas[bala].explodir) { 
            
            for(int bateria = 0; bateria < MAX_BATERIAS; bateria++){ //verifica as baterias que foram dropadas
                if (baterias_dropadas[bateria].ativo) {
                    if (CheckCollisionRecs(balas[bala].hitbox, baterias_dropadas[bateria].hitbox)){
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
