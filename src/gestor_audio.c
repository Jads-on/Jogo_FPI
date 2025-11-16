#include "stdio.h"
#include "gestor_audio.h"

static bool audio_iniciado = false;
static Gestor_Audio gestor_de_audio = {0};
static Tipo_Barra_Selecionada barra_selecionada = BARRA_MUSICA;

extern Estados_Jogo estado_anterior; // extern para ser usado em outras fontes    

void CarregarSom(int indice, const char* caminho){ //otimiza o carregamento dos sons
    gestor_de_audio.sons[indice] = LoadSound(caminho);
    gestor_de_audio.sons_carregados[indice] = true;
}

void CarregarMusica(int indice, const char* caminho){ //otimiza o carregamento dos sons
    gestor_de_audio.musicas[indice] = LoadMusicStream(caminho);
    gestor_de_audio.musicas_carregadas[indice] = true;
}

void Iniciar_Gestor_Audio(){
    if(!audio_iniciado){ //apenas inicia caso o audio n esteja iniciado

        //Preenche as variaveis
            InitAudioDevice();
            //musica
                gestor_de_audio.volume_musica = 0.4f; //Inicia o volume em 40%
                gestor_de_audio.mutado_musica = false;
                gestor_de_audio.volume_anterior_musica = 0.4f;
                gestor_de_audio.musica_atual = MUSICA_MENU;
                gestor_de_audio.musica_tocando = false;
            
            //sons
                gestor_de_audio.volume_sons = 0.5f;
                gestor_de_audio.mutado_sons = false;
                gestor_de_audio.volume_anterior_sons = 0.5f;

        //Carrega as musicas
            CarregarMusica(MUSICA_MENU, "assets/musicas/(Menu)Punch_Deck-Music_To_Wear_Fingerless_Gloves_To.mp3");
             CarregarMusica(MUSICA_FASE_1, "assets/musicas/(Fase_1)Carpenter_Brut-Turbo_Killer.mp3");
              CarregarMusica(MUSICA_ENCERRAMENTO, "assets/musicas/encerramento.mp3");
         
        

        //Carrega Sons
            CarregarSom(SOM_COLETA_BATERIA, "assets/sons/coleta_bateria.mp3");
             CarregarSom(SOM_DANO_BOSS, "assets/sons/dano_boss.mp3");
              CarregarSom(SOM_DANO_JOGADOR, "assets/sons/dano.mp3");
               CarregarSom(SOM_EFEITO_ENERGETICO, "assets/sons/efeito_energetico.mp3");
                CarregarSom(SOM_EXPLOSAO, "assets/sons/explosao.mp3");
                 CarregarSom(SOM_FINAL_BOSS, "assets/sons/final_boss.mp3"); 
                  CarregarSom(SOM_INIMIGO_1, "assets/sons/inimigo1.mp3");
                   CarregarSom(SOM_INIMIGO_2, "assets/sons/inimigo2.mp3");
                    CarregarSom(SOM_INIMIGO_3, "assets/sons/inimigo3.mp3");
                     CarregarSom(SOM_MENU_SELECT, "assets/sons/menu_select.mp3");
                      CarregarSom(SOM_MORTE_JOGADOR, "assets/sons/morte_player.mp3");
                       CarregarSom(SOM_SELECIONAR, "assets/sons/selecionar.mp3");
                        CarregarSom(SOM_TIRO, "assets/sons/tiro.mp3");
                         CarregarSom(SOM_TROCA_MUNICAO, "assets/sons/troca_municao.mp3");
                          CarregarSom(SOM_VIDA_BAIXA, "assets/sons/vida_baixa.mp3");
                           CarregarSom(SOM_PULO, "assets/sons/pulo.mp3");
                    
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

        for(int idx_som = 0; idx_som < TOTAL_SONS; idx_som++) {
            if(gestor_de_audio.sons_carregados[idx_som]) {
                UnloadSound(gestor_de_audio.sons[idx_som]);
            }
        }
        
        CloseAudioDevice();
        audio_iniciado = false;
    }
}

