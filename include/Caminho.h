#ifndef CAMINHO_H
#define CAMINHO_H

#include "entrada.h"

void ProcessarSaida(DadosEntrada* mapas, int altura, int largura,int mapa_Max[][altura][largura], int linhaAnterior[][altura][largura],int mapaAnterior[][altura][largura], int mapaAtual, int linhaAtual, int colunaAtual) ;
void calcularCaminho(DadosEntrada* mapas);
void ImprimirCaminho(DadosEntrada* mapas, int altura, int largura,int linhaAnterior[][altura][largura],
                    int mapaAnterior[][altura][largura],  int mapaFinal, int linhaFinal, int colunaFinal);
void RespostaFinal(int forcaFinal, int linhaFinal, int mapaFinal, int colunaFinal, DadosEntrada* mapas, int altura, int largura,int linhaAnterior[][altura][largura], int mapaAnterior[][altura][largura]);
#endif