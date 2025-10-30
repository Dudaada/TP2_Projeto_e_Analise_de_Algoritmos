#include <stdio.h>
#include "../include/entrada.h"
#include "../include/Caminho.h"

int main() {
    const char* nomeArquivo = "../testes/mapa1.txt";
    DadosEntrada* d = lerArquivo(nomeArquivo);


    if (d) {

        imprimirMapas(d);


        calcularCaminho(d);

        liberarDados(d);
    }

    return 0;
}