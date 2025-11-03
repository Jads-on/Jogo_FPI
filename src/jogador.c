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
#define POSICAO_HUD_CENTRO_X 1750
#define POSICAO_HUD_CENTRO_Y 745
#define CUSTO_ENERGETICO 5
#define CUSTO_BALA_EXPLOSIVA 5
#define CUSTO_BALA_PERFURANTE 3

#define LARGURA_FRAME_JOGADOR 400
#define ALTURA_FRAME_JOGADOR 400

// Parametros das imagens usadas no hud 
static Texture2D sprite_barras,
                 sprite_hud_habilidades;

static Rectangle textura_barra_vida[6],
                 textura_barra_energetico[6],
                 hud_tiro_perfurante[4],
                 hud_tiro_explosivo[4],
                 hud_energetico[4];

//spritesheet e animacoes do jogador
static Texture2D sprite_jogador;
static Rectangle jogador_parado[2],
                 jogador_atirando,             //frame do membro inferior
                 jogador_andando[2],
                 jogador_pulando,
                 jogador_arma[3],              //frames dos membros superiores
                 jogador_disparo[3],
                 jogador_energetico_parado,
                 jogador_energetico_andando[3],
                 jogador_energetico_pulando,
                 jogador_energetico_arma[3],
                 jogador_morto[2];

int CalcularFrame(int baterias, int custo) {
    if(baterias >= custo) {
        return 3;  // Cheio - pode usar
    }
    else {
        int frame = baterias * 3/ custo;
        return (frame > 3) ? 3 : frame;  // Garante máximo de 3
    }
}

