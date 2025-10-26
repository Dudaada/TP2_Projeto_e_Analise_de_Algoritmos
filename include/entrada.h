#ifndef ENTRADA_H
#define ENTRADA_H

// Códigos ANSI de cor
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define GRAY    "\033[90m"

typedef struct {
    int altura; //número de linhas
    int largura; //número de colunas
    int F_inicial; //força inicial
    int D; //descanso
    int N; //força de Nikador
    char ***mapas; // mapas[2][altura][largura*4] (presente e passado)
} DadosEntrada;

//coloquei pra definir os mapas de presente e passado (sla se é a melhor forma)
typedef enum {
    PRESENTE = 0,
    PASSADO = 1
} Tempo;

DadosEntrada* lerArquivo(const char* nomeArquivo);
void liberarDados(DadosEntrada* d);
void imprimirMapas(DadosEntrada* d);

#endif
