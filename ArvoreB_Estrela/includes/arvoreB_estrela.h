#ifndef ARVOREB_ESTRELA_H
#define ARVOREB_ESTRELA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "item.h"

#define M 4
#define MM (2 * M)

//Registro armazenado no índice da árvore
typedef struct {
    int chave;
    long posicaoNoArquivo;
} RegistroIndiceBEstrela;

//Indica se a página é interna ou externa (folha)
typedef enum {
    Interna,
    Externa
} TipoIntExt;

//Ponteiro para uma página da árvore
typedef struct TipoPagina *TipoApontador;

//Estrutura de uma página da árvore
typedef struct TipoPagina {
    TipoIntExt Pt;

    union {
        struct {
            int ni;
            int ri[MM];
            TipoApontador pi[MM + 1];
        } U0;

        struct {
            int ne;
            RegistroIndiceBEstrela re[MM];
        } U1;
    } UU;
} TipoPagina;

//Inserção em página interna
void InserePaginaInt(
    TipoPagina *pag,
    TipoApontador apontador,
    int ch
);

//Inserção em página externa
int inserePaginaExterna(
    TipoPagina *pagina,
    RegistroIndiceBEstrela *item,
    AnaliseExperimental *analise
);

//Inserção recursiva, com divisão de páginas quando necessário
int insRecEstrela(
    TipoApontador pagina,
    RegistroIndiceBEstrela item,
    int *cresceu,
    int *chaveRetorno,
    TipoApontador *apontadorRetorno,
    AnaliseExperimental *analise
);

//Inserção na árvore inteira
void InsereArvoreB_estrela(
    TipoApontador *raiz,
    RegistroIndiceBEstrela item,
    AnaliseExperimental *analise
);

//Construção do índice a partir do arquivo de registros
void CriaArvoreB_estrela(
    FILE *arq,
    TipoApontador *raiz,
    AnaliseExperimental *analise
);

//Pesquisa recursiva no índice
int PesquisaRecEstrela(
    TipoApontador ap,
    int chave,
    RegistroIndiceBEstrela *indice,
    AnaliseExperimental *analise
);

//Pesquisa e recuperação do registro no arquivo
int PesquisarChaveB_estrela(
    TipoApontador raiz,
    FILE *arq,
    int chave,
    Item *resultado,
    AnaliseExperimental *analise
);

//Impressão da árvore para depuração
void printArvoreEstrelaRec(
    TipoApontador raiz,
    int nivel
);

void printArvoreB_estrela(
    TipoApontador raiz
);

#endif