void Definir_Volume_Musica(float volume){
    //garante que o audio esteja no intervalo definido 
    if(volume < VOL_MIN){
        volume = VOL_MIN;
    }
    
    if(volume > VOL_MAX){
        volume = VOL_MAX;
    }

    gestor_de_audio.volume_musica = volume;

    if(gestor_de_audio.musica_tocando){
        Tipo_Musica atual = gestor_de_audio.musica_atual;
        
        if (gestor_de_audio.musicas_carregadas[atual]) {
            SetMusicVolume(gestor_de_audio.musicas[atual], gestor_de_audio.volume_musica);
        }
    }
}

float Obter_Volume_Musica(){
    return gestor_de_audio.volume_musica;
}

void Aumentar_Volume_Musica(){
    if(!gestor_de_audio.mutado_musica){
        Definir_Volume_Musica(gestor_de_audio.volume_musica + VOL_GRAU);
    }
}

void Diminuir_Volume_Musica(){
     if(!gestor_de_audio.mutado_musica){
        Definir_Volume_Musica(gestor_de_audio.volume_musica - VOL_GRAU);
    }
}

void Mutar_Musica(){
   if(gestor_de_audio.mutado_musica){
        //Desmutar
        gestor_de_audio.volume_musica = gestor_de_audio.volume_anterior_musica;
        gestor_de_audio.mutado_musica = false;

        //chama o definir volume apenas a musica estiver realmente tocando
        if(gestor_de_audio.musica_tocando){
            Definir_Volume_Musica(gestor_de_audio.volume_anterior_musica);
        }
        
    } 
    else {
        //Mutar
        gestor_de_audio.volume_anterior_musica = gestor_de_audio.volume_musica;
        gestor_de_audio.mutado_musica = true;
        Definir_Volume_Musica(0.0f);

    }  
}

void TocarMusica(Tipo_Musica tipo) {
   if(audio_iniciado && tipo < TOTAL_MUSICAS) {
        //Atualiza música atual
        gestor_de_audio.musica_atual = tipo;
        
        if(gestor_de_audio.musicas_carregadas[tipo]) {
            PlayMusicStream(gestor_de_audio.musicas[tipo]);
            SetMusicVolume(gestor_de_audio.musicas[tipo], gestor_de_audio.volume_musica);
            gestor_de_audio.musica_tocando = true;
          
        } 
        else{
            gestor_de_audio.musica_tocando = false;
        }
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
    Tipo_Musica atual = gestor_de_audio.musica_atual;

    if(gestor_de_audio.musicas_carregadas[atual]) {
        ResumeMusicStream(gestor_de_audio.musicas[atual]);
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

void Controlar_Volume_Musica(Estados_Jogo *estado) {
    if (IsKeyPressed(KEY_DOWN)) {
        if (barra_selecionada == BARRA_MUSICA) {
            barra_selecionada = BARRA_SONS;
        }
        TocarSom(SOM_SELECIONAR);
    }
    if (IsKeyPressed(KEY_UP)) {
        if (barra_selecionada == BARRA_SONS) {
            barra_selecionada = BARRA_MUSICA;
        }
        TocarSom(SOM_SELECIONAR);
    }

    
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_KP_ADD)) {
        if (barra_selecionada == BARRA_MUSICA) {
            Aumentar_Volume_Musica();
        } else { // BARRA_SONS
            Aumentar_Volume_Sons();
        }
        TocarSom(SOM_SELECIONAR);
    }
    
    
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_KP_SUBTRACT)) {
        if (barra_selecionada == BARRA_MUSICA) {
            Diminuir_Volume_Musica();
        } else { // BARRA_SONS
            Diminuir_Volume_Sons();
        }
        TocarSom(SOM_SELECIONAR);
    }

    
    if (IsKeyPressed(KEY_M)) { 
        if (barra_selecionada == BARRA_MUSICA) {
            Mutar_Musica();
        } else { // BARRA_SONS
            Mutar_Sons();
        }
        TocarSom(SOM_SELECIONAR);
    }

    
    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_P)) {
        Tipo_Musica atual = gestor_de_audio.musica_atual;
        if (gestor_de_audio.musicas_carregadas[atual]) {
            if (IsMusicStreamPlaying(gestor_de_audio.musicas[atual])) {
                PausarMusica();
                gestor_de_audio.musica_tocando = false; // Atualiza nosso flag
            } else {
                RetomarMusica();
                gestor_de_audio.musica_tocando = true; // Atualiza nosso flag
            }
        }
    }
    
    // --- 6. SAIR ---
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)){
        *estado = estado_anterior;
    }
}

