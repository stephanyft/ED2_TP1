#ifndef ABP_H
#define ABP_H

#include "../item.h"

#define TAM_PAGINA 4

typedef struct {
    Item reg;
    int menor;
    int maior;
}TipoNodo;

FILE* inicializaABP(FILE *arq, AnaliseExperimental *analise);
void insereABP(FILE *abp, TipoNodo nodo, AnaliseExperimental *analise);
Item* buscaABP(FILE *abp, int chave, AnaliseExperimental *analise);
void executarMetodo2(int quantidade, int situacao, int chaveBusca, int imprimir, const char *nomeArquivo, AnaliseExperimental *analise);


#endif // ABP_H