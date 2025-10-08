#include "jogador.h"
#include "raymath.h"
#include "tiros.h"
#include <stdio.h>

#define VELOCIDADE_JOGADOR 500
#define VIDA_JOGADOR 100
#define DURACAO_ENERGETICO 5.0  //duracao do efeito do energetico
#define AUMENTO_DE_VELOCIDADE 2.0 //escolha o multiplo de aumento de velocidade (2.0x, 3.0x, etc)
#define RECUPERACAO_ENERGETICO 20 //recupera um pouco da vida apos tomar
#define POSICAO_BARRA_X 90
#define POSICAO_BARRA_Y 0
#define ESPACO_ENTRE_BARRAS 50
#define POSICAO_HUD_CENTRO_X 1675
#define POSICAO_HUD_CENTRO_Y 800

// Parametros das imagens usadas pelo jogador
static Texture2D textura_barra_vida[6],
                 textura_barra_energetico[6],
                 hud_habilidades[3];

//procedimentos e funcoes
void IniciarJogador(Jogador *jogador, Vector2 PosicaoInicial){
    
    //inicia as variaveis pra evitar bugs com lixo de memoria
    jogador->posicao.x = PosicaoInicial.x;
    jogador->posicao.y = PosicaoInicial.y;
    jogador->velocidade = VELOCIDADE_JOGADOR;
    jogador->vida = VIDA_JOGADOR;
    jogador->inventario.energeticos = 2;
    jogador->inventario.efeitos.energetico_ativo = false;
    jogador->inventario.efeitos.energetico_duracao = 0.0;
    jogador->inventario.municao_explosiva = 2;
    jogador->inventario.municao_perfurante = 2;   
    jogador->Tipo_Tiro = Bala_Padrao; 

    //carrega a textura das barras
        //Barra de vida
        textura_barra_vida[0] = LoadTexture("assets/sprites/barras/vida/0.png");
        textura_barra_vida[1] = LoadTexture("assets/sprites/barras/vida/1.png");
        textura_barra_vida[2] = LoadTexture("assets/sprites/barras/vida/2.png");
        textura_barra_vida[3] = LoadTexture("assets/sprites/barras/vida/3.png");
        textura_barra_vida[4] = LoadTexture("assets/sprites/barras/vida/4.png");
        textura_barra_vida[5] = LoadTexture("assets/sprites/barras/vida/5.png");

        //Barra de energetico
        textura_barra_energetico[0] = LoadTexture("assets/sprites/barras/energetico/0.png");
        textura_barra_energetico[1] = LoadTexture("assets/sprites/barras/energetico/1.png");
        textura_barra_energetico[2] = LoadTexture("assets/sprites/barras/energetico/2.png");
        textura_barra_energetico[3] = LoadTexture("assets/sprites/barras/energetico/3.png");
        textura_barra_energetico[4] = LoadTexture("assets/sprites/barras/energetico/4.png");
        textura_barra_energetico[5] = LoadTexture("assets/sprites/barras/energetico/5.png");
    
    //carrega o hud de habilidades
        hud_habilidades[0] = LoadTexture("assets/sprites/hud_de_habilidades/energetico.png");
        hud_habilidades[1] = LoadTexture("assets/sprites/hud_de_habilidades/tiro_explosivo.png");
        hud_habilidades[2] = LoadTexture("assets/sprites/hud_de_habilidades/tiro_reforcado.png");
}

void JogadorImagem(Jogador jogador){
//aqui ficara as sprites do protagonista
    DrawRectangle(jogador.posicao.x, jogador.posicao.y, 200, 200, RED);
}

void JogadorUpdate(Jogador *jogador){
    
//movimentacao modelo: WASD)
    float variacao_tempo = GetFrameTime(); //captura a taxa de fps atual (evita limitar a 60)
    Vector2 mover = {0.0f, 0.0f};
    
    //no eixo x 
    mover.x = (int)IsKeyDown(KEY_D) - (int)IsKeyDown(KEY_A); // fonte (clear code (raylib) 1:05:00)
    
    //no eixo y
    mover.y = (int)IsKeyDown(KEY_S) - (int)IsKeyDown(KEY_W);
   
    //normalizar - evita aumento de velocidade quando se move na diagonal. Fonte (clear code (raylib) 45:00)
    mover = Vector2Normalize(mover);

    jogador->posicao.x += mover.x * jogador->velocidade * variacao_tempo;
    jogador->posicao.y += mover.y * jogador->velocidade * variacao_tempo;

//mecanica do tiro do jogador
    if(IsKeyPressed(KEY_Q)){ //botao que ativa o tiro explosivo

        if(jogador->inventario.municao_explosiva > 0){ //se o jogador tiver balas explosivas
            jogador->Tipo_Tiro = Bala_Explosiva; //dispara o tiro explosivo
            jogador->inventario.municao_explosiva -= 1; 
        }
        else{
             jogador->Tipo_Tiro = Bala_Padrao; //se nao, usa balas normais
        }

    }

    if(IsKeyPressed(KEY_R)){ //botao que ativa o tiro reforcado

        if(jogador->inventario.municao_perfurante > 0){ //se o jogador tiver balas reforcadas
            jogador->Tipo_Tiro = Bala_Perfurante; //dispara o tiro reforcado
            jogador->inventario.municao_perfurante -= 1;
        }
        else{
             jogador->Tipo_Tiro = Bala_Padrao; //se nao, usa balas normais
        }

    }
    
    //detecta o momento que e solicitado o disparo
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){ 
        //inicializa o parametros usados no disparo
        float angulo_rotacao_tiro = 0.0;
        Vector2 alvo = {0.0f, 0.0f},
                direcao_tiro = {0.0f, 0.0f};

        //localiza as coordnadas do mouse
        alvo.x = GetMouseX(),
        alvo.y = GetMouseY();
        
        //calcula qual a direcao e qual o angulo o disparo deve seguir 
        direcao_tiro = Vector2Subtract(alvo, jogador->posicao);
        angulo_rotacao_tiro = atan2f(direcao_tiro.y, direcao_tiro.x) * RAD2DEG; // Calcula a rotação da bala (converte O atan2f para graus com RAD2DEG)

        //executa o disparo e retorna a usar a bala padrao
        Tiro_Jogador(jogador->posicao, direcao_tiro, angulo_rotacao_tiro, jogador->Tipo_Tiro);
        jogador->Tipo_Tiro = Bala_Padrao; //evita o uso de mais de uma bala especial sem ativar o botao
    }

