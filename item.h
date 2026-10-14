#ifndef ITEM_H
#define ITEM_H

#define M 2
#define MM (2 * M)

typedef struct{
    int chave;
    long int dado1;
    char dado2[5000];
}Item;
 
typedef struct{
    int numTransferencias, numComparacoes;
    double tempoExecucao;
}AnaliseExperimental;

#endif