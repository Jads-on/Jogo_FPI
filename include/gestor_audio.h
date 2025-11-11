#ifndef GESTOR_AUDIO_H_
#define GESTOR_AUDIO_H_

#include "raylib.h"
#include "fases_estados.h"
#include <stdbool.h>

#define VOL_MIN 0.0f
#define VOL_MAX 1.0f
#define VOL_GRAU 0.02f
#define TOTAL_MUSICAS 6

typedef enum{
    MUSICA_MENU,
    MUSICA_FASE_1,
    MUSICA_FASE_2,
    MUSICA_BOSS,
    MUSICA_GAMEOVER,
    MUSICA_ENCERRAMENTO,

} Tipo_Musica;

typedef struct{
    //variaveis da musica
        float volume;
        bool mutado;
        float volume_anterior;
    
    //Estruturas 
    Music musicas[TOTAL_MUSICAS];
    bool musicas_carregadas[TOTAL_MUSICAS];
    Tipo_Musica musica_atual;
    bool musica_tocando;
}Gestor_Audio;

//Funcoes

//Iniciar/finalizar
void Iniciar_Gestor_Audio();

void Encerrar_Gestor_Audio();

//Controle de volume
void Definir_Volume(float volume);
float Obter_Volume();
void Aumentar_Volume();
void Diminuir_Volume();
void Mudo();
bool Esta_Mutado();

//Controle da Musica
void TocarMusica(Tipo_Musica tipo);
void PausarMusica();
void RetomarMusica();
void PararMusica();
void AtualizarMusica();

void Desenha_Barra_Volume(Gestor_Audio gestor_de_audio);
#endif 