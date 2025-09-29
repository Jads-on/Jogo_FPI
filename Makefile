# ===========================================================
#                     CONFIGURAÇÃO DO COMPILADOR
# ===========================================================

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
BUILD_DIR = build

# ===========================================================
#                     ARQUIVOS-FONTE
# ===========================================================

SRC = src/main.c src/jogador.c src/tiros.c
OBJ = $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(SRC))

OUT = Jogo

# Caminho para a Raylib estática
RAYLIB_LIB = ./lib/libraylib.a

# Dependências extras necessárias no Linux (dinâmicas)
LDLIBS = $(RAYLIB_LIB) -lm -lpthread -ldl -lrt -lX11

# ===========================================================
#                     REGRAS
# ===========================================================

# Regra padrão
all: $(OUT)

# Compilação do executável
$(OUT): $(OBJ)
	@echo "🔗 Linking executable: $@"
	$(CC) $(OBJ) $(LDLIBS) -o $(OUT)

# Compilação dos objetos dentro de build/
$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	@echo "🔨 Compiling $< -> $@"
	$(CC) $(CFLAGS) -c $< -o $@

# Garante que a pasta build exista
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Regra para executar o jogo
run: $(OUT)
	@echo "🚀 Running game..."
	./$(OUT)

# Limpeza
clean:
	@echo "🧹 Cleaning up..."
	rm -rf $(BUILD_DIR) $(OUT)

.PHONY: all clean run
