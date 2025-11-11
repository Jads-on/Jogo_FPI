#include "tiros.h"
#include "raymath.h"
#define MAX_BALAS 30
#define VELOCIDADE_TIRO 750
#define Dano_Bala_Padrao 10
#define Dano_Bala_Perfurante 20
#define Dano_Bala_Explosiva 5

static Bala balas[MAX_BALAS];

void IniciarTiros(){ //evita lixo de memoria e bugs 

    for(int i = 0; i < MAX_BALAS; i++){
        balas[i].posicao = (Vector2){0, 0};
        balas[i].direcao = (Vector2){0, 0};
        balas[i].angulo = 0;
        balas[i].velocidade = 0;
        balas[i].dano = 0;
        balas[i].ativo = false;
        balas[i].tipo = 0;
     
        balas[i].explodir = false;
        balas[i].tempo_explosao = 0.5;
    }

}

void Tiro_Jogador(Vector2 Posicao_Jogador, Vector2 direcao_tiro, float angulo_rotacao_tiro, int Tipo_Tiro){

    //variaveis necessarias pra o disparo
    bool bala_usada = false; //flag que evita tiro duplo
    
    for(int idx = 0; idx < MAX_BALAS; idx++){ //usa limitadas balas 
        
        if(balas[idx].ativo == false){ //procura uma bala inativa pra reutilizar
            
            if (bala_usada == false){

                switch (Tipo_Tiro){
                    case Bala_Padrao:
                        balas[idx].dano = Dano_Bala_Padrao;
                        balas[idx].tipo = Bala_Padrao;
                        balas[idx].velocidade = VELOCIDADE_TIRO;
                        break;
                    
                    case Bala_Perfurante:
                        balas[idx].dano = Dano_Bala_Perfurante;
                        balas[idx].tipo = Bala_Perfurante;
                        balas[idx].velocidade = VELOCIDADE_TIRO * 2; //a bala reforcada tem o dobro de velocidade das outras
                        break;
                    
                    case Bala_Explosiva:
                        balas[idx].dano = Dano_Bala_Explosiva;
                        balas[idx].tipo = Bala_Explosiva;
                        balas[idx].velocidade = VELOCIDADE_TIRO;
                        break;
                    
                    default:
                        balas[idx].dano = Dano_Bala_Padrao;
                        balas[idx].tipo = Bala_Padrao;
                        balas[idx].velocidade = VELOCIDADE_TIRO;
                        break;
                }

                balas[idx].posicao = Posicao_Jogador;
                balas[idx].angulo = angulo_rotacao_tiro;
                balas[idx].direcao= Vector2Normalize(direcao_tiro);
                balas[idx].ativo = true; //flag que identifique se a bala foi usada ou nao

                bala_usada = true; //flag pra evitar uso de mais bala no mesmo disparo
            }
        }
    }   
}

void AtualizarTiros(){
    float variacao_tempo = GetFrameTime(); 

    for(int idx = 0; idx < MAX_BALAS; idx++){ // procura quais balas estao aivas e atualiza

        if(balas[idx].ativo){
            balas[idx].posicao.x += balas[idx].direcao.x * balas[idx].velocidade * variacao_tempo;
            balas[idx].posicao.y += balas[idx].direcao.y * balas[idx].velocidade * variacao_tempo;
            if(balas[idx].tempo_explosao > 0){
                balas[idx].tempo_explosao--;
            }

            // Desativa a bala se sair da tela
            if(balas[idx].posicao.x > GetScreenWidth() || balas[idx].posicao.x < 0 || balas[idx].posicao.y > GetScreenHeight() || balas[idx].posicao.y < 0){
                balas[idx].ativo = false;
            }
        }

    }

}

void Tiro_Imagem_Jogador(){

    for (int i = 0; i < MAX_BALAS; i++){

        if (balas[i].ativo){
            Vector2 tam_retangulo = {15, 10},
                    origem_rotacao = {tam_retangulo.x/2 , tam_retangulo.y/2};

            Rectangle retangulo_bala = {balas[i].posicao.x, balas[i].posicao.y, tam_retangulo.x, tam_retangulo.y};

            switch (balas[i].tipo){
                case Bala_Padrao:
                    DrawRectanglePro(retangulo_bala,origem_rotacao, balas[i].angulo, BLUE);
                    break;
                
                case Bala_Perfurante:
                    DrawRectanglePro(retangulo_bala,origem_rotacao, balas[i].angulo, GREEN);
                    break;
                    
                case Bala_Explosiva:
                    DrawRectanglePro(retangulo_bala,origem_rotacao, balas[i].angulo, ORANGE);

                    if(balas[i].tempo_explosao <= 0){
                        balas[i].explodir = true;
                    }

                    if(balas[i].explodir){
                        DrawCircleV(balas[i].posicao, 100, RED);
                    }
                    break;
                
                default:
                    DrawCircleV(balas[i].posicao, 5, BLUE);
                    break;
            }

        }

    } 
}