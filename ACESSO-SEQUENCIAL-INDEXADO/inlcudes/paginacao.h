#ifndef PAGINACAO_H
#define PAGINACAO_H

#define TAM_PAGINA 4

typedef struct _registro {
    int chave;
    char nome[50];
} Registro;

typedef struct _pagina {
    Registro registros[TAM_PAGINA];
    int quantidade;
} Pagina;

typedef struct _indice {
    int* chaves;
} Indice;


#endif

// 1
// 2
// 3
// 4
// 5
// 6
// 7
// 8
// 9
// 10
// 11
// 12
// 13
// 14
// 15
// 16
// 17
// 18
// 19
// 20