#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "abp.h"

FILE* inicializaABP(FILE *arq, AnaliseExperimental *analise) {
    clock_t inicio, fim;
    inicio = clock();

    if(arq == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return NULL;
    }

    FILE* abp = fopen("abp.bin", "wb+");
    if(abp == NULL) {
        printf("Erro ao criar o arquivo ABP!\n");
        return NULL;
    }

    Item regs[TAM_PAGINA];
    
    TipoNodo nodo;
    int qtd;
    while((qtd = fread(&regs, sizeof(Item), TAM_PAGINA, arq)) > 0) {
        analise->numTransferencias++; // Atualiza o número de transferências
        for(int i = 0; i < qtd; i++) {
            nodo.reg = regs[i];
            nodo.menor = -1;
            nodo.maior = -1;
            insereABP(abp, nodo, analise);
        }
    }
    fim = clock();
    analise->tempoExecucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    return abp;
}

void insereABP(FILE *abp, TipoNodo nodo, AnaliseExperimental *analise) {
    if(abp == NULL) {
        printf("Erro ao abrir o arquivo ABP!\n");
        return;
    }

    fseek(abp, 0, SEEK_END);
    fwrite(&nodo, sizeof(TipoNodo), 1, abp);
    long pos = ftell(abp) / sizeof(TipoNodo) - 1;

    if(pos == 0) {
        return; // Inserção do primeiro nodo
    }

    fseek(abp, 0, SEEK_SET);

    TipoNodo nodoAtual;
    int posicaoAtual = 0;
    while(1) {
        fread(&nodoAtual, sizeof(TipoNodo), 1, abp);
        analise->numComparacoes++; // Atualiza o número de comparações
        if(nodo.reg.chave < nodoAtual.reg.chave) {
            analise->numComparacoes++; // Atualiza o número de comparações
            if(nodoAtual.menor == -1) {
                nodoAtual.menor = pos;
                fseek(abp, posicaoAtual * sizeof(TipoNodo), SEEK_SET);
                fwrite(&nodoAtual, sizeof(TipoNodo), 1, abp);
                return;
            } 
            else {
                posicaoAtual = nodoAtual.menor;
                fseek(abp, posicaoAtual * sizeof(TipoNodo), SEEK_SET);
            }
        } 
        else {
            analise->numComparacoes++; // Atualiza o número de comparações
            if(nodoAtual.maior == -1) {
                nodoAtual.maior = pos;
                fseek(abp, posicaoAtual * sizeof(TipoNodo), SEEK_SET);
                fwrite(&nodoAtual, sizeof(TipoNodo), 1, abp);
                return;
            } 
            else {
                posicaoAtual = nodoAtual.maior;
                fseek(abp, posicaoAtual * sizeof(TipoNodo), SEEK_SET);
            }
        }
    }

}

Item* buscaABP(FILE *abp, int chave, AnaliseExperimental *analise) {
    clock_t inicio, fim;
    inicio = clock();
    
    if(abp == NULL) {
        printf("Erro ao abrir o arquivo ABP!\n");
        return NULL;
    }

    fseek(abp, 0, SEEK_SET);
    TipoNodo nodoAtual;
    while(1) {
        analise->numTransferencias++; // Atualiza o número de transferências
        if(fread(&nodoAtual, sizeof(TipoNodo), 1, abp) != 1) {
            printf("Chave %d não encontrada na ABP.\n", chave);
            fim = clock();
            analise->tempoExecucao += ((double)(fim - inicio)) / CLOCKS_PER_SEC;
            return NULL;
        }
        analise->numComparacoes++; // Atualiza o número de comparações
        if(chave == nodoAtual.reg.chave) {
            Item* resultado = malloc(sizeof(Item));
            if(resultado == NULL) {
                printf("Erro de alocação de memória!\n");
                return NULL;
            }
            *resultado = nodoAtual.reg;
            fim = clock();
            analise->tempoExecucao += ((double)(fim - inicio)) / CLOCKS_PER_SEC;
            return resultado;
        }
        else {
            analise->numComparacoes++; // Atualiza o número de comparações
            if(chave < nodoAtual.reg.chave) {
                analise->numComparacoes++; // Atualiza o número de comparações
                if(nodoAtual.menor == -1) {
                    fim = clock();
                    analise->tempoExecucao += ((double)(fim - inicio)) / CLOCKS_PER_SEC;
                    printf("Chave %d não encontrada na ABP.\n", chave);
                    return NULL;
                }
                fseek(abp, nodoAtual.menor * sizeof(TipoNodo), SEEK_SET);
            }
            else {
                analise->numComparacoes++; // Atualiza o número de comparações
                if(nodoAtual.maior == -1) {
                    fim = clock();
                    analise->tempoExecucao += ((double)(fim - inicio)) / CLOCKS_PER_SEC;
                    printf("Chave %d não encontrada na ABP.\n", chave);
                    return NULL;
                }
                fseek(abp, nodoAtual.maior * sizeof(TipoNodo), SEEK_SET);
            }
        }
    }
}

void executarMetodo2(int quantidade, int situacao, int chaveBusca, int imprimir, const char *nomeArquivo, AnaliseExperimental *analise) {
    FILE *arq = fopen(nomeArquivo, "rb");
    if(arq == NULL) {
        printf("Erro ao abrir o arquivo %s!\n", nomeArquivo);
        return;
    }

    AnaliseExperimental metricasI;
    metricasI.numComparacoes = 0;
    metricasI.numTransferencias = 0;
    metricasI.tempoExecucao = 0.0;

    printf("\nConstruindo a ABP (Metodo 2)...\n");
    FILE *abp = inicializaABP(arq, &metricasI);

    analise->numComparacoes = 0;
    analise->numTransferencias = 0;

    printf("Pesquisando pela chave %d...\n", chaveBusca);
    Item *resultado = buscaABP(abp, chaveBusca, analise);

    if(resultado != NULL) { 
        printf("\n--- REGISTRO ENCONTRADO! ---\n");
        printf("Chave: %d\n", chaveBusca);
        printf("Dado 1: %ld\n", resultado->dado1);
        printf("Dado 2: %.50s...\n", resultado->dado2);
        printf("\n\n--- METRICAS DE DESEMPENHO DE CRIAÇÃO ---\n");
        printf("Numero de transferencias da Criação: %d\n", metricasI.numTransferencias);
        printf("Numero de comparacoes da Criação: %d\n", metricasI.numComparacoes);
        printf("Tempo de execucao da Criação: %.5f segundos\n", metricasI.tempoExecucao);
        printf("\n\n--- METRICAS DE DESEMPENHO DE PESQUISA ---\n");
        printf("Numero de transferencias da Pesquisa: %d\n", analise->numTransferencias);
        printf("Numero de comparacoes da Pesquisa: %d\n", analise->numComparacoes);
        printf("Tempo de execucao da Pesquisa: %.5f segundos\n", analise->tempoExecucao);
        free(resultado);
    }

    if(imprimir) {
        printf("\n--- IMPRESSÃO DA ABP ---\n");
        fseek(abp, 0, SEEK_SET);
        TipoNodo nodo;
        while(fread(&nodo, sizeof(TipoNodo), 1, abp) == 1) {
            printf("Chave: %d, Dado1: %ld, Dado2: %.50s\n", nodo.reg.chave, nodo.reg.dado1, nodo.reg.dado2);
        }
    }

    fclose(arq);
    fclose(abp);
}