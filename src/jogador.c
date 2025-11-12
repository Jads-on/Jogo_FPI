#include "jogador.h"
#include "raymath.h"
#include "tiros.h"
#include "gestor_audio.h"

//parametros do jogador
#define DISTANCIA_ROTACAO 110 //reduz a area do centro de rotacao 
#define VELOCIDADE_JOGADOR 500
#define VIDA_JOGADOR 100
#define ALTURA_PULO -4000.0
#define DURACAO_ENERGETICO 5.0  //duracao do efeito do energetico
#define AUMENTO_DE_VELOCIDADE 2.0 //escolha o multiplo de aumento de velocidade (2.0x, 3.0x, etc)
#define RECUPERACAO_ENERGETICO 20 //recupera um pouco da vida apos tomar
#define CUSTO_ENERGETICO 5
#define CUSTO_BALA_EXPLOSIVA 5
#define CUSTO_BALA_PERFURANTE 3

//organizacao do hud
#define POSICAO_BARRA_X 90
#define POSICAO_BARRA_Y 0
#define ESPACO_ENTRE_BARRAS 50
#define POSICAO_HUD_CENTRO_X 1750
#define POSICAO_HUD_CENTRO_Y 745
#define LARGURA_FRAME_JOGADOR 400
#define ALTURA_FRAME_JOGADOR 400
#define MAX_BATERIAS 50 //qtd de entidades maximas dropadas
#define TAM_BATERIA 50

//ajuste do spwan disparo
#define DESLOCAMENTO_TIRO_X 150
#define DESLOCAMENTO_TIRO_Y 43

//fisica
#define GRAVIDADE 12500.0
#define ALTURA_CHAO 200.0

// Parametros das imagens usadas no hud 
static Bateria baterias_dropadas[MAX_BATERIAS];
static Texture2D sprite_barras,
        sprite_hud_habilidades;

static Rectangle textura_barra_vida[6],
                 textura_barra_energetico[6],
                 hud_tiro_perfurante[4],
                 hud_tiro_explosivo[4],
                 hud_energetico[4],
                 frame_bateria;

//spritesheet e animacoes do jogador
static Texture2D sprite_jogador_torso;          
       Texture2D sprite_jogador_corpo; //usarei em gameover por isso n esta static
                 
static Rectangle jogador_parado[2],
                 jogador_atirando,             //frame do membro inferior
                 jogador_andando[2],
                 jogador_pulando,
                 jogador_arma[3],              //frames dos membros superiores
                 jogador_flash_disparo[3],
                 jogador_energetico_parado,
                 jogador_energetico_andando[3],
                 jogador_energetico_pulando,
                 jogador_energetico_arma[3];

Rectangle jogador_morto[2]; //usarei em gameover por isso n esta static

static inline Vector2 Centro_Torso(Vector2 posicao_jogador){ //funcao que calcula onde unir o corpo e o tor
    return(Vector2){posicao_jogador.x + 95, posicao_jogador.y + 100};
}

static inline Rectangle Inverter_Imagem(Rectangle frame, bool flip){ //funcao que inverte os frames(economia de sprites)
    if(flip){
        frame.width = -frame.width;
        frame.x -= LARGURA_FRAME_JOGADOR / 2;
    }
    return frame;
}

