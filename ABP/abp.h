#ifndef ABP_H
#define ABP_H

#include "item.h"

#define TAM_PAGINA 4

typedef struct {
    Item reg;
    int menor;
    int maior;
}TipoNodo;

FILE* inicializaABP(FILE *arq);
void insereABP(FILE *abp, TipoNodo nodo, int *numComparacoes);
Item* buscaABP(FILE *abp, int chave, int *numComparacoes);


#endif // ABP_H