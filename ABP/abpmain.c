#include <stdio.h>
#include <stdlib.h>
#include "abp.h"
//#include "geradorDeArquivos.h"

int main() {
    gerarArquivo(150, 3, "dados.bin");
    mostraArquivo("dados.bin");

    AnaliseExperimental analise1; analise1.numTransferencias = 0; analise1.numComparacoes = 0; analise1.tempoExecucao = 0.0;
    AnaliseExperimental analise2; analise2.numTransferencias = 0; analise2.numComparacoes = 0; analise2.tempoExecucao = 0.0;

    FILE *arq = fopen("dados.bin", "rb");
    if(arq == NULL) {
        printf("Erro ao abrir o arquivo de dados!\n");
        return 1;
    }

    FILE *abp = inicializaABP(arq, &analise1);
    Item *x = buscaABP(abp, 24, &analise2);
    printf("ENCONTROU! Chave: %d, Dado1: %ld, Dado2: %s\n", x->chave, x->dado1, x->dado2);
    printf("Numero de transferencias da Insercao: %d\n", analise1.numTransferencias);
    printf("Numero de comparacoes da Insercao: %d\n", analise1.numComparacoes);
    printf("Tempo de execucao da Insercao: %.2f segundos\n", analise1.tempoExecucao);
    printf("Numero de transferencias da Pesquisa: %d\n", analise2.numTransferencias);
    printf("Numero de comparacoes da Pesquisa: %d\n", analise2.numComparacoes);
    printf("Tempo de execucao da Pesquisa: %.5f segundos\n", analise2.tempoExecucao);

    fclose(arq);
    fclose(abp);
    return 0;
}