#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "../include/Caminho.h"

#define INFINITO -999999999
//altura numero de linhas
//largura numero de colunas
//A Processar saida usa a melhor força que já temos para calcular a força que teriamos se nos movêssemos para a próxima célula
void ProcessarSaida(DadosEntrada* mapas, int altura, int largura,int mapa_Max[][altura][largura], int linhaAnterior[][altura][largura],int mapaAnterior[][altura][largura], int mapaAtual, int linhaAtual, int colunaAtual) {

    //pegamos a força atual
    int forcaAtual = mapa_Max[mapaAtual][linhaAtual][colunaAtual];

    // Se a força é INFINITO, significa que nunca chegamos aqui.
    if (forcaAtual == INFINITO) {
       return; //este caminho não existe.
    }

    //vamos ver se vamos para outro mapa
    char* celulaAtual = mapas->mapas[mapaAtual][linhaAtual][colunaAtual];
    int mapaProximo;

    if (strcmp(celulaAtual, "AAA") == 0) {
        mapaProximo = 1 - mapaAtual; // Troca o mapa (de 0 vai para 1, de 1 vai para 0)
    } else {
        mapaProximo = mapaAtual;     // Permanece no mesmo mapa
    }

    // Loop 'dy' vai de -1 (cima), 0 (frente), +1 (baixo)
    for (int dy = -1; dy <= 1; dy++) {

        int linhaProxima = linhaAtual + dy;
        int colunaProxima = colunaAtual + 1; //sempre para a proxima coluna

        //Verifique os Limites
        if (linhaProxima < 0 || linhaProxima >= mapas->altura) {
            continue;
        }

        //Verifique a Célula de DESTINO
        char* celulaDestino = mapas->mapas[mapaProximo][linhaProxima][colunaProxima];
        //verifica se e parede
        if (strcmp(celulaDestino, "***") == 0) {
            continue;
        }

        //se nao calculamos o custo

        int custoGanho = 0;
        if (strcmp(celulaDestino, "000") == 0) {
            custoGanho = mapas->D;
        } else if (strcmp(celulaDestino, "AAA") == 0) {
            custoGanho = 0;
        } else {
            // É inimigo, custo negativo pois é uma perda
            char *ptr;
            custoGanho = -strtol(celulaDestino, &ptr, 10);
        }


        int forcaNova = forcaAtual + custoGanho;


        // Se a força que acabamos de calcular (forcaNova)
        // é MELHOR do que a força que já estava em mapaMax...
        if (forcaNova > mapa_Max[mapaProximo][linhaProxima][colunaProxima]) {

            mapa_Max[mapaProximo][linhaProxima][colunaProxima] = forcaNova;
            // ...E guardamos o caminho!
            // Dizemos: "Para chegar em (mapaProximo, linhaProxima, colunaProxima),
            // nós viemos de (mapaAtual, linhaAtual)".
            linhaAnterior[mapaProximo][linhaProxima][colunaProxima] = linhaAtual;
            mapaAnterior[mapaProximo][linhaProxima][colunaProxima] = mapaAtual;
        }
    }

}
void calcularCaminho(DadosEntrada* mapas){
    //Inicializa e preenche com ifinito
    int mapa_Max[2][mapas->altura][mapas->largura];

    for(int j=0;j<mapas->altura;j++){
        for(int k=0;k<mapas->largura;k++){
            mapa_Max[PRESENTE][j][k]= INFINITO;
        }
    }
    for(int j=0;j<mapas->altura;j++){
       for(int k=0;k<mapas->largura;k++){
            mapa_Max[PASSADO][j][k]= INFINITO;
       }
    }

    // matrizes para guardar o caminho
    int linhaAnterior[2][mapas->altura][mapas->largura];
    int mapaAnterior[2][mapas->altura][mapas->largura];

    for(int k=0; k<2; k++){
        for(int j=0; j<mapas->altura; j++){
            for(int i=0; i<mapas->largura; i++){
                linhaAnterior[k][j][i] = -1;
                mapaAnterior[k][j][i] = -1;
            }
        }
    }

    //Calculando coluna 0
    for(int j=0;j<mapas->altura;j++){
        if(strcmp(mapas->mapas[PRESENTE][j][0], "000") == 0){
            mapa_Max[PRESENTE][j][0] = mapas->F_inicial + mapas->D;
        }
        else if(strcmp(mapas->mapas[PRESENTE][j][0], "***") == 0){
            mapa_Max[PRESENTE][j][0] = INFINITO;
        }
        else if(strcmp(mapas->mapas[PRESENTE][j][0], "AAA") == 0){
            // A força "aterrissa" no mapa PASSADO
            mapa_Max[PASSADO][j][0] = mapas->F_inicial;
            // O mapa PRESENTE continua "impossível"
            mapa_Max[PRESENTE][j][0] = INFINITO;
        }
        else{
            char *ptr;
            int inimigos = strtol(mapas->mapas[PRESENTE][j][0], &ptr, 10);

            mapa_Max[PRESENTE][j][0] = mapas->F_inicial-inimigos;
        }
    }
    //calculo principal !!!

    for (int x = 0; x < mapas->largura - 1; x++) {
        for (int y = 0; y < mapas->altura; y++) {
            //Processa a saída do mapa PRESENTE
            ProcessarSaida(mapas, mapas->altura, mapas->largura, mapa_Max, linhaAnterior, mapaAnterior, PRESENTE, y, x);
            //processa a saída do mapa PASSADO
            ProcessarSaida(mapas, mapas->altura, mapas->largura, mapa_Max, linhaAnterior, mapaAnterior, PASSADO, y, x);

        }
    }

    //Parte final encontra a forcaFinal sua localização

    int forcaFinal = INFINITO;//se continuar INFINITO  significa que não tem caminho
    int linhaFinal = -1; // Guarda a linha (y) onde o melhor caminho terminou
    int mapaFinal = -1;  // Guarda o mapa (k) onde o melhor caminho terminou
    int colunaFinal = mapas->largura - 1;//guarda a coluna final

    for (int y = 0; y < mapas->altura; y++) {

        // Verifica o mapa PRESENTE
        if (mapa_Max[PRESENTE][y][mapas->largura - 1] > forcaFinal) {
            forcaFinal = mapa_Max[PRESENTE][y][mapas->largura - 1];
            linhaFinal = y;
            mapaFinal = PRESENTE;
        }

        // Verifica o mapa PASSADO
        if (mapa_Max[PASSADO][y][mapas->largura - 1] > forcaFinal) {
            forcaFinal = mapa_Max[PASSADO][y][mapas->largura - 1];
            linhaFinal = y;
            mapaFinal = PASSADO;
        }
    }

    RespostaFinal(forcaFinal, linhaFinal, mapaFinal, colunaFinal, mapas, mapas->altura, mapas->largura,linhaAnterior, mapaAnterior);
    printf("Valor de F quando chegou: %d\n",forcaFinal);
}

