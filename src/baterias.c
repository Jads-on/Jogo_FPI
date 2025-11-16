#include "raylib.h"
#include "jogador.h"
#include "baterias.h"
#include "gestor_audio.h"

static Texture2D frame_bateria;
Bateria baterias_dropadas[MAX_BATERIAS];

void IniciarBaterias(){

    //carregar sprite baterias
    frame_bateria = LoadTexture("assets/sprites/jogador/bateria.png");

    //iniciar baterias
    for(int bateria = 0;bateria < MAX_BATERIAS; bateria++){
        baterias_dropadas[bateria].ativo = false;
        baterias_dropadas[bateria].valor = 0;
        baterias_dropadas[bateria].posicao = (Vector2){0, 0};
        baterias_dropadas[bateria].hitbox = (Rectangle){0, 0, 0, 0};
    }
}

void DesenharBaterias(){
    for(int i = 0; i < MAX_BATERIAS; i++) {
        if(baterias_dropadas[i].ativo) {
            // Desenha a bateria
            DrawTexture(frame_bateria, baterias_dropadas[i].posicao.x, baterias_dropadas[i].posicao.y, WHITE);
            //Debug de hitbox -> DrawRectangle(baterias_dropadas[i].hitbox.x, baterias_dropadas[i].hitbox.y,baterias_dropadas[i].hitbox.width, baterias_dropadas[i].hitbox.height, BLACK);
        } 
    }

}

bool VerificarColisaoBateria(Rectangle hitbox_jogador, int *valor_coletado){
    for(int i = 0; i < MAX_BATERIAS; i++) {
        if(baterias_dropadas[i].ativo) {
            if(CheckCollisionRecs(hitbox_jogador, baterias_dropadas[i].hitbox)) {
                
                // Colisão detectada
                baterias_dropadas[i].ativo = false; // Desativa a bateria
                *valor_coletado = baterias_dropadas[i].valor; // Retorna o valor coletado
                
                TocarSom(SOM_COLETA_BATERIA);
                return true; // Confirma a colisão
            }
        }
    }
    *valor_coletado = 0;
    return false; // Nenhuma colisão
}

void SpawnarBaterias(Vector2 posicao, int valor){
    for(int idx_bateria = 0; idx_bateria < MAX_BATERIAS; idx_bateria++){
        if(!baterias_dropadas[idx_bateria].ativo){
            baterias_dropadas[idx_bateria].posicao = posicao;
            baterias_dropadas[idx_bateria].valor = valor;
            baterias_dropadas[idx_bateria].ativo = true;
            baterias_dropadas[idx_bateria].hitbox = (Rectangle){
            posicao.x + 174,  
            posicao.y + 180,  
            TAM_BATERIA, 
            TAM_BATERIA};
            return;
        }
    }
}

void DescarregarBateria(){
    UnloadTexture(frame_bateria);
}
