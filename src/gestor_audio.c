#include "gestor_audio.h"

static Gestor_Audio gestor_de_audio = {0};
static bool audio_iniciado = false;

void Iniciar_Gestor_Audio(){
    if(!audio_iniciado){ //apenas inicia caso o audio n esteja iniciado

        //Preenche as variaveis do volume
            InitAudioDevice();
                gestor_de_audio.volume = 0.4f; //Inicia o volume em 40%
                gestor_de_audio.mutado = false;
                gestor_de_audio.volume_anterior = 0.4f;
                gestor_de_audio.musica_atual = MUSICA_MENU;
                gestor_de_audio.musica_tocando = false;

        //Carrega as musicas
            //encerramento
            gestor_de_audio.musicas[MUSICA_ENCERRAMENTO] = LoadMusicStream("assets/musicas/encerramento.mp3");
            gestor_de_audio.musicas_carregadas[MUSICA_ENCERRAMENTO] = true;
        

        audio_iniciado = true;
       
    }
}

void Encerrar_Gestor_Audio(){
    if(audio_iniciado){ //apenas encerra caso o audio ja esteja iniciado
        PararMusica();
    
        for(int musica = 0; musica < TOTAL_MUSICAS; musica++) {
            if(gestor_de_audio.musicas_carregadas[musica]) {
                UnloadMusicStream(gestor_de_audio.musicas[musica]);
            }
        }
        
        CloseAudioDevice();
        audio_iniciado = false;
    }
}

void Definir_Volume(float volume){
    //garante que o audio esteja no intervalo definido 
    if(volume < VOL_MIN){
        volume = VOL_MIN;
    }
    
    if(volume > VOL_MAX){
        volume = VOL_MAX;
    }

    gestor_de_audio.volume = volume;

    if(gestor_de_audio.musica_tocando){
        Tipo_Musica atual = gestor_de_audio.musica_atual;
        
        if (gestor_de_audio.musicas_carregadas[atual]) {
            SetMusicVolume(gestor_de_audio.musicas[atual], gestor_de_audio.volume);
        }
    }
}

float Obter_Volume(){
    return gestor_de_audio.volume;
}

void Aumentar_Volume(){
    if(!gestor_de_audio.mutado){
        Definir_Volume(gestor_de_audio.volume + VOL_GRAU);
    }
}

void Diminuir_Volume(){
     if(!gestor_de_audio.mutado){
        Definir_Volume(gestor_de_audio.volume - VOL_GRAU);
    }
}

void Mudo(){
   if(gestor_de_audio.mutado){
        //Desmutar
        gestor_de_audio.volume = gestor_de_audio.volume_anterior;
        gestor_de_audio.mutado = false;

        //chama o definir volume apenas a musica estiver realmente tocando
        if(gestor_de_audio.musica_tocando){
            Definir_Volume(gestor_de_audio.volume);
        }
        
    } 
    else {
        //Mutar
        gestor_de_audio.volume_anterior = gestor_de_audio.volume;
        gestor_de_audio.mutado = true;
        Definir_Volume(0.0f);
      
    }  
}

//questionar a necessidade
bool Esta_Mutado() {
    return gestor_de_audio.mutado;
}

void TocarMusica(Tipo_Musica tipo) {
    if(!audio_iniciado) {
        printf("ERRO: Audio não iniciado!\n");
        return;
    }
    
    if(tipo >= TOTAL_MUSICAS) {
        printf("ERRO: Tipo de música inválido!\n");
        return;
    }
    
    // Para a musica atual se estiver tocando
    if(gestor_de_audio.musica_tocando) {
        Tipo_Musica anterior = gestor_de_audio.musica_atual;
        if(gestor_de_audio.musicas_carregadas[anterior]) {
            StopMusicStream(gestor_de_audio.musicas[anterior]);
        }
    }
    
    // Toca a nova musica
    gestor_de_audio.musica_atual = tipo;
    
    if(gestor_de_audio.musicas_carregadas[tipo]) {
        PlayMusicStream(gestor_de_audio.musicas[tipo]);
        SetMusicVolume(gestor_de_audio.musicas[tipo], gestor_de_audio.volume);
        gestor_de_audio.musica_tocando = true;
        
    } 
    else {
        gestor_de_audio.musica_tocando = false;
    }
}

void PausarMusica(){
    if(gestor_de_audio.musica_tocando){ //apenas pausa a musica se estiver tocando
        Tipo_Musica atual = gestor_de_audio.musica_atual;

        if(gestor_de_audio.musicas_carregadas[atual]) {
            PauseMusicStream(gestor_de_audio.musicas[atual]);
        }
    }
}

void RetomarMusica(){
    if(!gestor_de_audio.musica_tocando){ //retoma apenas caso a musica esteja pausada
        Tipo_Musica atual = gestor_de_audio.musica_atual;

        if(gestor_de_audio.musicas_carregadas[atual]) {
            ResumeMusicStream(gestor_de_audio.musicas[atual]);
        }
    }
}

void PararMusica(){
    if(gestor_de_audio.musica_tocando){ //apenas para caso a musica esteja tocando
        Tipo_Musica atual = gestor_de_audio.musica_atual;

        if(gestor_de_audio.musicas_carregadas[atual]) {
            StopMusicStream(gestor_de_audio.musicas[atual]);
        }
    }
    
    gestor_de_audio.musica_tocando = false;
}