//-------Reconstrói o caminho e o imprime------

void ImprimirCaminho(DadosEntrada* mapas, int altura, int largura,int linhaAnterior[][altura][largura],
                    int mapaAnterior[][altura][largura],  int mapaFinal, int linhaFinal, int colunaFinal) {


    // array temporario para guarda o caminho
    int caminhoLinhas[mapas->largura];
    int caminhoMapas[mapas->largura]; // guarda o mapa (0 ou 1)


    // Reconstroi o caminho de tras para frente
    int mapaAtual = mapaFinal;
    int linhaAtual = linhaFinal;

    // Loop das colunas, de trás para frente
    for (int x = colunaFinal; x >= 0; x--) {

        caminhoLinhas[x] = linhaAtual;
        caminhoMapas[x] = mapaAtual; // mapa guardado
        
        if (x > 0) {
            int k_ant = mapaAnterior[mapaAtual][linhaAtual][x];
            int y_ant = linhaAnterior[mapaAtual][linhaAtual][x];
            mapaAtual = k_ant;
            linhaAtual = y_ant;
        }
    }
   // Coordenadas)
    printf(CYAN "\n--- Coordenadas do Caminho ---\n" RESET);
    for (int x = 0; x <= colunaFinal; x++) {
        // Formato: (linha) (coluna)
        printf("%d %d\n", caminhoLinhas[x], x);
    }
    
    // extra 2: Imprime o gráfico do caminho
    imprimirGrafico(mapas, caminhoLinhas, caminhoMapas, largura);
}

void RespostaFinal(int forcaFinal, int linhaFinal, int mapaFinal, int colunaFinal, DadosEntrada* mapas,
                   int altura, int largura,int linhaAnterior[][altura][largura], int mapaAnterior[][altura][largura]) {

    if(forcaFinal == INFINITO || forcaFinal <= 0){
        printf("A calamidade de Nikador é inevitável\n");
    }
    else {
        ImprimirCaminho(mapas, altura, largura, linhaAnterior, mapaAnterior, mapaFinal, linhaFinal, colunaFinal);
        if(forcaFinal >= mapas->N){
            printf("A ruína de Nikador é iminente\n");
        }
        else {
            printf("Será necessário mais planejamento para parar a calamidade\n");
        }
    }
}

/* * Nova funçao para o extra 2
 * Imprime os mapas com o caminho destacado em azul.
 */

void imprimirGrafico(DadosEntrada* mapas, int* caminhoLinhas, int* caminhoMapas, int largura) {
    
    const char* nomes[] = {"Mapa do Presente", "Mapa do Passado"};

    //(t = 0 é Presente, t = 1 é Passado)
    for (int t = 0; t < 2; t++) {
        printf(MAGENTA "\n--- %s (Caminho Gráfico) ---\n" RESET, nomes[t]);

        for (int i = 0; i < mapas->altura; i++) {
            
            for (int j = 0; j < largura; j++) {
                
                char* cel = mapas->mapas[t][i][j];
                
                // O caminho está neste mapa (t) E nesta linha (i) E nesta coluna (j)?
                if (caminhoMapas[j] == t && caminhoLinhas[j] == i) {
                    
                    // Se sim, imprime destacado em azul
                    printf(BLUE "[%s]" RESET, cel);

                } else {
                    
                    // Se não, imprime normalmente
                    if (strcmp(cel, "***") == 0)
                        printf(RED " %s " RESET, cel);
                    else if (strcmp(cel, "000") == 0)
                        printf(GREEN " %s " RESET, cel);
                    else if (strcmp(cel, "AAA") == 0)
                        printf(YELLOW " %s " RESET, cel);
                    else
                        printf(WHITE " %s " RESET, cel);
                }
            }
            printf("\n"); 
        }
    }
}