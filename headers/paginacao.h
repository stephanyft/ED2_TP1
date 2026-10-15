#ifndef PAGINACAO_H
#define PAGINACAO_H

#include <stdio.h>

#include "../item.h"

#define ITENS_POR_PAGINA 4

//Entrada do índice esparso mantido em memória principal. A chave é a do primeiro registro da página, enquanto primeiroRegistro informa em qual posição do arquivo essa página começa.
typedef struct {
    int chave;
    long primeiroRegistro;
} Indice;

// Cria a tabela de índices a partir de um arquivo ordenado O ponteiro duplo permite que a função devolva a memória alocada ao chamador Retorna 1 em caso de sucesso e 0 em caso de erro 
int criaTabelaIndices(FILE *arquivo, Indice **tabela, int *tamanhoTabela, AnaliseExperimental *metricas);

// Localiza a página pelo índice, carrega essa página e procura a chave nela. Quando encontra o registro, copia seus dados para resultado. 
int pesquisaSequencialIndexada(FILE *arquivo, const Indice tabela[], int tamanhoTabela, int chave, Item *resultado, AnaliseExperimental *metricas);

// Executa o método 1 e apresenta o registro e as métricas coletadas.
void executarMetodo1(int quantidade, int situacao, int chaveBusca, int imprimir, const char *nomeArquivo, AnaliseExperimental *metricas);

#endif