int CalcularFrame(int baterias, int custo){ 
    //calcula o frame dos habilidades (quanto falta pra poder usar)
        if(baterias >= custo){
            return 3;  // Cheio 
        }
        else{
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
        jogador->baterias = 0;
        jogador->total_baterias = jogador->baterias;
        jogador->Tipo_Tiro = Bala_Padrao; 
        jogador->efeitos.energetico_duracao = 0.0; 
        jogador->deslocamento_vertical = 0.0;
        jogador->efeitos.energetico_ativo = false;
        jogador->animacoes.andando = false;
        jogador->animacoes.atirando = false;
        jogador->animacoes.pulando = false;
        jogador->animacoes.morto = false;
        jogador->animacoes.tem_chao = true;
        jogador->animacoes.timer_animacao = 0.0f;
        jogador->animacoes.frame_atual_corpo = 0;
        jogador->animacoes.direcao = 0;
        jogador->hitbox = (Rectangle){jogador->posicao.x, jogador->posicao.y, 128, 318};

    //carrega a textura das barras e inicia o vetor das baterias (pool object)
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
        sprite_jogador_corpo = LoadTexture("assets/sprites/jogador/jogador_spritesheet (corpo).png");
        sprite_jogador_torso = LoadTexture("assets/sprites/jogador/jogador_spritesheet (cabeca e efeitos).png");

        //corpo 
            jogador_atirando = (Rectangle) {2 * LARGURA_FRAME_JOGADOR, 0 * ALTURA_FRAME_JOGADOR, LARGURA_FRAME_JOGADOR, ALTURA_FRAME_JOGADOR};
            jogador_pulando = (Rectangle) {0 * LARGURA_FRAME_JOGADOR, 1 * ALTURA_FRAME_JOGADOR, LARGURA_FRAME_JOGADOR, ALTURA_FRAME_JOGADOR};
            jogador_energetico_parado = (Rectangle) {LARGURA_FRAME_JOGADOR, 1 * ALTURA_FRAME_JOGADOR, LARGURA_FRAME_JOGADOR, ALTURA_FRAME_JOGADOR};
            jogador_energetico_pulando = (Rectangle) {0 * LARGURA_FRAME_JOGADOR, 2 * ALTURA_FRAME_JOGADOR, LARGURA_FRAME_JOGADOR, ALTURA_FRAME_JOGADOR};
                    
            for(int frame = 0; frame < 2; frame++){
                jogador_parado[frame] = (Rectangle) {frame * LARGURA_FRAME_JOGADOR, 0 * ALTURA_FRAME_JOGADOR, LARGURA_FRAME_JOGADOR, ALTURA_FRAME_JOGADOR};
                jogador_andando[frame] =  (Rectangle) {(frame + 3) * LARGURA_FRAME_JOGADOR, 0 * ALTURA_FRAME_JOGADOR, LARGURA_FRAME_JOGADOR, ALTURA_FRAME_JOGADOR};
                jogador_morto[frame] = (Rectangle) {(frame + 1) * LARGURA_FRAME_JOGADOR, 2 * ALTURA_FRAME_JOGADOR, LARGURA_FRAME_JOGADOR, ALTURA_FRAME_JOGADOR};
            }
            
        //corpo e torso
            for(int frame = 0; frame < 3; frame++){
                jogador_energetico_andando[frame] = (Rectangle) {(frame + 2) * LARGURA_FRAME_JOGADOR, 1 * ALTURA_FRAME_JOGADOR, LARGURA_FRAME_JOGADOR, ALTURA_FRAME_JOGADOR};
                        
                //efeitos
                jogador_arma[frame] = (Rectangle) {frame * LARGURA_FRAME_JOGADOR, 0 * ALTURA_FRAME_JOGADOR, LARGURA_FRAME_JOGADOR, ALTURA_FRAME_JOGADOR};
                jogador_energetico_arma[frame] = (Rectangle) {frame * LARGURA_FRAME_JOGADOR, 1 * ALTURA_FRAME_JOGADOR, LARGURA_FRAME_JOGADOR, ALTURA_FRAME_JOGADOR};
                jogador_flash_disparo[frame] = (Rectangle) {frame * LARGURA_FRAME_JOGADOR, 2 * ALTURA_FRAME_JOGADOR, LARGURA_FRAME_JOGADOR, ALTURA_FRAME_JOGADOR};
            }

            //baterias
            frame_bateria = (Rectangle) { 3 * LARGURA_FRAME_JOGADOR, 2 * ALTURA_FRAME_JOGADOR, LARGURA_FRAME_JOGADOR, ALTURA_FRAME_JOGADOR};
            for(int bateria = 0;bateria < MAX_BATERIAS; bateria++){
                baterias_dropadas[bateria].ativo = false;
                baterias_dropadas[bateria].valor = 0;
                baterias_dropadas[bateria].posicao = (Vector2){0, 0};
                baterias_dropadas[bateria].hitbox = (Rectangle){0, 0, 0, 0};
            }
        }

void JogadorImagem(Jogador jogador){

    //calculo da rotacao do torso
        Vector2 alvo = {alvo.x = GetMouseX(), alvo.y = GetMouseY()},
                direcao = {0.0f, 0.0f},
                centro_torso = Centro_Torso(jogador.posicao),
                centro_rotacao = {100, 100};

        float angulo_rotacao = 0.0,
              distacia_mouse_corpo = Vector2Length(Vector2Subtract(alvo, centro_torso));
              
        //Calculo da rotacao do torso, baseado no mouse 
            if (distacia_mouse_corpo >= DISTANCIA_ROTACAO){ //evita bugs quando o mouse esta proximno do player
                direcao = Vector2Subtract(alvo, centro_torso);
                angulo_rotacao = atan2f(direcao.y, direcao.x) * RAD2DEG; 
            }
            Rectangle destRec = {centro_torso.x, centro_torso.y, 400, 400};

    //desenho do corpo
        //caso esteja no chao
            Rectangle frame_corpo;
            if(jogador.animacoes.tem_chao){ //so fica parado se tiver chao

                //andando
                if(jogador.animacoes.andando){
                    if(jogador.efeitos.energetico_ativo){
                        frame_corpo = jogador_energetico_andando[jogador.animacoes.frame_atual_corpo];
                    }
                    else{
                        frame_corpo = jogador_andando[jogador.animacoes.frame_atual_corpo]; 
                    }   
                }
                
                //parado
                else{
                    if(jogador.efeitos.energetico_ativo){
                        frame_corpo = jogador_energetico_parado;
                    }
                    else{
                        frame_corpo = jogador_parado[jogador.animacoes.frame_atual_corpo];
                    }
                }
            }
        
        //Caso não esteja tocando o chao
            //pulando
                else{ //desenho pulo
                    if(jogador.efeitos.energetico_ativo){
                        frame_corpo= jogador_energetico_pulando;
                    }
                    else{
                        frame_corpo = jogador_pulando;
                    }
                }
                frame_corpo = Inverter_Imagem(frame_corpo, jogador.animacoes.direcao < 0); //se precisar inverte
                DrawTextureRec(sprite_jogador_corpo, frame_corpo,jogador.posicao, WHITE);
            
    //desenho do torso
        Rectangle frame_torso; 
        
        if(jogador.Tipo_Tiro == Bala_Perfurante){
            frame_torso = jogador.efeitos.energetico_ativo ? jogador_energetico_arma[1] : jogador_arma[1];
        }
        else if(jogador.Tipo_Tiro == Bala_Explosiva){
            frame_torso = jogador.efeitos.energetico_ativo ? jogador_energetico_arma[2] : jogador_arma[2];
        }
        else{
            frame_torso = jogador.efeitos.energetico_ativo ? jogador_energetico_arma[0] : jogador_arma[0];
        }
           
    //Por fim a inversao do frame do torso quando necessaria
        if(alvo.x < (jogador.posicao.x)){
            frame_torso.height *= -1;
            centro_rotacao.y += 200;
        }
    
    DrawTexturePro(sprite_jogador_torso, frame_torso, destRec, centro_rotacao, angulo_rotacao, WHITE); 
}

void JogadorUpdate(Jogador *jogador){      
    //movimentacao modelo: WASD
        float variacao_tempo = GetFrameTime(); //captura a taxa de fps atual (evita limitar a 60)
        Vector2 mover = {0.0f, 0.0f};
            
    //no eixo x 
        mover.x = (int)IsKeyDown(KEY_D) - (int)IsKeyDown(KEY_A); // fonte (clear code (raylib) 1:05:00)
        jogador->posicao.x += mover.x * jogador->velocidade * variacao_tempo;
        jogador->animacoes.andando = (mover.x != 0)? true: false;
        jogador->animacoes.direcao = mover.x;
            
    //no eixo y (pulo)
        if(IsKeyPressed(KEY_SPACE) && jogador->animacoes.tem_chao){
            jogador->deslocamento_vertical = ALTURA_PULO;
        }

        jogador->deslocamento_vertical += GRAVIDADE * variacao_tempo; //necessario ser em struct para salvar os valores de deslocamento e deixar o pulo suave
        jogador->posicao.y += jogador->deslocamento_vertical * variacao_tempo;

        if(jogador->posicao.y >= ALTURA_CHAO){
        jogador->posicao.y = ALTURA_CHAO;       // Trava o jogador no chão
        jogador->deslocamento_vertical = 0;       // Zera a velocidade vertical
        jogador->animacoes.tem_chao = true;           // Permite pular novamente
        } 
        else{
            jogador->animacoes.tem_chao = false;
        }
        
    // Controle de hitbox
        //ajuste fino da hitbox
            jogador->hitbox.x = jogador->posicao.x + 28;
            jogador->hitbox.y = jogador->posicao.y + 17;
    
        // Debug da hitbox do protagonista -> DrawRectangle(jogador->hitbox.x, jogador->hitbox.y, jogador->hitbox.width, jogador->hitbox.height, BLACK);

    //checagem da coleta de bateria
        for(int i = 0; i < MAX_BATERIAS; i++) {
            if(baterias_dropadas[i].ativo) {
                if(CheckCollisionRecs(jogador->hitbox, baterias_dropadas[i].hitbox)) {
                    
                    // Coleta da bateria
                    jogador->baterias += baterias_dropadas[i].valor;
                    jogador->total_baterias += baterias_dropadas[i].valor;
                    baterias_dropadas[i].ativo = false;
                    TocarSom(SOM_COLETA_BATERIA);
                }
            }
        }

    // Mecanica do tiro do jogador
        if(IsKeyPressed(KEY_Q)){ //botao que ativa o tiro explosivo
            if(jogador->Tipo_Tiro != Bala_Explosiva){
                if(jogador->baterias >= CUSTO_BALA_EXPLOSIVA){ //se o jogador tiver baterias suficiente para usar a bala explosiva
                        jogador->Tipo_Tiro = Bala_Explosiva; //dispara o tiro explosivo
                        jogador->baterias -= CUSTO_BALA_EXPLOSIVA;
                }
                TocarSom(SOM_TROCA_MUNICAO);
            }
        }

        if(IsKeyPressed(KEY_R)){ //botao que ativa o tiro reforcado
            if(jogador->Tipo_Tiro != Bala_Perfurante){
                if(jogador->baterias >= CUSTO_BALA_PERFURANTE){ //se o jogador tiver balas reforcadas
                        jogador->Tipo_Tiro = Bala_Perfurante; //dispara o tiro reforcado
                        jogador->baterias -= CUSTO_BALA_PERFURANTE;
                }
                TocarSom(SOM_TROCA_MUNICAO);
            }
        }   
            
    //detecta o momento que e solicitado o disparo
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){ 

            //inicializa o parametros usados no disparo
                float angulo_rotacao_tiro = 0.0,
                      theta = 0.0;
                Vector2 alvo = {alvo.x = GetMouseX(), alvo.y = GetMouseY()}, //localiza as coordnadas do mouse
                        direcao_tiro = {0.0f, 0.0f},
                        deslocamento_disparo = {DESLOCAMENTO_TIRO_X, DESLOCAMENTO_TIRO_Y},
                        disparo_rotacionado = {0.0f, 0.0f},
                        posicao_tiro = {0.0f, 0.0f},
                        centro_torso = Centro_Torso(jogador->posicao);
                
            //calcula qual a direcao e qual o angulo o disparo deve seguir 
                if(alvo.x < (jogador->posicao.x)){
                    deslocamento_disparo.y *= -1;
                }
            
            //rotacao do tiro (mesma formula da rotacao de conicas)
                direcao_tiro = Vector2Subtract(alvo, centro_torso);  
                angulo_rotacao_tiro = atan2f(direcao_tiro.y, direcao_tiro.x) * RAD2DEG; // Calcula a rotacao da bala (converte o radianos para graus)
                theta = angulo_rotacao_tiro * DEG2RAD; //sera usado para rotacionar o spawn do tiro
                disparo_rotacionado = Vector2Rotate(deslocamento_disparo, theta);
                posicao_tiro = Vector2Add(centro_torso, disparo_rotacionado);

            //executa o disparo e retorna a usar a bala padrao
                Tiro_Jogador(posicao_tiro, direcao_tiro, angulo_rotacao_tiro, jogador->Tipo_Tiro);
                jogador->Tipo_Tiro = Bala_Padrao; //evita o uso de mais de uma bala especial sem ativar o botao
            
            TocarSom(SOM_TIRO);
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
                TocarSom(SOM_EFEITO_ENERGETICO);
            }

        //se o energetico esta ativo 
            if(jogador->efeitos.energetico_ativo == true){
                if(GetTime() >= jogador->efeitos.energetico_duracao){ //se a duracao do energetico acabou

                    jogador->velocidade /= AUMENTO_DE_VELOCIDADE; //reduz a velocidade
                    jogador->efeitos.energetico_duracao = 0.0;
                    jogador->efeitos.energetico_ativo = false;
                }
            }

        //animacao de andar
            if(jogador->animacoes.andando){
                jogador->animacoes.timer_animacao += GetFrameTime();
                
                if(jogador->animacoes.timer_animacao >= 0.345f){
                    jogador->animacoes.timer_animacao = 0.0f;
                    jogador->animacoes.frame_atual_corpo = (jogador->animacoes.frame_atual_corpo + 1) % 2; //incrementa e garante que n passe do indice 2
                }
            }
            
            else{
                jogador->animacoes.andando = false;
                 if(jogador->animacoes.timer_animacao >= 0.345f){
                    jogador->animacoes.timer_animacao = 0.0f;
                    jogador->animacoes.frame_atual_corpo =  (jogador->animacoes.frame_atual_corpo + 1) %2; 
                }
            }

            if(IsKeyPressed(KEY_U))
            jogador->vida -= 20;
            if(IsKeyPressed(KEY_T)) {
                SpawnarBaterias((Vector2){0, 0}, 1);
            }
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
        }
        return;
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
      
    DrawTextureRec(sprite_jogador_corpo, frame_bateria, (Vector2) {(POSICAO_BARRA_X * 18.5), (POSICAO_BARRA_Y - 150)}, WHITE);
    DrawText(TextFormat("%d", jogador.baterias), (POSICAO_BARRA_X * 18.5) + 207, POSICAO_BARRA_Y + 84, 24, BLACK);
    DrawText(TextFormat("%d", jogador.baterias), (POSICAO_BARRA_X * 18.5) + 205, POSICAO_BARRA_Y + 82, 24, DARKBLUE);
}

void DesenharBaterias() {
    for(int i = 0; i < MAX_BATERIAS; i++) {
        if(baterias_dropadas[i].ativo) {
            // Desenha a bateria
            //Debug de hitbox das baterias -> DrawRectangle(baterias_dropadas[i].hitbox.x, baterias_dropadas[i].hitbox.y, baterias_dropadas[i].hitbox.width, baterias_dropadas[i].hitbox.height, BLACK);
            DrawTextureRec(sprite_jogador_corpo, frame_bateria, baterias_dropadas[i].posicao, WHITE);
        } 
    }
}   

void DescarregarAssets(){
    UnloadTexture(sprite_barras);
    UnloadTexture(sprite_hud_habilidades);
    UnloadTexture(sprite_jogador_corpo);
    UnloadTexture(sprite_jogador_torso);
}