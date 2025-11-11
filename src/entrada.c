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

        dados->N_original=dados->N;
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

void gerarCelula(char* celula, int dificuldade) {
    int r = rand() % 100;
    int monstro;

    switch (dificuldade) {
        case 1: // fácil
            if (r < 45) strcpy(celula, "000");            // 45% vazio
            else if (r < 65) strcpy(celula, "***");       // 20% obstáculo
            else if (r < 80) strcpy(celula, "AAA");       // 15% âncora
            else if (r < 90) strcpy(celula, "BBB");       // 10% bônus regenerativo
            else if (r < 95) strcpy(celula, "zzz");       // 5% um raio atinge Nikador 
            else {                                        // 20% monstros
                monstro = (rand() % 30) + 1;              // força 001–030
                sprintf(celula, "%03d", monstro);
            }
            break;

        case 2: // médio
            if (r < 35) strcpy(celula, "000");            // 35% vazio
            else if (r < 55) strcpy(celula, "***");       // 20% obstáculo
            else if (r < 70) strcpy(celula, "AAA");       // 15% âncora
            else if (r < 80) strcpy(celula, "BBB");       // 10% bônus regenerativo
            else if (r < 85) strcpy(celula, "zzz");       // 5% um raio atinge Nikador
            else {                                        // 30% monstros
                monstro = (rand() % 300) + 1;             // força 001–300
                sprintf(celula, "%03d", monstro);
            }
            break;

        case 3: // difícil
            if (r < 25) strcpy(celula, "000");            // 25% vazio
            else if (r < 45) strcpy(celula, "***");       // 20% obstáculo
            else if (r < 60) strcpy(celula, "AAA");       // 15% âncora
            else if (r < 70) strcpy(celula, "BBB");       // 10% bônus regenerativo
            else if (r < 75) strcpy(celula, "zzz");       // 5% um raio atinge Nikador
            else {                                        // 40% monstros
                monstro = (rand() % 999) + 1;             // força 001–999
                sprintf(celula, "%03d", monstro);
            }
            break;

        default:
            strcpy(celula, "000");
    }
}

char* gerarArquivoEntrada() {
    srand(time(NULL));
    static char nomeArquivo[200];

    int altura, largura, F_inicial, D, N, dificuldade;

    printf("\n--- GERADOR DE ARQUIVOS DE ENTRADA ---\n");

    printf("Nome do arquivo de saida (ex: testes/mapa1.txt): ");
    scanf("%199s", nomeArquivo);

    printf("Altura do mapa: ");
    scanf("%d", &altura);

    printf("Largura do mapa: ");
    scanf("%d", &largura);

    printf("Forca inicial (F): ");
    scanf("%d", &F_inicial);

    printf("Descanso (D): ");
    scanf("%d", &D);

    printf("Forca de Nikador (N): ");
    scanf("%d", &N);

    printf("Dificuldade (1=facil, 2=medio, 3=dificil): ");
    scanf("%d", &dificuldade);

    FILE* arq = fopen(nomeArquivo, "w");
    if (!arq) {
        printf(RED "Erro ao criar o arquivo!\n" RESET);
        return NULL;
    }

    // Cabeçalho
    fprintf(arq, "%d %d %d %d %d\n", altura, largura, F_inicial, D, N);

    // --- GERAÇÃO DO MAPA DO PRESENTE ---
    char*** mapaPresente = malloc(altura * sizeof(char**));
    int** ancora = malloc(altura * sizeof(int*)); // matriz de marcação das âncoras

    for (int i = 0; i < altura; i++) {
        mapaPresente[i] = malloc(largura * sizeof(char*));
        ancora[i] = calloc(largura, sizeof(int)); // inicializa com 0
    }

    char celula[4];
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            mapaPresente[i][j] = malloc(4 * sizeof(char));
            gerarCelula(celula, dificuldade);
            strcpy(mapaPresente[i][j], celula);

            if (strcmp(celula, "AAA") == 0)
                ancora[i][j] = 1; // marca posição de âncora

            fprintf(arq, "%s ", celula);
        }
        fprintf(arq, "\n");
    }

    // --- SEPARADOR ---
    fprintf(arq, "///\n");

    // --- GERAÇÃO DO MAPA DO PASSADO ---
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            if (ancora[i][j]) {
                // se havia âncora no presente, repete no passado
                strcpy(celula, "AAA");
            } else {
                gerarCelula(celula, dificuldade);
                // se gerou uma âncora nova no passado, ignora (mantém coerência)
                if (strcmp(celula, "AAA") == 0)
                    strcpy(celula, "000");
            }
            fprintf(arq, "%s ", celula);
        }
        fprintf(arq, "\n");
    }

    fclose(arq);

    // Libera memória auxiliar
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++)
            free(mapaPresente[i][j]);
        free(mapaPresente[i]);
        free(ancora[i]);
    }
    free(mapaPresente);
    free(ancora);

    printf(GREEN "\nArquivo '%s' gerado com sucesso!\n" RESET, nomeArquivo);
    return nomeArquivo;
}
