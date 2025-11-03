# Compilador e flags
CC = gcc
CFLAGS = -Wall -Iinclude

# Diretórios
SRC_DIR = src
OBJ_DIR = $(SRC_DIR)
BIN = programa

# Arquivos fonte e objetos
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/entrada.c $(SRC_DIR)/Caminho.c
OBJS = $(SRCS:.c=.o)

# Regra padrão
all: $(BIN)

# Linkagem final
$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS)

# Compilação dos .c
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Executar com arquivo de teste
run: $(BIN)
	./$(BIN) testes/mapa1.txt

# Limpar arquivos compilados
clean:
	rm -f $(OBJ_DIR)/*.o $(BIN)
