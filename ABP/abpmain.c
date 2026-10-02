#include <stdio.h>
#include <stdlib.h>
#include "abp.h"
#include "geradorDeArquivos.h"

int main() {
    gerarArquivo(150, 3, "dados.bin");
    mostraArquivo("dados.bin");

    FILE *arq = fopen("dados.bin", "rb");
    if(arq == NULL) {
        printf("Erro ao abrir o arquivo de dados!\n");
        return 1;
    }

    FILE *abp = inicializaABP(arq);
    Item *x = buscaABP(abp, 24, NULL);
    printf("ENCONTROU! Chave: %d, Dado1: %ld, Dado2: %s, Dado3: %s\n", x->chave, x->dado1, x->dado2, x->dado3);

    fclose(arq);
    fclose(abp);
    return 0;
}