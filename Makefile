# Nome do executável e do arquivo de código
TARGET = meu_jogo
SRC = main.c

# Compilador e flags de compilação
CC = gcc
CFLAGS = -Iinclude -std=c99 -Wall -g
LDFLAGS = -Llib -L/usr/lib -L/usr/local/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
#LDFLAGS = -Llib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 (linha original)

# Caminhos do projeto
BUILD_DIR = build
SRC_DIR = .

# Regra para compilar e linkar
$(BUILD_DIR)/$(TARGET): $(SRC_DIR)/$(SRC)
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(SRC_DIR)/$(SRC) -o $(BUILD_DIR)/$(TARGET) $(CFLAGS) $(LDFLAGS)

# Regra para rodar o executável
run: $(BUILD_DIR)/$(TARGET)
	@$(BUILD_DIR)/$(TARGET)

# Regra para limpar os arquivos gerados
clean:
	@rm -rf $(BUILD_DIR)

.PHONY: all run clean