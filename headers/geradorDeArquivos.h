#ifndef GERADOR_DE_ARQUIVOS_H
#define GERADOR_DE_ARQUIVOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../item.h"

void gerarString(char *str, int tamanho);
void embaralhar(int *vetor, int n);
int gerarArquivo(int quantReg, int situacao, const char *nomeArq);
void mostraArquivo(const char *nomeArq);
void sortearChaves(const char *nomeArq, int quantidade, int *chaves, int numChaves);

#endif