# Compilador e flags
CC = gcc
CFLAGS = -Wall -g

# Pastas
INCLUDE = include
SRC = src
TESTE = testes

# Arquivos
OBJS = $(SRC)/entrada.o main.o
EXEC = tp2

# Regra padrão
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -I$(INCLUDE) $(OBJS) -o $(EXEC)

$(SRC)/entrada.o: $(SRC)/entrada.c $(INCLUDE)/entrada.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $(SRC)/entrada.c -o $(SRC)/entrada.o

main.o: main.c $(INCLUDE)/entrada.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c main.c -o main.o

# Executa com um arquivo de teste
# Exemplo: make run ARQ=testes/mapa1.txt
run: all
	./$(EXEC) $(ARQ)

# Limpa tudo
clean:
	rm -f $(SRC)/*.o *.o $(EXEC)
