#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/entrada.h"
#include "../include/Caminho.h"

int main() {
    DadosEntrada* mapa_atual = NULL;
    char nomeArquivoCarregado[200];
    int opcao = 0;

    printf(CYAN "\n=== Bem-vindo(a) a Jornada contra Nikador! ===\n" RESET);
    printf("Prepare-se para carregar o mapa e iniciar sua aventura.\n");

    do {
        printf(MAGENTA "\n===== MENU PRINCIPAL =====\n" RESET);
        printf("1. Carregar mapa\n");
        printf("2. Exibir mapa atual\n");
        printf("3. Calcular melhor caminho\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // consome o '\n' após o número

        switch (opcao) {
            case 1: {
                printf("\n--- Carregar Mapa ---\n");
                printf("Digite o nome do arquivo de entrada (ex: testes/mapa1.txt): ");
                scanf("%199s", nomeArquivoCarregado);

                // Se já existir um mapa carregado, libera antes de carregar um novo
                if (mapa_atual != NULL) {
                    liberarDados(mapa_atual);
                    mapa_atual = NULL;
                }

                mapa_atual = lerArquivo(nomeArquivoCarregado);

                if (mapa_atual != NULL) {
                    printf(GREEN "\nMapa '%s' carregado com sucesso!\n" RESET, nomeArquivoCarregado);
                    imprimirMapas(mapa_atual); // Mostra o mapa para o usuário confirmar
                }

                printf("\nPressione Enter para voltar ao menu principal...");
                while (getchar() != '\n');
                break;
            }

            case 2: {
                if (mapa_atual != NULL) {
                    imprimirMapas(mapa_atual);
                } else {
                    printf(RED "\nNenhum mapa carregado ainda!\n" RESET);
                }

                printf("\nPressione Enter para voltar ao menu principal...");
                while (getchar() != '\n');
                break;
            }

            case 3: {
                if (mapa_atual == NULL) {
                    printf(RED "\nNenhum mapa carregado! Use a opcao 1 primeiro.\n" RESET);
                } else {
                    printf(CYAN "\n--- Calculando melhor caminho... ---\n" RESET);
                    calcularCaminho(mapa_atual);
                }

                printf("\nPressione Enter para voltar ao menu principal...");
                while (getchar() != '\n');
                break;
            }

            case 0:
                printf(YELLOW "\nEncerrando o programa... Ate a proxima jornada!\n" RESET);
                break;

            default:
                printf(RED "\nOpcao inválida! Tente novamente.\n" RESET);
        }

    } while (opcao != 0);

    if (mapa_atual != NULL)
        liberarDados(mapa_atual);

    return 0;
}
