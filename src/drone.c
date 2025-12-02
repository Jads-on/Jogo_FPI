#include "drone.h"
#include "raymath.h"
#include "tiros.h" 
#include "colisoes.h"     
#include "gestor_audio.h"
#include <stdbool.h>
#include <stdio.h>

#define VIDA_DRONE_INICIAL 100
#define VEL_DRONE 250.0f
#define TEMPO_TIROS 5.0f
#define CADENCIA_TIRO 0.3f
#define LARGURA_LASER 30
#define COR_LASER Fade(RED, 0.7f)

// dano do drone 1 em 1
#define DANO_LASER 1
#define DANO_TIRO_DRONE 1

#define DRONE_SCALE 2.0f

// ---------------- TIROS DO DRONE ----------------
#define MAX_BALAS_DRONE 60
#define VEL_TIRO_DRONE 500.0f

typedef struct {
    Vector2 posicao;
    Vector2 direcao;
    Rectangle hitbox;
    bool ativo;
} BalaDrone;

static BalaDrone balas_drone[MAX_BALAS_DRONE];
// ------------------------------------------------

static Texture2D sprite_drone;
static Vector2 pos_drone;
static Rectangle hitbox_drone;

static int vida_drone;
static int vida_drone_max;
static bool laser_ativo = false;
static Rectangle laser_hitbox;

static int estado_drone = 0;   
// 0 = indo direita -> meio (laser)
// 1 = parado no meio atirando
// 2 = indo meio -> esquerda (laser)
// 3 = parado na esquerda atirando
// 4 = indo esquerda -> meio (laser)
// 5 = parado no meio atirando
// 6 = indo meio -> direita (laser)
// 7 = parado na direita atirando

static float timer_fase = 0;   // tempo total na fase atual
static float timer_tiro = 0;   // tempo pra cadência de tiro

bool drone_ativo = false;

// macros (não dependem de ordem de declaração)
#define DRONE_W() (sprite_drone.width * DRONE_SCALE)
#define DRONE_H() (sprite_drone.height * DRONE_SCALE)

// dispara uma bala na direção do jogador
static void TiroDrone(Vector2 origem, Vector2 alvo){
    for(int i = 0; i < MAX_BALAS_DRONE; i++){
        if(!balas_drone[i].ativo){

            Vector2 dir = Vector2Subtract(alvo, origem);
            dir = Vector2Normalize(dir);

            balas_drone[i].posicao = origem;
            balas_drone[i].direcao = dir;
            balas_drone[i].ativo = true;

            balas_drone[i].hitbox.x = origem.x;
            balas_drone[i].hitbox.y = origem.y;

            break;
        }
    }
}

static void AtualizarTirosDrone(Jogador *jogador){
    float dt = GetFrameTime();

    for(int i = 0; i < MAX_BALAS_DRONE; i++){
        if(balas_drone[i].ativo){

            balas_drone[i].posicao.x += balas_drone[i].direcao.x * VEL_TIRO_DRONE * dt;
            balas_drone[i].posicao.y += balas_drone[i].direcao.y * VEL_TIRO_DRONE * dt;

            balas_drone[i].hitbox.x = balas_drone[i].posicao.x;
            balas_drone[i].hitbox.y = balas_drone[i].posicao.y;

            // saiu da tela
            if(balas_drone[i].posicao.x < -50 || balas_drone[i].posicao.x > GetScreenWidth()+50 ||
               balas_drone[i].posicao.y < -50 || balas_drone[i].posicao.y > GetScreenHeight()+50){
                balas_drone[i].ativo = false;
                continue;
            }

            // colisão com jogador
            if(CheckCollisionRecs(balas_drone[i].hitbox, jogador->hitbox)){
                jogador->vida -= DANO_TIRO_DRONE;
                balas_drone[i].ativo = false;
            }
        }
    }
}

static void DesenharTirosDrone(){
    for(int i = 0; i < MAX_BALAS_DRONE; i++){
        if(balas_drone[i].ativo){
            DrawRectangleRec(balas_drone[i].hitbox, YELLOW);
        }
    }
}

void IniciarDrone() {
    sprite_drone = LoadTexture("assets/sprites/inimigos/drone.png");

    pos_drone = (Vector2){1800, 120};  
    estado_drone = 0;

    vida_drone_max = VIDA_DRONE_INICIAL;
    vida_drone = vida_drone_max;

    timer_fase = 0;
    timer_tiro = 0;

    // hitbox começa do tamanho visual (sprite * escala)
    hitbox_drone = (Rectangle){
        pos_drone.x, pos_drone.y,
        DRONE_W(), DRONE_H()
    };

    // init balas do drone
    for(int i = 0; i < MAX_BALAS_DRONE; i++){
        balas_drone[i].posicao = (Vector2){0,0};
        balas_drone[i].direcao = (Vector2){0,0};
        balas_drone[i].hitbox  = (Rectangle){0,0,12,6};
        balas_drone[i].ativo   = false;
    }
}

static void AtualizarHitboxDrone() {
    hitbox_drone.x = pos_drone.x;
    hitbox_drone.y = pos_drone.y;
    hitbox_drone.width  = DRONE_W();
    hitbox_drone.height = DRONE_H();
}

// (por enquanto desenha no update mesmo)
static void LaserDrone(Jogador *jogador) {
    laser_ativo = true;

    laser_hitbox = (Rectangle){
        pos_drone.x + DRONE_W()/2 - LARGURA_LASER/2,
        pos_drone.y + DRONE_H(),
        LARGURA_LASER,
        1500
    };

    if (CheckCollisionRecs(jogador->hitbox, laser_hitbox)) {
        jogador->vida -= DANO_LASER;
    }
}

