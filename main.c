#include <stdio.h>
#include "../include/entrada.h"

//main bobo pra testar leitura de arquivos, makefile tb
int main() {
    const char* nomeArquivo = "testes/mapa1.txt"; 
    DadosEntrada* d = lerArquivo(nomeArquivo);

    if (d) {
        imprimirMapas(d);
        liberarDados(d);
    }
    return 0;
}