void Desenha_Barra_Volume_Musica(){
    ClearBackground((Color){20, 20, 30, 255});
    
    DrawText("REGULADOR DE VOLUME", 
             GetScreenWidth()/2 - MeasureText("REGULADOR DE VOLUME", 40)/2, 
             80, 40, WHITE);
             
    //Cores
    Color corLabelMusica = LIGHTGRAY;
    Color corBordaMusica = WHITE;
    Color corLabelSons = LIGHTGRAY;
    Color corBordaSons = WHITE;

    if(barra_selecionada == BARRA_MUSICA) {
        corLabelMusica = YELLOW;
        corBordaMusica = YELLOW;
    } 
    else{ // BARRA_SONS
        corLabelSons = YELLOW;
        corBordaSons = YELLOW;
    }
    
    //Barra musica
    int barX = 150;
    int barY_musica = 220;
    int barWidth = 500;
    int barHeight = 60;
    
    // Rótulo da barra (Usa cor de destaque)
    DrawText("MÚSICA", 
             barX, 
             barY_musica - 30, 25, corLabelMusica);
    
    // Barra de volume - fundo (Usa cor de destaque)
    DrawRectangle(barX, barY_musica, barWidth, barHeight, (Color){40, 40, 50, 255});
    DrawRectangleLines(barX, barY_musica, barWidth, barHeight, corBordaMusica);
    
    // Barra de volume - preenchimento
    int fillWidth_musica = (int)(barWidth * gestor_de_audio.volume_musica);
    Color fillColor_musica;
    if (gestor_de_audio.volume_musica > 0.7f) fillColor_musica = RED;
    else if (gestor_de_audio.volume_musica > 0.4f) fillColor_musica = ORANGE;
    else fillColor_musica = GREEN;
    DrawRectangle(barX, barY_musica, fillWidth_musica, barHeight, fillColor_musica);
    
    //musica
    char volumeText_musica[16];
    if (gestor_de_audio.mutado_musica) sprintf(volumeText_musica, "MUDO");
    else sprintf(volumeText_musica, "%d%%", (int)(gestor_de_audio.volume_musica * 100));
    DrawText(volumeText_musica, 
             barX + barWidth/2 - MeasureText(volumeText_musica, 30)/2, 
             barY_musica + 15, 30, WHITE);

             
    // --- BARRA DE VOLUME (SONS) ---
    int barY_sons = 370;
    
    // Rótulo da barra (Usa cor de destaque)
    DrawText("SONS (SFX)", 
             barX, 
             barY_sons - 30, 25, corLabelSons);

    // Barra de volume - fundo (Usa cor de destaque)
    DrawRectangle(barX, barY_sons, barWidth, barHeight, (Color){40, 40, 50, 255});
    DrawRectangleLines(barX, barY_sons, barWidth, barHeight, corBordaSons);
    
    // Barra de volume - preenchimento
    int fillWidth_sons = (int)(barWidth * gestor_de_audio.volume_sons);
    Color fillColor_sons;
    if (gestor_de_audio.volume_sons > 0.7f) fillColor_sons = RED;
    else if (gestor_de_audio.volume_sons > 0.4f) fillColor_sons = ORANGE;
    else fillColor_sons = GREEN;
    DrawRectangle(barX, barY_sons, fillWidth_sons, barHeight, fillColor_sons);
    
    //Sons
    char volumeText_sons[16];
    if (gestor_de_audio.mutado_sons) sprintf(volumeText_sons, "MUDO");
    else sprintf(volumeText_sons, "%d%%", (int)(gestor_de_audio.volume_sons * 100));
    DrawText(volumeText_sons, 
             barX + barWidth/2 - MeasureText(volumeText_sons, 30)/2, 
             barY_sons + 15, 30, WHITE);
    
    //Instrucoes
    int instructY = 500;
    DrawText("CONTROLES:", GetScreenWidth()/2 - MeasureText("CONTROLES:", 25)/2, 
             instructY, 25, YELLOW);
    
    instructY += 50;
    DrawText("SETAS (baixo) / (cima)    : Selecionar Barra", 
             GetScreenWidth()/2 - MeasureText("SETAS (baixo) / (cima) : Selecionar Barra", 20)/2, 
             instructY, 20, WHITE);

    instructY += 35;
    DrawText("SETAS <- / -> : Ajustar volume", 
             GetScreenWidth()/2 - MeasureText("SETAS <- / -> : Ajustar volume", 20)/2, 
             instructY, 20, WHITE);
    
    instructY += 35;
    DrawText("ESPAÇO ou P : Tocar/Pausar (Música)", 
             GetScreenWidth()/2 - MeasureText("ESPAÇO ou P : Tocar/Pausar (Música)", 20)/2, 
             instructY, 20, WHITE);
    
    instructY += 35;
    DrawText("M : Mudo (Barra selecionada)", 
             GetScreenWidth()/2 - MeasureText("M : Mudo (Barra selecionada)", 20)/2, 
             instructY, 20, WHITE);
    
    instructY += 35;
    DrawText("ENTER : Sair", 
            GetScreenWidth()/2 - MeasureText("ENTER : Sair", 20)/2, 
            instructY, 20, WHITE);
}

