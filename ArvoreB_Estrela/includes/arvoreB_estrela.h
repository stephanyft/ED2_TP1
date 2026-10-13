#ifndef ARVOREB_ESTRELA_H
#define ARVOREB_ESTRELA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "item.h"

#define M 4
#define MM (2 * M)

/* Entrada do índice da árvore B* */
typedef struct {
    int chave;
    long posicaoNoArquivo;
} RegistroIndiceBEstrela;

typedef enum {
    Interna,
    Externa
} TipoIntExt;

typedef struct TipoPagina *TipoApontador;

typedef struct TipoPagina {
    TipoIntExt Pt;

    union {
        /* Página interna */
        struct {
            int ni;
            int ri[MM];
            TipoApontador pi[MM + 1];
        } U0;

        /* Página externa: índice dos registros */
        struct {
            int ne;
            RegistroIndiceBEstrela re[MM];
        } U1;

    } UU;

} TipoPagina;

void InserePaginaInt(TipoPagina *pag, TipoApontador apontador, int ch);
void InsereArvoreB_estrela(TipoApontador *raiz, RegistroIndiceBEstrela item, AnaliseExperimental *analise);
int PesquisaRecEstrela(TipoApontador ap, int chave, RegistroIndiceBEstrela *indice, AnaliseExperimental *analise);
int PesquisarChaveB_estrela(TipoApontador raiz, FILE *arq, int chave, Item *resultado, AnaliseExperimental *analise);
void CriaArvoreB_estrela(FILE *arq, TipoApontador *raiz, AnaliseExperimental *analise);

#endif