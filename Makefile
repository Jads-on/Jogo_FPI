# Define as pastas de origem
SRC_DIR = src
INC_DIR = include

# Nomes dos arquivos-fonte (sem o caminho)
SRC_FILES_NAMES = main.c jogador.c tiros.c

# Constrói o caminho completo para os arquivos-fonte
SRC_FILES = $(addprefix $(SRC_DIR)/, $(SRC_FILES_NAMES))

# Define os arquivos objeto (o que é gerado depois da compilação)
OBJ_FILES = $(SRC_FILES_NAMES:.c=.o)

# Outras configurações
CC = gcc
CFLAGS = -Wall -std=c99 -I$(INC_DIR) # <- ADICIONADO: diz para o compilador procurar cabeçalhos na pasta Headers
LDFLAGS = -lraylib -lm

TARGET = meu_jogo

all: $(TARGET)

# Regra para vincular os arquivos objeto em um executável
$(TARGET): $(OBJ_FILES)
	$(CC) $^ -o $@ $(LDFLAGS)

# Regra para compilar cada arquivo .c em seu respectivo .o
# O $< é uma variável automática que se refere ao primeiro pré-requisito
# O $@ se refere ao nome do alvo
%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJ_FILES)