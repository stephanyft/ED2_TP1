#ifndef ARVOREB_H
#define ARVOREB_H

#include <stdio.h>
#include <stdlib.h>
#include "../item.h"

#define TRUE 1
#define FALSE 0

// Define o tipo da chave de pesquisa //
typedef int TipoChave;

// Estrutura do item indexado na memoria principal //
typedef struct TipoRegistro {
    TipoChave Chave;
    long posicaoNoArquivo;
}TipoRegistro;

typedef struct TipoPagina* TipoApontador;

// Estrutura de uma pagina (no) da Arvore B. //
typedef struct TipoPagina {
    short n;
    TipoRegistro r[MM];
    TipoApontador p[MM + 1];
} TipoPagina;

void inicializa(TipoApontador* Arvore);
int Pesquisa(TipoRegistro *x, TipoApontador Ap, AnaliseExperimental *metricas);
void Insere(TipoRegistro Reg, TipoApontador* Ap, AnaliseExperimental *metricas);
void Ins(TipoRegistro Reg, TipoApontador Ap, short *Cresceu, TipoRegistro *RegRetorno, TipoApontador *ApRetorno, AnaliseExperimental *metricas);
void InsereNaPagina(TipoApontador Ap, TipoRegistro Reg, TipoApontador ApDir, AnaliseExperimental *metricas);
void Imprime(TipoApontador arvore);
void CriaArvore(FILE * arq, TipoApontador *arvore, AnaliseExperimental *analise);
void executarMetodo3(int quantidade, int situacao, int chaveBusca, int imprimir, const char *nomeArquivo, AnaliseExperimental *analise);

#endif