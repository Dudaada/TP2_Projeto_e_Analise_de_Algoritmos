#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/entrada.h"
#include "../include/Caminho.h"

int benchmark =0;

int main(int argc, char *argv[]) {
    DadosEntrada* mapa_atual = NULL;
    srand(time(NULL));
    char nomeArquivoCarregado[200];
    int opcao = 0;

    if (argc > 1) {
        //o usuário passou um arquivo
        printf(CYAN "Modo de execucao direta...\n" RESET);
        printf("Carregando mapa de: %s\n", argv[1]);

        mapa_atual = lerArquivo(argv[1]); 

        if (mapa_atual == NULL) {
            printf(RED "Falha ao carregar o mapa. Encerrando.\n" RESET);
            return 1; 
        }

        // Se carregou, calcula o caminho imediatamente
        printf(CYAN "\n--- Calculando melhor caminho... ---\n" RESET);
        calcularCaminho(mapa_atual);

        liberarDados(mapa_atual);
        return 0; 

    }

    else {

        printf(CYAN "\n=== Bem-vindo(a) a Jornada contra Nikador! ===\n" RESET);
        printf("Prepare-se para carregar o mapa e iniciar sua aventura.\n");

        do {
            printf(MAGENTA "\n===== MENU PRINCIPAL =====\n" RESET);
            printf("1. Carregar mapa\n");
            printf("2. Gerar mapa automaticamente\n");
            printf("3. Exibir mapa atual\n");
            printf("4. Calcular melhor caminho\n");
            printf("5. Modo Benchmark\n");
            printf("0. Sair\n");
            printf("Escolha uma opcao: ");
            fflush(stdout);
            

            char buffer_input[100];
            if (fgets(buffer_input, sizeof(buffer_input), stdin) != NULL) {
                sscanf(buffer_input, "%d", &opcao);
            } else {
                opcao = 0; 
            }

            switch (opcao) {
                case 1: {
                    printf("\n--- Carregar Mapa ---\n");
                    printf("Digite o nome do arquivo de entrada (ex: testes/mapa1.txt): ");
                    fflush(stdout);

                    if (fgets(nomeArquivoCarregado, sizeof(nomeArquivoCarregado), stdin) != NULL) {
                        // Remove o '\n' que o fgets captura
                        nomeArquivoCarregado[strcspn(nomeArquivoCarregado, "\n")] = 0; 
                    }


                    if (mapa_atual != NULL) {
                        liberarDados(mapa_atual);
                        mapa_atual = NULL;
                    }

                    mapa_atual = lerArquivo(nomeArquivoCarregado);

                    if (mapa_atual != NULL) {
                        printf(GREEN "\nMapa '%s' carregado com sucesso!\n" RESET, nomeArquivoCarregado);
                        imprimirMapas(mapa_atual);
                    }

                    printf("\nPressione Enter para voltar ao menu principal...");
                    fflush(stdout); 
                    getchar(); 
                    break;
                }


                case 2: {
                    printf("\n--- Gerar Mapa Automaticamente ---\n");
                    char* arquivoGerado = gerarArquivoEntrada(); // chama função e recebe nome

                    if (arquivoGerado != NULL) {
                        strncpy(nomeArquivoCarregado, arquivoGerado, sizeof(nomeArquivoCarregado) - 1);
                        nomeArquivoCarregado[sizeof(nomeArquivoCarregado) - 1] = '\0'; // Garante terminação nula

                        if (mapa_atual != NULL) {
                            liberarDados(mapa_atual);
                            mapa_atual = NULL;
                        }

                        mapa_atual = lerArquivo(arquivoGerado);

                        if (mapa_atual != NULL) {
                            printf(GREEN "\nMapa '%s' carregado automaticamente!\n" RESET, arquivoGerado);
                            imprimirMapas(mapa_atual);
                        }
                    }

                    printf("\nPressione Enter para voltar ao menu principal...");
                    fflush(stdout);

                    int c;
                    while ((c = getchar()) != '\n' && c != EOF); // Limpa o '\n' sujo
                    getchar(); 
                    break;
            }

                case 3: {
                    if (mapa_atual != NULL) {
                        imprimirMapas(mapa_atual);
                    } else {
                        printf(RED "\nNenhum mapa carregado ainda!\n" RESET);
                    }

                    printf("\nPressione Enter para voltar ao menu principal...");
                    fflush(stdout); 
                    getchar(); 
                    break;
                
                }

                case 4: {
                    if (mapa_atual == NULL) {
                        printf(RED "\nNenhum mapa carregado! Use a opcao 1 ou 2 primeiro.\n" RESET);
                    } else {
                        printf(CYAN "\n--- Calculando melhor caminho... ---\n" RESET);
                        calcularCaminho(mapa_atual);
                    }

                    printf("\nPressione Enter para voltar ao menu principal...");
                    fflush(stdout); 
                    getchar(); 
                    break;
                }

                case 5: {
                    int qntTestes;
                    int arq;
                    benchmark=1;
                    printf("Modo benchmark\n");
                    printf("Quantos tamanhos mapas deseja verificar?\n");
                    scanf("%d",&qntTestes);
                    getchar();
                    
                    FILE* arquivo_resultados = fopen("benchmark.txt", "a");
                    if (arquivo_resultados == NULL) {
                        printf(RED "Erro ao criar arquivo de resultados!\n" RESET);
                        break;
                    }

                    fprintf(arquivo_resultados, "Dimensao - (largura e altura)\tTempo\n");

                    for (int i = 0; i < qntTestes; i++) {
                        printf("Teste numero %d\n",i+1);
                        printf("1. Arquivo existente\n2. Gerar arquivo\n");
                        scanf("%d",&arq);
                        getchar(); // Limpa o buffer

                        char nomeArquivo[200];
                        DadosEntrada* mapa_teste = NULL;

                        if (arq==1){
                            printf("Digite o nome do arquivo: ");
                            fflush(stdout);

                            if (fgets(nomeArquivoCarregado, sizeof(nomeArquivoCarregado), stdin) != NULL) {
                                // Remove o '\n' que o fgets captura
                                nomeArquivoCarregado[strcspn(nomeArquivoCarregado, "\n")] = 0; 
                            }


                            if (mapa_teste != NULL) {
                                liberarDados(mapa_teste);
                                mapa_teste = NULL;
                            }

                            mapa_teste = lerArquivo(nomeArquivoCarregado);

                        }else if (arq==2){
                            char* arquivoGerado = gerarArquivoEntrada(); // chama função e recebe nome

                            if (arquivoGerado != NULL) {
                                strncpy(nomeArquivoCarregado, arquivoGerado, sizeof(nomeArquivoCarregado) - 1);
                                nomeArquivoCarregado[sizeof(nomeArquivoCarregado) - 1] = '\0'; // Garante terminação nula

                                if (mapa_teste != NULL) {
                                    liberarDados(mapa_teste);
                                    mapa_teste = NULL;
                                }

                                mapa_teste = lerArquivo(arquivoGerado);
                            }
                        }else{
                            printf(RED "Opcaoo invalida!\n" RESET);
                            continue;
                        }
                        if (mapa_teste == NULL) {
                            printf(RED "Erro\n" RESET);
                            continue;
                        }
                    
                        clock_t inicio = clock();
                        calcularCaminho(mapa_teste);
                        clock_t fim = clock();
                        
                        double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

                        fprintf(arquivo_resultados, "%dx%d\t%.6f\n",  
                                mapa_teste->largura, 
                                mapa_teste->altura, 
                                tempo);
                        fflush(arquivo_resultados);


                        printf(CYAN "\nTempo de execucao: %.6f seg\n" RESET, tempo);

                        liberarDados(mapa_teste);

                    }
                        fclose(arquivo_resultados);

                        printf("\nPressione Enter para voltar ao menu principal...");
                        fflush(stdout); 
                        getchar();
                        benchmark=0; 
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
    }

    return 0;
}
