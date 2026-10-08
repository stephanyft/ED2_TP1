#include <stdio.h>
#include "arvoreb.h"
#include <stdlib.h>
#include <string.h>
#include "../item.h"
#include <time.h>
#include "../geradorDeArquivos.h"

void inicializa(TipoApontador* Arvore) {
    *Arvore = NULL;
}
int Pesquisa(TipoRegistro *x, TipoApontador Ap, AnaliseExperimental *metricas) {
    long i = 1;
    if(Ap == NULL){
        return 0;
    }
    while(i < Ap->n && x->Chave > Ap->r[i-1].Chave){ 
        i++;
        metricas->numComparacoes++;
    }
    metricas->numComparacoes++;
    if(x->Chave == Ap->r[i-1].Chave){
        *x = Ap->r[i-1];
        return 1;
    }
    if(x->Chave < Ap->r[i - 1].Chave){
        return Pesquisa(x, Ap->p[i - 1], metricas);
    }
    else{
        return Pesquisa(x, Ap->p[i], metricas);
    }
}

void Insere(TipoRegistro Reg, TipoApontador* Ap){
    short Cresceu;
    TipoRegistro RegRetorno;
    TipoPagina *ApRetorno, *ApTemp;
    Ins (Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno);
    if(Cresceu) {
        ApTemp = (TipoPagina *)malloc((sizeof(TipoPagina)));
        ApTemp->n = 1;
        ApTemp->r[0] = RegRetorno;
        ApTemp->p[1] = ApRetorno;
        ApTemp->p[0] = *Ap;
        *Ap = ApTemp;
    }

}

void Ins(TipoRegistro Reg, TipoApontador Ap, short *Cresceu, TipoRegistro *RegRetorno, TipoApontador *ApRetorno){
    
    long i = 1;
    long j;
    TipoApontador ApTemp;
    if (Ap == NULL) {
        *Cresceu = TRUE;
        *RegRetorno  = Reg;
        *ApRetorno = NULL;
        return;
    }
    while (i < Ap->n){
        if (Reg.Chave > Ap->r[i - 1].Chave) 
            i++;
        else
            break;
    }
        if (Reg.Chave == Ap->r[i - 1].Chave){
            printf("Erro: Registro ja esta presente\n");
            *Cresceu = FALSE;
            return;
        }
        if(Reg.Chave < Ap->r[i - 1].Chave)
            i--;
        Ins(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno);

        if(!*Cresceu)
            return;
        if(Ap->n < MM) {
            InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
            *Cresceu = FALSE;
            return;
        }
        ApTemp = (TipoApontador)malloc(sizeof(TipoPagina));
        ApTemp->n = 0;
        ApTemp->p[0] = NULL;

        if(i < M + 1) {
            InsereNaPagina(ApTemp, Ap->r[MM -1], Ap->p[MM]);
            Ap->n--;
            InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
        }
        else{
            InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno);
        }
        for(j = M + 2; j <= MM; j++)
            InsereNaPagina(ApTemp, Ap->r[j - 1], Ap->p[j]);
            Ap->n = M;
            ApTemp->p[0] = Ap->p[M + 1];
            *RegRetorno = Ap->r[M];
            *ApRetorno = ApTemp;
        
}

void Imprime(TipoApontador arvore) {
    int i = 0;

    if(arvore == NULL)
        return;
    while(i <= arvore->n){
        Imprime (arvore->p[i]);
        if(i != arvore->n)
        printf(" %d ", arvore->r[i].Chave);
        i++;
    }
}
    