// mecanicas de power-up
    //energetico
    if(IsKeyPressed(KEY_E)){
        if((jogador->inventario.energeticos > 0) && (jogador->inventario.efeitos.energetico_ativo == false)){
            jogador->inventario.efeitos.energetico_ativo = true;
            jogador->inventario.efeitos.energetico_duracao = GetTime() + DURACAO_ENERGETICO;
            
            // Aplica o efeito
            jogador->velocidade *= AUMENTO_DE_VELOCIDADE;

            //regeneração de vida
            if(jogador->vida <= 95){
                jogador->vida += RECUPERACAO_ENERGETICO;
            }
            else{
                jogador->vida = 100;
            }

            jogador->inventario.energeticos -= 1;
        }
    }

    //se o energetico esta ativo 
    if(jogador->inventario.efeitos.energetico_ativo == true){
        if(GetTime() >= jogador->inventario.efeitos.energetico_duracao){ //se a duracao do energetico acabou

            jogador->velocidade /= AUMENTO_DE_VELOCIDADE; //reduz a velocidade
            jogador->inventario.efeitos.energetico_duracao = 0.0;
            jogador->inventario.efeitos.energetico_ativo = false;

        }
    }
}

void JogadorVidaImagem(Jogador jogador){
    int nivel_barra_vida = jogador.vida / 20; //divide a vida total em partes iguais para cada nivel da barra de vida

    //para casos de a vida ficar fora do intervalo definido por algum bug
    if(nivel_barra_vida <= 0){
        nivel_barra_vida = 0;
    }
    else if(nivel_barra_vida >= 5){
        nivel_barra_vida = 5;
    }

    DrawTexture(textura_barra_vida[nivel_barra_vida], POSICAO_BARRA_X, POSICAO_BARRA_Y - ESPACO_ENTRE_BARRAS, WHITE);
}

void JogadorEnergeticoImagem(Jogador jogador){
    if(jogador.inventario.efeitos.energetico_ativo == false){ //caso o energetico nao esteja ativo, retorna sem fazer nada
        return;
    }

    //caso o energetico seja ativado
    const double tempo_por_nivel = DURACAO_ENERGETICO / 5.0; //uso de const pois o tempo por nivel sempre sera o mesmo, sem alterar,uso de double pois GetTime retorna um double

    double tempo_restante = jogador.inventario.efeitos.energetico_duracao - GetTime(); //uso de double pois GetTime retorna um double

    //caso o tempo restante saia do intervalo definido (evita bugs)
    if(tempo_restante <= 0.0){
        tempo_restante = 0.0;
    }

    //calculo do nivel de energia restante
    int nivel_barra_energetico = (int)(tempo_restante / tempo_por_nivel);

    if(nivel_barra_energetico >= 5){
        nivel_barra_energetico = 5;
    }
    else if(nivel_barra_energetico <= 0){
        nivel_barra_energetico = 0;
    }

    DrawTexture(textura_barra_energetico[nivel_barra_energetico], (POSICAO_BARRA_X), (POSICAO_BARRA_Y), WHITE);
}

void HudHabilidadesImagem(Jogador jogador){

    if(jogador.inventario.energeticos > 0){
        DrawTexture(hud_habilidades[0], POSICAO_HUD_CENTRO_X, POSICAO_HUD_CENTRO_Y, WHITE);
    }
    if(jogador.inventario.municao_explosiva > 0){
        DrawTexture(hud_habilidades[1], (POSICAO_HUD_CENTRO_X + 85), (POSICAO_HUD_CENTRO_Y - 85), WHITE);
    }
    if(jogador.inventario.municao_perfurante > 0){
        DrawTexture(hud_habilidades[2], (POSICAO_HUD_CENTRO_X + 105), (POSICAO_HUD_CENTRO_Y + 100), WHITE);
    }
}

void DescarregarAssets(){
    for(int i = 0; i < 6; i++){
        UnloadTexture(textura_barra_vida[i]);
    }
}