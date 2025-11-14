# === CONFIGURAÇÃO DO COMPILADOR ===

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
BUILD_DIR = build

#  === ARQUIVOS-FONTE ===

SRC = src/camera.c src/colisoes.c src/creditos.c src/fase_1.c src/gameover.c src/gestor_audio.c src/gestor_fases.c src/historia.c src/inimigos.c src/jogador.c src/main.c src/menu.c src/spawn.c src/tiros.c     
OBJ = $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(SRC))
OUT = SYNTHETIC

# Caminho para a Raylib estática
RAYLIB_LIB = ./lib/libraylib.a

# Dependências extras necessárias no Linux (dinâmicas)
LDLIBS = $(RAYLIB_LIB) -lm -lpthread -ldl -lrt -lX11

# === REGRAS ===

# Regra padrão
all: $(OUT)

# Compilação do executável
$(OUT): $(OBJ)
	$(CC) $(OBJ) $(LDLIBS) -o $(OUT)

# Compilação dos objetos dentro de build/
$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Garante que a pasta build exista
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Regra para executar o jogo
run: $(OUT)
	./$(OUT)

# Limpeza
clean:
	rm -rf $(BUILD_DIR) $(OUT)

.PHONY: all clean run