void InsereNaPagina(TipoApontador Ap, TipoRegistro Reg, TipoApontador ApDir){
    short NaoAchouPosicao;
    int k;
    k = Ap->n;
    NaoAchouPosicao = (k > 0);
    while (NaoAchouPosicao) {
        if (Reg.Chave >= Ap->r[k - 1].Chave){
            NaoAchouPosicao = FALSE;
            break;
        }
        Ap->r[k] = Ap->r[k - 1];
        Ap->p[k + 1] = Ap->p[k];
        k--;
        if (k < 1) NaoAchouPosicao = FALSE;
    }
    Ap->r[k] = Reg;
    Ap->p[k+1] = ApDir;
    Ap->n++;
}
void CriaArvore(FILE * arq, TipoApontador *arvore, AnaliseExperimental *analise) {
    clock_t inicio, fim;
    inicio = clock();

    Item reg; 
    TipoRegistro itemIndice;
    long pos = 0;

    inicializa(arvore);
    rewind(arq);

    while(fread(&reg, sizeof(Item), 1, arq) == 1) {
        analise->numTransferencias++; 
        itemIndice.Chave = reg.chave;
        itemIndice.posicaoNoArquivo = pos; 
        
        Insere(itemIndice, arvore);
        pos++;
    }

    fim = clock();
    analise->tempoExecucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
}

void executarMetodo3(int quantidade, int situacao, int chaveBusca, int imprimir, const char *nomeArquivo, AnaliseExperimental *analise) {
    FILE *arq = fopen(nomeArquivo, "rb");
    if(arq == NULL) {
        printf("Erro ao abrir o arquivo %s!\n", nomeArquivo);
        return;
    }

    AnaliseExperimental metricasI;
    metricasI.numComparacoes = 0;
    metricasI.numTransferencias = 0;
    metricasI.tempoExecucao = 0.0;

    printf("\nConstruindo a Arvore B (Metodo 3)...\n");
    TipoApontador arvore;
    CriaArvore(arq, &arvore, &metricasI);

    analise->numComparacoes = 0;
    analise->numTransferencias = 0;
    analise->tempoExecucao = 0.0;

    printf("Pesquisando pela chave %d...\n", chaveBusca);
    
    TipoRegistro itemBuscado;
    itemBuscado.Chave = chaveBusca;
    itemBuscado.posicaoNoArquivo = -1;

    clock_t inicio, fim;
    inicio = clock();

    int achou = Pesquisa(&itemBuscado, arvore, analise); 

    Item regEncontrado; 
    if(achou) {
        fseek(arq, itemBuscado.posicaoNoArquivo * (long)sizeof(Item), SEEK_SET);
        if (fread(&regEncontrado, sizeof(Item), 1, arq) == 1) {
            analise->numTransferencias++; 
        }
        
        fim = clock();
        analise->tempoExecucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

        printf("\n--- REGISTRO ENCONTRADO! ---\n");
        printf("Chave: %d\n", chaveBusca);
        printf("Dado 1: %ld\n", regEncontrado.dado1);
        printf("Dado 2: %.50s...\n", regEncontrado.dado2);
        
        printf("\n\n--- METRICAS DE DESEMPENHO DE CRIAÇÃO ---\n");
        printf("Numero de transferencias da Criação: %d\n", metricasI.numTransferencias);
        printf("Numero de comparacoes da Criação: %d\n", metricasI.numComparacoes);
        printf("Tempo de execucao da Criação: %.5f segundos\n", metricasI.tempoExecucao);
        
        printf("\n\n--- METRICAS DE DESEMPENHO DE PESQUISA ---\n");
        printf("Numero de transferencias da Pesquisa: %d\n", analise->numTransferencias);
        printf("Numero de comparacoes da Pesquisa: %d\n", analise->numComparacoes);
        printf("Tempo de execucao da Pesquisa: %.5f segundos\n", analise->tempoExecucao);
    } else {
        fim = clock();
        analise->tempoExecucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
        printf("Chave %d não encontrada na Arvore B.\n", chaveBusca);
    }

    if (imprimir) {
        printf("\n--- CHAVES DE PESQUISA DO ARQUIVO ---\n");
        rewind(arq); 
        Item regTemp;
        
        while(fread(&regTemp, sizeof(Item), 1, arq) == 1) {
            printf("%d ", regTemp.chave);
        }
        printf("\n");
    }

    fclose(arq);
}