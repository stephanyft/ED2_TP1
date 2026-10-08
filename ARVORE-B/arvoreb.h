#ifndef ARVOREB_H
#define ARVOREB_H

#include <stdio.h>
#include <stdlib.h>
#include "../item.h"

#define TRUE 1
#define FALSE 0

#define M 2
#define MM 2 * M

typedef int TipoChave;

typedef struct TipoRegistro {
    TipoChave Chave;
    long posicaoNoArquivo;
}TipoRegistro;

typedef struct TipoPagina* TipoApontador;

typedef struct TipoPagina {
    short n;
    TipoRegistro r[MM];
    TipoApontador p[MM + 1];
} TipoPagina;

void inicializa(TipoApontador* Arvore);
int Pesquisa(TipoRegistro *x, TipoApontador Ap);
void Insere(TipoRegistro Reg, TipoApontador* Ap);
void Ins(TipoRegistro Reg, TipoApontador Ap, short *Cresceu, TipoRegistro *RegRetorno, TipoApontador *ApRetorno);
void InsereNaPagina(TipoApontador Ap, TipoRegistro Reg, TipoApontador ApDir);
void Imprime(TipoApontador arvore);
void CriaArvore(FILE * arq, TipoApontador *arvore, int *comparacoesCriacao, int *transferenciasCriacao);
void executarMetodo3(int quantidade, int situacao, int chaveBusca, int imprimir, const char *nomeArquivo, AnaliseExperimental *analise);

#endif