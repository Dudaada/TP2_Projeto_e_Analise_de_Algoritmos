#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/entrada.h"

DadosEntrada* lerArquivo(const char* nomeArquivo) {
    FILE* arq = fopen(nomeArquivo, "r");
    if (!arq) {
        printf(RED "Erro ao abrir o arquivo: %s\n" RESET, nomeArquivo);
        return NULL;
    }

    DadosEntrada* dados = malloc(sizeof(DadosEntrada));
    if (!dados) {
        printf(RED "Erro de alocacao de memoria!\n" RESET);
        fclose(arq);
        return NULL;
    }

    //lê cabeçalho: altura largura F_inicial D N
    fscanf(arq, "%d %d %d %d %d", 
        &dados->altura, &dados->largura, 
        &dados->F_inicial, &dados->D, &dados->N);

    dados->mapas = malloc(2 * sizeof(char***));
    //aloca espaço para dois mapas: presente e passado
    for (int t = 0; t < 2; t++) {
        // Aloca o Nível 3 (ponteiro para as linhas)
        dados->mapas[t] = malloc(dados->altura * sizeof(char**));

        // Aloca o Nível 2 (ponteiro para as colunas)
        for (int i = 0; i < dados->altura; i++) {
            dados->mapas[t][i] = malloc(dados->largura * sizeof(char*));
        }
    }

    //lê mapa do presente
    for (int i = 0; i < dados->altura; i++) {
        for (int j = 0; j < dados->largura; j++) {
            dados->mapas[PRESENTE][i][j] = malloc(4 * sizeof(char));
            fscanf(arq, "%s", dados->mapas[PRESENTE][i][j]);
        }
    }

    //lê linha "///"
    char separador[4];
    fscanf(arq, "%s", separador);

    //lê mapa do passado
    for (int i = 0; i < dados->altura; i++) {
        for (int j = 0; j < dados->largura; j++) {
            dados->mapas[PASSADO][i][j] = malloc(4 * sizeof(char));
            fscanf(arq, "%s", dados->mapas[PASSADO][i][j]);
        }
    }

    fclose(arq);
    return dados;
}

void imprimirMapas(DadosEntrada* d) {
    printf(CYAN "\n=== DADOS LIDOS ===\n" RESET);
    printf("Altura: %d | Largura: %d\n", d->altura, d->largura);
    printf("Forca inicial (F): %d | Descanso (D): %d | Forca de Nikador (N): %d\n", 
           d->F_inicial, d->D, d->N);

    const char* nomes[] = {"Mapa do Presente", "Mapa do Passado"};
    for (int t = 0; t < 2; t++) {
        printf(MAGENTA "\n--- %s ---\n" RESET, nomes[t]);
        for (int i = 0; i < d->altura; i++) {
            for (int j = 0; j < d->largura; j++) {
                char* cel = d->mapas[t][i][j];
                if (strcmp(cel, "***") == 0)
                    printf(RED "*** " RESET);
                else if (strcmp(cel, "000") == 0)
                    printf(GREEN "000 " RESET);
                else if (strcmp(cel, "AAA") == 0)
                    printf(YELLOW "AAA " RESET);
                else
                    printf(WHITE "%s " RESET, cel);
            }
            printf("\n");
        }
    }
}

// CORREÇÃO: Libera todos os 4 níveis
void liberarDados(DadosEntrada* d) {
    if (!d) return;

    for (int t = 0; t < 2; t++) {
        for (int i = 0; i < d->altura; i++) {
            // Nível 1: Libera as strings ("AAA", "010", etc.)
            for (int j = 0; j < d->largura; j++) {
                free(d->mapas[t][i][j]);
            }
            // Nível 2: Libera o array de colunas
            free(d->mapas[t][i]);
        }
        // Nível 3: Libera o array de linhas
        free(d->mapas[t]);
    }
    // Nível 4: Libera o array de mapas
    free(d->mapas);
    // Libera a struct principal
    free(d);
}