void AtualizarMusica(){
    if(audio_iniciado && gestor_de_audio.musica_tocando){ //atualiza apenas se a musica estiver iniciada e tocando
        Tipo_Musica atual = gestor_de_audio.musica_atual;

        if(gestor_de_audio.musicas_carregadas[atual]) {
            UpdateMusicStream(gestor_de_audio.musicas[atual]);
        }
    }  
}

void Desenha_Barra_Volume(Gestor_Audio gestor_de_audio){
    // Limpa o fundo
    ClearBackground((Color){20, 20, 30, 255});
    
    // Título
    DrawText("REGULADOR DE VOLUME (MÚSICA)", 
             GetScreenWidth()/2 - MeasureText("REGULADOR DE VOLUME (MÚSICA)", 40)/2, 
             80, 40, WHITE);
    
    // --- STATUS DA MÚSICA ---
    const char* status = "PARADA";
    Color statusColor = GRAY;
    
    // Verifica se a música atual está tocando.
    // Usamos musica_tocando E a Raylib para ser mais robusto.
    Tipo_Musica atual = gestor_de_audio.musica_atual;
    
    if (gestor_de_audio.musica_tocando) {
        // Se o gestor diz que está tocando, verificamos o Raylib também.
        if (gestor_de_audio.musicas_carregadas[atual] && IsMusicStreamPlaying(gestor_de_audio.musicas[atual])) {
             status = "TOCANDO";
             statusColor = GREEN;
        } else {
             // Caso a música esteja pausada ou parada, mas o flag não foi limpo
             status = "PAUSADA";
             statusColor = ORANGE;
        }
    } else {
         // Verifica se está carregada, mas não tocando (provavelmente pausada)
         if (gestor_de_audio.musicas_carregadas[atual]) {
             status = "PAUSADA";
             statusColor = ORANGE;
         }
    }
    
    DrawText(status, 
             GetScreenWidth()/2 - MeasureText(status, 30)/2, 
             150, 30, statusColor);
    
    // --- BARRA DE VOLUME ---
    int barX = 150;
    int barY = 250;
    int barWidth = 500;
    int barHeight = 60;
    
    // Barra de volume - fundo
    DrawRectangle(barX, barY, barWidth, barHeight, (Color){40, 40, 50, 255});
    DrawRectangleLines(barX, barY, barWidth, barHeight, WHITE);
    
    // Barra de volume - preenchimento
    // Não precisa de verificação extra, pois o volume é sempre válido (0.0 a 1.0)
    
    // Cálculo da largura do preenchimento usando o volume salvo na struct
    int fillWidth = (int)(barWidth * gestor_de_audio.volume);
    
    // Cor gradiente baseada no volume
    Color fillColor;
    if (gestor_de_audio.volume > 0.7f) {
        fillColor = RED;
    } else if (gestor_de_audio.volume > 0.4f) {
        fillColor = ORANGE;
    } else {
        fillColor = GREEN;
    }
    
    DrawRectangle(barX, barY, fillWidth, barHeight, fillColor);
    
    // --- TEXTO DO VOLUME ---
    char volumeText[16];
    
    // Usa o membro 'mutado'
    if (gestor_de_audio.mutado) {
        sprintf(volumeText, "MUDO");
    } else {
        // Exibe o volume salvo na struct
        sprintf(volumeText, "%d%%", (int)(gestor_de_audio.volume * 100));
    }
    
    DrawText(volumeText, 
             barX + barWidth/2 - MeasureText(volumeText, 30)/2, 
             barY + 15, 30, WHITE);
    
    // --- INSTRUÇÕES ---
    int instructY = 380;
    DrawText("CONTROLES:", GetScreenWidth()/2 - MeasureText("CONTROLES:", 25)/2, 
             instructY, 25, YELLOW);
    
    instructY += 50;
    DrawText("SETAS <- / -> : Ajustar volume", 
             GetScreenWidth()/2 - MeasureText("SETAS <- / -> : Ajustar volume", 20)/2, 
             instructY, 20, WHITE);
    
    instructY += 35;
    DrawText("ESPAÇO ou P : Tocar/Pausar", 
             GetScreenWidth()/2 - MeasureText("ESPAÇO ou P : Tocar/Pausar", 20)/2, 
             instructY, 20, WHITE);
    
    instructY += 35;
    DrawText("M : Mudo", 
             GetScreenWidth()/2 - MeasureText("M : Mudo", 20)/2, 
             instructY, 20, WHITE);
    
    instructY += 35;
    DrawText("ESC : Sair", 
            GetScreenWidth()/2 - MeasureText("ESC : Sair", 20)/2, 
            instructY, 20, WHITE);
    
    // Mensagem se música não foi carregada (Usando o array de status)
    if (!gestor_de_audio.musicas_carregadas[MUSICA_ENCERRAMENTO]) { // Usamos um índice conhecido
        DrawText("MÚSICA NÃO CARREGADA", 
                GetScreenWidth()/2 - MeasureText("MÚSICA NÃO CARREGADA", 18)/2, 
                GetScreenHeight() - 40, 18, RED);
    }
}