static void TomarDanoDrone() {
    for(int i = 0; i < MAX_BALAS; i++){
        if(balas[i].ativo && CheckCollisionRecs(balas[i].hitbox, hitbox_drone)){
            vida_drone -= balas[i].dano;
            

            //timer da explosão
            if (balas[i].tipo == Bala_Explosiva) {

                balas[i].explodir = true; 
                balas[i].velocidade = 0;

                // FORÇA o timer para a duração da explosão 
                balas[i].tempo_explosao = 0.2f; 
                Aplicar_Dano_em_Area(balas[i].posicao, 75, balas[i].dano);
                           
            } 
            else if (balas[i].tipo != Bala_Perfurante) { 

                balas[i].ativo = false; 

            }
        }
    }

    if(vida_drone <= 0){
        TocarSom(SOM_INIMIGO_2);
        SpawnarBaterias(pos_drone, 10);
        drone_ativo = false;
    }
}

void AtualizarDrone(Jogador *jogador) {
    if (!drone_ativo) return;
    laser_ativo = false;

    float dt = GetFrameTime();

    AtualizarHitboxDrone();
    TomarDanoDrone();

    timer_fase += dt;
    timer_tiro += dt;

    switch (estado_drone) {

        case 0: // direita -> meio (laser)
            pos_drone.x -= VEL_DRONE * dt;
            LaserDrone(jogador);
            if (pos_drone.x <= 960) {
                estado_drone = 1;
                timer_fase = 0;
                timer_tiro = 0;
            }
            break;

        case 1: { // meio parado atirando
            if(timer_tiro >= CADENCIA_TIRO){
                Vector2 origem = {
                    pos_drone.x + DRONE_W()/2,
                    pos_drone.y + DRONE_H()
                };
                TiroDrone(origem, jogador->posicao);
                timer_tiro = 0;
            }

            if(timer_fase >= TEMPO_TIROS){
                estado_drone = 2;
                timer_fase = 0;
                timer_tiro = 0;
            }
        } break;

        case 2: // meio -> esquerda (laser)
            pos_drone.x -= VEL_DRONE * dt;
            LaserDrone(jogador);
            if (pos_drone.x <= 150){
                estado_drone = 3;
                timer_fase = 0;
                timer_tiro = 0;
            }
            break;

        case 3: { // esquerda parado atirando
            if(timer_tiro >= CADENCIA_TIRO){
                Vector2 origem = {
                    pos_drone.x + DRONE_W()/2,
                    pos_drone.y + DRONE_H()
                };
                TiroDrone(origem, jogador->posicao);
                timer_tiro = 0;
            }

            if(timer_fase >= TEMPO_TIROS){
                estado_drone = 4;
                timer_fase = 0;
                timer_tiro = 0;
            }
        } break;

        case 4: // esquerda -> meio (laser)
            pos_drone.x += VEL_DRONE * dt;
            LaserDrone(jogador);
            if(pos_drone.x >= 960){
                estado_drone = 5;
                timer_fase = 0;
                timer_tiro = 0;
            }
            break;

        case 5: { // meio parado atirando
            if(timer_tiro >= CADENCIA_TIRO){
                Vector2 origem = {
                    pos_drone.x + DRONE_W()/2,
                    pos_drone.y + DRONE_H()
                };
                TiroDrone(origem, jogador->posicao);
                timer_tiro = 0;
            }

            if(timer_fase >= TEMPO_TIROS){
                estado_drone = 6;
                timer_fase = 0;
                timer_tiro = 0;
            }
        } break;

        case 6: // meio -> direita (laser)
            pos_drone.x += VEL_DRONE * dt;
            LaserDrone(jogador);
            if(pos_drone.x >= 1800){
                estado_drone = 7;
                timer_fase = 0;
                timer_tiro = 0;
            }
            break;

        case 7: { // direita parado atirando
            if(timer_tiro >= CADENCIA_TIRO){
                Vector2 origem = {
                    pos_drone.x + DRONE_W()/2,
                    pos_drone.y + DRONE_H()
                };
                TiroDrone(origem, jogador->posicao);
                timer_tiro = 0;
            }

            if(timer_fase >= TEMPO_TIROS){
                estado_drone = 0;
                timer_fase = 0;
                timer_tiro = 0;
            }
        } break;
    }

    AtualizarTirosDrone(jogador);
}

void DesenharDrone() {
    if (!drone_ativo) return;
    
    if(laser_ativo){
        DrawRectangleRec(laser_hitbox, COR_LASER);
    }


    // drone 10x maior
    DrawTextureEx(sprite_drone, pos_drone, 0.0f, DRONE_SCALE, WHITE);

    // barra de vida
    float largura_barra = hitbox_drone.width;
    float altura_barra = 8;
    float x = hitbox_drone.x;
    float y = hitbox_drone.y - 12;

    float pct = (float)vida_drone / (float)vida_drone_max;
    if (pct < 0) pct = 0;

    DrawRectangle(x, y, largura_barra, altura_barra, DARKGRAY);
    DrawRectangle(x, y, largura_barra * pct, altura_barra, GREEN);
    DrawRectangleLines(x, y, largura_barra, altura_barra, BLACK);

    DesenharTirosDrone();

    // debug opcional
    // DrawRectangleLinesEx(hitbox_drone, 2, RED);
}

void Descarregar_Drone(){
    UnloadTexture(sprite_drone);
}
