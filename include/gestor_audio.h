#ifndef GESTOR_AUDIO_H_
#define GESTOR_AUDIO_H_

#include "raylib.h"
#include "fases_estados.h"
#include <stdbool.h>

#define VOL_MIN 0.0f
#define VOL_MAX 1.0f
#define VOL_GRAU 0.02f

typedef enum{
    BARRA_MUSICA = 0,
    BARRA_SONS
} Tipo_Barra_Selecionada;


typedef enum{
    MUSICA_MENU,
    MUSICA_FASE_1,
    MUSICA_FASE_2,
    MUSICA_BOSS,
    MUSICA_GAMEOVER,
    MUSICA_ENCERRAMENTO,
    TOTAL_MUSICAS,

} Tipo_Musica;

typedef enum{
    SOM_COLETA_BATERIA,
    SOM_DANO_JOGADOR,             
    SOM_DANO_BOSS,
    SOM_EFEITO_ENERGETICO,
    SOM_EXPLOSAO,
    SOM_FINAL_BOSS,
    SOM_INIMIGO_1,
    SOM_INIMIGO_2,
    SOM_INIMIGO_3,
    SOM_MENU_SELECT,
    SOM_MORTE_JOGADOR,
    SOM_PULO,
    SOM_SELECIONAR,  
    SOM_TIRO,
    SOM_TROCA_MUNICAO,
    SOM_VIDA_BAIXA,
    SOM_MISSAO_COMPLETA,
    TOTAL_SONS,

}Tipo_Som;

typedef struct{
    //variaveis da musica
        float volume_anterior_musica;
        float volume_musica;
        bool mutado_musica;
        bool musicas_carregadas[TOTAL_MUSICAS];
        bool musica_tocando;
    
    //Estruturas
        Music musicas[TOTAL_MUSICAS];
        Tipo_Musica musica_atual;
    
    //variaveis de som
        float volume_anterior_sons;    // Volume antes de mutar o Sons
        float volume_sons;             // Volume atual dos Sons
        bool mutado_sons;              // Estado de mudo dos Sons
        bool sons_carregados[TOTAL_SONS]; // Array para saber se o som foi carregado
        Sound sons[TOTAL_SONS];         // Array para armazenar os sons carregados
}Gestor_Audio;

//Funcoes

//Iniciar/finalizar
void Iniciar_Gestor_Audio();

void Encerrar_Gestor_Audio();

//Controle de volume
void Definir_Volume_Musica(float volume);
float Obter_Volume_Musica();
void Aumentar_Volume_Musica();
void Diminuir_Volume_Musica();
void Mutar_Musica();

//Controle da Musica
void TocarMusica(Tipo_Musica tipo);
void PausarMusica();
void RetomarMusica();
void PararMusica();
void AtualizarMusica();
void Controlar_Volume_Musica();

//Controle dos Sons
void TocarSom(Tipo_Som som);
void Aumentar_Volume_Sons();
void Diminuir_Volume_Sons();
void Definir_Volume_Sons(float volume);
void Mutar_Sons();
float Obter_Volume_Sons();
    
void Desenha_Barra_Volume_Musica();
#endif 