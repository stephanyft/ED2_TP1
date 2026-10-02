#include <stdio.h>
#include <stdlib.h>
#include "abp.h"

FILE* inicializaABP(FILE *arq) {
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
        for(int i = 0; i < qtd; i++) {
            nodo.reg = regs[i];
            nodo.menor = -1;
            nodo.maior = -1;
            insereABP(abp, nodo, NULL);
        }
    }

    return abp;
}

void insereABP(FILE *abp, TipoNodo nodo, int *numComparacoes) {
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
        if(nodo.reg.chave < nodoAtual.reg.chave) {
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

Item* buscaABP(FILE *abp, int chave, int *numComparacoes) {
    if(abp == NULL) {
        printf("Erro ao abrir o arquivo ABP!\n");
        return NULL;
    }

    fseek(abp, 0, SEEK_SET);
    TipoNodo nodoAtual;
    while(1) {
        if(fread(&nodoAtual, sizeof(TipoNodo), 1, abp) != 1) {
            printf("Chave %d não encontrada na ABP.\n", chave);
            return NULL;
        }
        if(chave == nodoAtual.reg.chave) {
            Item* resultado = malloc(sizeof(Item));
            if(resultado == NULL) {
                printf("Erro de alocação de memória!\n");
                return NULL;
            }
            *resultado = nodoAtual.reg;
            return resultado;
        }
        else if(chave < nodoAtual.reg.chave) {
            if(nodoAtual.menor == -1) {
                printf("Chave %d não encontrada na ABP.\n", chave);
                return NULL;
            }
            fseek(abp, nodoAtual.menor * sizeof(TipoNodo), SEEK_SET);
        } 
        else {
            if(nodoAtual.maior == -1) {
                printf("Chave %d não encontrada na ABP.\n", chave);
                return NULL;
            }
            fseek(abp, nodoAtual.maior * sizeof(TipoNodo), SEEK_SET);
        }
    }
}