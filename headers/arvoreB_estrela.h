#ifndef ARVOREB_ESTRELA_H
#define ARVOREB_ESTRELA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../item.h"

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
typedef struct TipoPaginaE *TipoApontadorE;

//Estrutura de uma página da árvore
typedef struct TipoPaginaE {
    TipoIntExt Pt;

    union {
        struct {
            int ni;
            int ri[MM];
            TipoApontadorE pi[MM + 1];
        } U0;

        struct {
            int ne;
            RegistroIndiceBEstrela re[MM];
        } U1;
    } UU;
} TipoPaginaE;

//Inserção em página interna
void InserePaginaInt(
    TipoPaginaE *pag,
    TipoApontadorE apontador,
    int ch
);

//Inserção em página externa
int inserePaginaExterna(
    TipoPaginaE *pagina,
    RegistroIndiceBEstrela *item,
    AnaliseExperimental *analise
);

//Inserção recursiva, com divisão de páginas quando necessário
int insRecEstrela(
    TipoApontadorE pagina,
    RegistroIndiceBEstrela item,
    int *cresceu,
    int *chaveRetorno,
    TipoApontadorE *apontadorRetorno,
    AnaliseExperimental *analise
);

//Inserção na árvore inteira
void InsereArvoreB_estrela(
    TipoApontadorE *raiz,
    RegistroIndiceBEstrela item,
    AnaliseExperimental *analise
);

//Construção do índice a partir do arquivo de registros
void CriaArvoreB_estrela(
    FILE *arq,
    TipoApontadorE *raiz,
    AnaliseExperimental *analise
);

//Pesquisa recursiva no índice
int PesquisaRecEstrela(
    TipoApontadorE ap,
    int chave,
    RegistroIndiceBEstrela *indice,
    AnaliseExperimental *analise
);

//Pesquisa e recuperação do registro no arquivo
int PesquisarChaveB_estrela(
    TipoApontadorE raiz,
    FILE *arq,
    int chave,
    Item *resultado,
    AnaliseExperimental *analise
);

//Impressão da árvore para depuração
void printArvoreEstrelaRec(
    TipoApontadorE raiz,
    int nivel
);

void printArvoreB_estrela(
    TipoApontadorE raiz
);

void executarMetodo4(
    int quantidade,
    int situacao,
    int chaveBusca,
    int imprimir,
    const char *nomeArquivo,
    AnaliseExperimental *analise);

#endif