//procedimentos e funcoes
void IniciarJogador(Jogador *jogador, Vector2 PosicaoInicial){
    
    //inicia as variaveis pra evitar bugs com lixo de memoria
    jogador->posicao = PosicaoInicial;
    jogador->velocidade = VELOCIDADE_JOGADOR;
    jogador->vida = VIDA_JOGADOR;
    jogador->baterias = 13;
    jogador->efeitos.energetico_ativo = false;
    jogador->efeitos.energetico_duracao = 0.0; 
    jogador->Tipo_Tiro = Bala_Padrao; 

    //carrega a textura das barras
        sprite_barras = LoadTexture("assets/sprites/barras/Barras.png");
        for(int frame = 0; frame < 6; frame++){
            //Barra de vida
            textura_barra_vida[frame] = (Rectangle){frame * 200, 0, 200, 200};
                
            //Barra de energetico
            textura_barra_energetico[frame] =(Rectangle){frame * 200, 200, 200, 200};
        }

    //carrega o hud de habilidades
        sprite_hud_habilidades = LoadTexture("assets/sprites/hud_de_habilidades/hud.png");
        for(int frame = 0; frame < 4; frame++){
            //tiro perfurante
            hud_tiro_perfurante[frame] = (Rectangle){frame * 200, 0, 200, 200};

            //energetico
            hud_energetico[frame] = (Rectangle){frame * 200, 200, 200, 200};

            //tiro explosivo
            hud_tiro_explosivo[frame] = (Rectangle){frame * 200, 400, 200, 200};
        }
    
    //carregar e preencher os sprites do jogador
        sprite_jogador = LoadTexture("assets/sprites/jogador/jogador_spritesheet.png");

        //frames do jogador normal
            //frames parado
            for(int frame_parado = 0; frame_parado < 2; frame_parado++){
                jogador_parado[frame_parado] = (Rectangle){frame_parado * LARGURA_FRAME_JOGADOR, 0 * ALTURA_FRAME_JOGADOR, LARGURA_FRAME_JOGADOR, ALTURA_FRAME_JOGADOR};
            }

            //frame atirando (torso)
            jogador_atirando = (Rectangle){2 * LARGURA_FRAME_JOGADOR, ALTURA_FRAME_JOGADOR, 0 * LARGURA_FRAME_JOGADOR, ALTURA_FRAME_JOGADOR};

            //frame andando
            for(int frame_andando = 3; frame_andando < 5; frame_andando++){
                jogador_andando[frame_andando] = (Rectangle){frame_andando * LARGURA_FRAME_JOGADOR, 0 * ALTURA_FRAME_JOGADOR, LARGURA_FRAME_JOGADOR, ALTURA_FRAME_JOGADOR};
            }

            //frame pulando
            jogador_pulando = (Rectangle){5 * LARGURA_FRAME_JOGADOR, ALTURA_FRAME_JOGADOR, 0 * LARGURA_FRAME_JOGADOR, ALTURA_FRAME_JOGADOR};

            //frame do corpo superior
            for(int frame_arma = 0; frame_arma > 3; frame_arma++){
                
                if((frame_arma + 5) == 5){
                    jogador_arma[frame_arma] = (Rectangle){frame_arma * LARGURA_FRAME_JOGADOR, 1 * ALTURA_FRAME_JOGADOR, LARGURA_FRAME_JOGADOR, ALTURA_FRAME_JOGADOR};
                }
                else{
                    jogador_arma[4 - frame_arma] = (Rectangle){frame_arma * LARGURA_FRAME_JOGADOR, 2 * ALTURA_FRAME_JOGADOR, LARGURA_FRAME_JOGADOR, ALTURA_FRAME_JOGADOR};
                }
            }

             //frame do brilho do tiro 
            for(int frame_disparo = 0; frame_disparo > 3; frame_disparo++){

                if((frame_disparo + 5) == 5){ //desloca o frame em 5 frames pra casar com o spritesheet
                    jogador_disparo[frame_disparo] = (Rectangle){frame_disparo * LARGURA_FRAME_JOGADOR, 2 * ALTURA_FRAME_JOGADOR, LARGURA_FRAME_JOGADOR, ALTURA_FRAME_JOGADOR};
                }
                else{
                    jogador_disparo[frame_disparo] = (Rectangle){frame_disparo * LARGURA_FRAME_JOGADOR, 3 * ALTURA_FRAME_JOGADOR, LARGURA_FRAME_JOGADOR, ALTURA_FRAME_JOGADOR};
                }
            }

        //frames do jogador no modo energetico
            jogador_energetico_parado = (Rectangle){6 * LARGURA_FRAME_JOGADOR, 1 * ALTURA_FRAME_JOGADOR, LARGURA_FRAME_JOGADOR, ALTURA_FRAME_JOGADOR};
            
            //frame andando
            for(int frame_andando = 1; frame_andando < 4; frame_andando++){
                jogador_energetico_andando[frame_andando] = (Rectangle){frame_andando * LARGURA_FRAME_JOGADOR, 1 * ALTURA_FRAME_JOGADOR, LARGURA_FRAME_JOGADOR, ALTURA_FRAME_JOGADOR};
            }

            //frame pulando
            jogador_energetico_pulando = (Rectangle){4 * LARGURA_FRAME_JOGADOR, 1 * ALTURA_FRAME_JOGADOR, LARGURA_FRAME_JOGADOR, ALTURA_FRAME_JOGADOR};

            for(int frame_arma = 2; frame_arma < 5; frame_arma++){
                jogador_energetico_arma[frame_arma] = (Rectangle){frame_arma * LARGURA_FRAME_JOGADOR, 2 * ALTURA_FRAME_JOGADOR, LARGURA_FRAME_JOGADOR, ALTURA_FRAME_JOGADOR};
            }

            for(int frame_morto = 2; frame_morto < 4; frame_morto++){
                jogador_morto[frame_morto] = (Rectangle){frame_morto * LARGURA_FRAME_JOGADOR, 3 * ALTURA_FRAME_JOGADOR, LARGURA_FRAME_JOGADOR, ALTURA_FRAME_JOGADOR};
            }
       
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

        if(jogador->Tipo_Tiro != Bala_Explosiva){
            if(jogador->baterias >= CUSTO_BALA_EXPLOSIVA){ //se o jogador tiver baterias suficiente para usar a bala explosiva
                jogador->Tipo_Tiro = Bala_Explosiva; //dispara o tiro explosivo
                jogador->baterias -= CUSTO_BALA_EXPLOSIVA;
            }
        }
    }

    if(IsKeyPressed(KEY_R)){ //botao que ativa o tiro reforcado
        if(jogador->Tipo_Tiro != Bala_Explosiva){
            if(jogador->baterias >= CUSTO_BALA_PERFURANTE){ //se o jogador tiver balas reforcadas
                jogador->Tipo_Tiro = Bala_Perfurante; //dispara o tiro reforcado
                jogador->baterias -= CUSTO_BALA_PERFURANTE;
            }
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
        if((jogador->baterias >= CUSTO_ENERGETICO) && (jogador->efeitos.energetico_ativo == false)){
            jogador->efeitos.energetico_ativo = true;
            jogador->efeitos.energetico_duracao = GetTime() + DURACAO_ENERGETICO;
            
            // Aplica o efeito
            jogador->velocidade *= AUMENTO_DE_VELOCIDADE;

            //regeneração de vida
            if(jogador->vida <= 95){
                jogador->vida += RECUPERACAO_ENERGETICO;
            }
            else{
                jogador->vida = 100;
            }
            jogador->baterias -= CUSTO_ENERGETICO;
        }
    }

    //se o energetico esta ativo 
    if(jogador->efeitos.energetico_ativo == true){
        if(GetTime() >= jogador->efeitos.energetico_duracao){ //se a duracao do energetico acabou

            jogador->velocidade /= AUMENTO_DE_VELOCIDADE; //reduz a velocidade
            jogador->efeitos.energetico_duracao = 0.0;
            jogador->efeitos.energetico_ativo = false;
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

    DrawTextureRec(sprite_barras, textura_barra_vida[nivel_barra_vida], (Vector2) {POSICAO_BARRA_X, POSICAO_BARRA_Y - ESPACO_ENTRE_BARRAS}, WHITE);
}

void JogadorEnergeticoImagem(Jogador jogador){ //desenho e posicionamento da barra de energia

    if(jogador.efeitos.energetico_ativo == false){ //caso o energetico nao esteja ativo, retorna sem fazer nada
        return;
    }

    //caso o energetico seja ativado
    const double tempo_por_nivel = DURACAO_ENERGETICO / 5.0; //uso de const pois o tempo por nivel sempre sera o mesmo, sem alterar,uso de double pois GetTime retorna um double

    double tempo_restante = jogador.efeitos.energetico_duracao - GetTime(); //uso de double pois GetTime retorna um double

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

    DrawTextureRec(sprite_barras, textura_barra_energetico[nivel_barra_energetico], (Vector2) {POSICAO_BARRA_X, POSICAO_BARRA_Y}, WHITE);
}

void HudHabilidadesImagem(Jogador jogador){ //desenho e posicionamento do hud de habilidades
    int frame_tiro_explosivo = CalcularFrame(jogador.baterias, CUSTO_BALA_EXPLOSIVA),
        frame_tiro_perfurante = CalcularFrame(jogador.baterias, CUSTO_BALA_PERFURANTE),
        frame_energetico = CalcularFrame(jogador.baterias, CUSTO_ENERGETICO);

    DrawTextureRec(sprite_hud_habilidades, hud_energetico[frame_energetico], (Vector2) {POSICAO_HUD_CENTRO_X - 115, POSICAO_HUD_CENTRO_Y + 100}, WHITE);
 
    DrawTextureRec(sprite_hud_habilidades, hud_tiro_explosivo[frame_tiro_explosivo], (Vector2) {(POSICAO_HUD_CENTRO_X - 115), (POSICAO_HUD_CENTRO_Y - 80)}, WHITE);
    
    DrawTextureRec(sprite_hud_habilidades, hud_tiro_perfurante[frame_tiro_perfurante], (Vector2) {(POSICAO_HUD_CENTRO_X), (POSICAO_HUD_CENTRO_Y)}, WHITE);
    
}

void DescarregarAssets(){
    UnloadTexture(sprite_barras);
    UnloadTexture(sprite_hud_habilidades);
}