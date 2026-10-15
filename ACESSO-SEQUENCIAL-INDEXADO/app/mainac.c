#include "paginacao.h"

#include <stdio.h>
#include <stdlib.h>

/* Executavel auxiliar para testar o modulo isoladamente. */
int main(int argc, char *argv[]) {
    /* O teste isolado recebe o arquivo já criado e a chave desejada. */
    if (argc != 3) {
        printf("Uso: %s <arquivo-binario> <chave>\n", argv[0]);
        return 1;
    }

    char *fim = NULL;
    long chaveLong = strtol(argv[2], &fim, 10);

    /* strtol permite detectar entradas que não representam um número inteiro. */
    if (*argv[2] == '\0' || *fim != '\0') {
        printf("Erro: chave invalida.\n");
        return 1;
    }

    FILE *arquivo = fopen(argv[1], "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", argv[1]);
        return 1;
    }

    AnaliseExperimental criacao = {0, 0, 0.0};
    AnaliseExperimental pesquisa = {0, 0, 0.0};
    Indice *tabela = NULL;
    int tamanhoTabela = 0;

    /* Cria o índice em memória antes de iniciar a pesquisa. */
    if (!criaTabelaIndices(arquivo, &tabela, &tamanhoTabela, &criacao)) {
        printf("Erro ao construir a tabela de indices.\n");
        fclose(arquivo);
        return 1;
    }

    /* Pesquisa e apresenta o registro recuperado do arquivo, quando existir. */
    Item resultado;
    if (pesquisaSequencialIndexada(arquivo, tabela, tamanhoTabela,
                                   (int)chaveLong, &resultado, &pesquisa)) {
        printf("Registro encontrado: chave=%d, dado1=%ld, dado2=%.50s...\n",
               resultado.chave, resultado.dado1, resultado.dado2);
    } else {
        printf("Chave %ld nao encontrada.\n", chaveLong);
    }

    printf("Criacao: transferencias=%d, comparacoes=%d, tempo=%.5f s\n",
           criacao.numTransferencias, criacao.numComparacoes,
           criacao.tempoExecucao);
    printf("Pesquisa: transferencias=%d, comparacoes=%d, tempo=%.5f s\n",
           pesquisa.numTransferencias, pesquisa.numComparacoes,
           pesquisa.tempoExecucao);

    /* Libera os recursos adquiridos durante o teste. */
    free(tabela);
    fclose(arquivo);
    return 0;
}
