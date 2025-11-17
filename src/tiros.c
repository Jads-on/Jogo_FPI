#include "tiros.h"
#include "raymath.h"
#include "baterias.h"

#define VELOCIDADE_TIRO 750
#define Dano_Bala_Padrao 10
#define Dano_Bala_Perfurante 20
#define Dano_Bala_Explosiva 5
#define TEMPO_EXPLOSAO 0.2f
#define LARGURA_FRAME_JOGADOR 200
#define ALTURA_FRAME_JOGADOR 200
#define FRAMES_EXPLOSAO 3

static Texture2D explosao;
static Rectangle frames_explosao[3];

Bala balas[MAX_BALAS];

void IniciarTiros(){ //evita lixo de memoria e bugs 

    for(int i = 0; i < MAX_BALAS; i++){
        balas[i].posicao = (Vector2){0, 0};
        balas[i].direcao = (Vector2){0, 0};
        balas[i].hitbox = (Rectangle){balas[i].posicao.x, balas[i].posicao.y, 20, 10};
        balas[i].angulo = 0;
        balas[i].velocidade = 0;
        balas[i].dano = 0;
        balas[i].ativo = false;
        balas[i].tipo = 0;
     
        balas[i].explodir = false;
        balas[i].tempo_explosao = 0.0;
    }
    explosao = LoadTexture("assets/sprites/jogador/jogador_spritesheet (cabeca e efeitos).png");

    for(int frame = 0; frame < 3; frame++){         
        frames_explosao[frame] = (Rectangle) {frame * LARGURA_FRAME_JOGADOR, 2 * ALTURA_FRAME_JOGADOR, LARGURA_FRAME_JOGADOR, ALTURA_FRAME_JOGADOR};
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
                        balas[idx].explodir = false;
                        balas[idx].tempo_explosao = TEMPO_EXPLOSAO;
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
            if(balas[idx].explodir){ //se a bala estiver explodindo
                
                balas[idx].tempo_explosao -= variacao_tempo;  
                
                if(balas[idx].tempo_explosao <= 0.0f){
                    balas[idx].ativo = false;      // Remove bala
                    balas[idx].explodir = false;   // Reseta flag
                }
                
                continue;  // Não atualiza posição durante explosão
            }

            balas[idx].posicao.x += balas[idx].direcao.x * balas[idx].velocidade * variacao_tempo;
            balas[idx].posicao.y += balas[idx].direcao.y * balas[idx].velocidade * variacao_tempo;
        
            //atualiza a posicao da hitbox da bala
            balas[idx].hitbox.x = balas[idx].posicao.x;
            balas[idx].hitbox.y = balas[idx].posicao.y;
            
            // Desativa se sair da tela 
            if(!balas[idx].explodir && (balas[idx].posicao.x > GetScreenWidth() || balas[idx].posicao.x < 0 || balas[idx].posicao.y > GetScreenHeight() || balas[idx].posicao.y < 0)){
                balas[idx].ativo = false;
            }
        }
    }
}

void Tiro_Imagem_Jogador(){

    for (int i = 0; i < MAX_BALAS; i++){

        if (balas[i].ativo){
            Vector2 tam_retangulo = {10, 5},
                    origem_rotacao = {tam_retangulo.x/2 , tam_retangulo.y/2};

            Rectangle retangulo_bala = {balas[i].posicao.x, balas[i].posicao.y, tam_retangulo.x, tam_retangulo.y};

            switch (balas[i].tipo){
                case Bala_Padrao:
                    DrawRectanglePro(retangulo_bala,origem_rotacao, balas[i].angulo, BLUE);
                    //Debug de hitbox -> DrawRectangle(balas[i].hitbox.x, balas[i].hitbox.y, balas[i].hitbox.width, balas[i].hitbox.height, BLACK);
                    break;
                
                case Bala_Perfurante:
                    DrawRectanglePro(retangulo_bala,origem_rotacao, balas[i].angulo, GREEN);
                    //Debug de hitbox -> DrawRectangle(balas[i].hitbox.x, balas[i].hitbox.y, balas[i].hitbox.width, balas[i].hitbox.height, BLACK);
                    break;
                    
                case Bala_Explosiva:
                    if (!balas[i].explodir){
                        DrawRectanglePro(retangulo_bala, origem_rotacao, balas[i].angulo, ORANGE);
                    }

                    // Desenha a explosao da bala
                    if(balas[i].explodir){

                        //calculo do frame necessario
                        float progresso = 1.0f - (balas[i].tempo_explosao / TEMPO_EXPLOSAO); 
                        int frame_atual = (int)(progresso * FRAMES_EXPLOSAO);

                        if(frame_atual >= FRAMES_EXPLOSAO){ //decrementa o frame usado
                            frame_atual = FRAMES_EXPLOSAO - 1;
                        }
                                    
                        Rectangle dest = {
                            balas[i].posicao.x,
                            balas[i].posicao.y,
                            LARGURA_FRAME_JOGADOR,
                            ALTURA_FRAME_JOGADOR
                        };
                        
                        Vector2 origem = {
                            LARGURA_FRAME_JOGADOR / 2.0f,
                            ALTURA_FRAME_JOGADOR / 2.0f
                        };
                        
                        float alpha = 1.0f;
                        if(progresso > 0.7f){ 
                            alpha = 1.0f - ((progresso - 0.7f) / 0.3f);
                        }
                        
                        DrawTexturePro(
                            explosao,                   
                            frames_explosao[frame_atual],  
                            dest,                       
                            origem,                      
                            0.0f,                     
                            Fade(WHITE, alpha)            
                        );
                    }
                    break;
                
                default:
                    break;
            }

        }

    } 
}

void DescarregarExplosao(){
    UnloadTexture(explosao);
}