void TocarSom(Tipo_Som tipo){
    if(!audio_iniciado){
        printf("ERRO: Audio não iniciado!\n");
        return;
    }
    
    if(tipo >= TOTAL_SONS){
        printf("ERRO: Tipo de SFX inválido!\n");
        return;
    }
    
    // Verifica se está carregado E se o volume Som não está mudo 
        if(gestor_de_audio.sons_carregados[tipo] && !gestor_de_audio.mutado_sons) {
            PlaySound(gestor_de_audio.sons[tipo]);
        }
}

void Aumentar_Volume_Sons(){
    if(!gestor_de_audio.mutado_sons){
        // Usa Definir_Volume_SFX para limitar e aplicar o volume
        Definir_Volume_Sons(gestor_de_audio.volume_sons + VOL_GRAU);
    }
}

void Diminuir_Volume_Sons(){
    if(!gestor_de_audio.mutado_sons){
        // Usa Definir_Volume_SFX para limitar e aplicar o volume
        Definir_Volume_Sons(gestor_de_audio.volume_sons - VOL_GRAU);
    }
}

void Definir_Volume_Sons(float volume){
    // Garante que o volume esteja no intervalo definido
    if(volume < VOL_MIN){
        volume = VOL_MIN;
    }
    
    if(volume > VOL_MAX){
        volume = VOL_MAX;
    }

    //Salva o novo valor
    gestor_de_audio.volume_sons = volume;
    
    //Aplica o volume Mestre da Raylib (afeta todos os Sounds)
    SetMasterVolume(gestor_de_audio.volume_sons);
}

void Mutar_Sons(){
    if(gestor_de_audio.mutado_sons){
        // Desmutar
        gestor_de_audio.volume_sons = gestor_de_audio.volume_anterior_sons;
        gestor_de_audio.mutado_sons = false;
        Definir_Volume_Sons(gestor_de_audio.volume_sons);
        
    } 
    else {
        // Mutar
        gestor_de_audio.volume_anterior_sons = gestor_de_audio.volume_sons;
        gestor_de_audio.mutado_sons = true;
        Definir_Volume_Sons(0.0f); // Aplica volume zero
    }  
}

float Obter_Volume_Sons(){
    return gestor_de_audio.volume_sons;
}
