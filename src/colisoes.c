#include "raylib.h"
#include "raymath.h"
#include "jogador.h"
#include "inimigos.h"
#include "tiros.h"
#include "gestor_audio.h"
#include "baterias.h"
#include "boss.h"

#define RAIO_EXPLOSAO 100
#define DURACAO_EXPLOSAO 0.2f

void ColisoesMapa(){}

void Aplicar_Dano_em_Area(Vector2 centro_da_explosao, float raio, int dano){

    //afeta BATERIAS 
    for(int i = 0; i < MAX_BATERIAS; i++) {
        if (baterias_dropadas[i].ativo) {
            if (CheckCollisionCircleRec(centro_da_explosao, raio, baterias_dropadas[i].hitbox)) {
                baterias_dropadas[i].ativo = false; // Destrói
            }
        }
    }

    //afeta INIMIGOS
    for(int i = 0; i < MAX_INIMIGOS; i++) { 
        if (spiderlith[i].ativo) {

            // Verifica se a hitbox do inimigo está dentro do círculo da explosão
            if (CheckCollisionCircleRec(centro_da_explosao, raio, spiderlith[i].hitbox)){
                
                spiderlith[i].vida -= dano; // Aplica o dano da explosão
            }
        }
    }
}

void ColisaoBalaBateria(Jogador *jogador){

    //verifica as balas que estao ativas
    for(int bala = 0; bala < MAX_BALAS; bala++){ 

        if (balas[bala].ativo && !balas[bala].explodir){ 
            
            //verifica as baterias que foram dropadas
            for(int bateria = 0; bateria < MAX_BATERIAS; bateria++){

                if (baterias_dropadas[bateria].ativo){

                    if (CheckCollisionRecs(balas[bala].hitbox, baterias_dropadas[bateria].hitbox)){

                        jogador->baterias += baterias_dropadas[bateria].valor; //se a bala acera a bateria o jogador coleta
                        jogador->total_baterias += baterias_dropadas[bateria].valor; //adiciona ao total de baterias coletadas
                        TocarSom(SOM_COLETA_BATERIA);
                        baterias_dropadas[bateria].ativo = false;

                        //timer da explosão
                        if (balas[bala].tipo == Bala_Explosiva) {

                            balas[bala].explodir = true; 
                            balas[bala].velocidade = 0;

                            // FORÇA o timer para a duração da explosão 
                            balas[bala].tempo_explosao = DURACAO_EXPLOSAO; 
                          

                        } 
                        else if (balas[bala].tipo != Bala_Perfurante) { 

                            balas[bala].ativo = false; 

                        }
                    }
                }
            } 
        }
    }
}

void ColisaoBalaInimigo(){
        
    for(int i = 0; i < MAX_BALAS; i++){ // Balas do JOGADOR

        if (balas[i].ativo && !balas[i].explodir) { 
            
            for(int j = 0; j < MAX_INIMIGOS; j++){ // INIMIGOS

                if (spiderlith[j].ativo) {
                    
                    if (CheckCollisionRecs(balas[i].hitbox, spiderlith[j].hitbox)){
                        
                        //aplica Dano Direto
                        spiderlith[j].vida -= balas[i].dano;

                        //timer da explosão
                        if (balas[i].tipo == Bala_Explosiva) {

                            balas[i].explodir = true; 
                            balas[i].velocidade = 0;

                            // FORÇA o timer para a duração da explosão 
                            balas[i].tempo_explosao = DURACAO_EXPLOSAO; 
                            Aplicar_Dano_em_Area(balas[i].posicao, 75, balas[i].dano);
                           
                        } 
                        else if (balas[i].tipo != Bala_Perfurante) { 

                            balas[i].ativo = false; 

                        }
                    }
                }
            } 
        }
    }
}

void ColisaoBalaBoss(Boss boss){
    for(int i = 0; i < MAX_BALAS; i++){ // Balas dos INIMIGOS
        if (balas[i].ativo) {
            
            // Verifica colisão com a hitbox do jogador
            if (CheckCollisionRecs(balas[i].hitbox, boss.hitbox)){
                
                // 1. Dano ao Jogador
                boss.vida -= balas[i].dano;
                
                //timer da explosão
                if (balas[i].tipo == Bala_Explosiva) {

                    balas[i].explodir = true; 
                    balas[i].velocidade = 0;

                    // FORÇA o timer para a duração da explosão 
                    balas[i].tempo_explosao = DURACAO_EXPLOSAO; 
                    Aplicar_Dano_em_Area(balas[i].posicao, 75, balas[i].dano);
                           
                } 
                else if((balas[i].tipo != Bala_Perfurante) && boss.vida <= 150){ 
                    balas[i].ativo = false; 
                }
            }
        }
    }
}