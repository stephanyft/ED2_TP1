#include <stdio.h>
#include "../headers/arvoreb.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>


void inicializa(TipoApontador* Arvore) {
    *Arvore = NULL;
} /* Procura a chave de x na árvore B a partir da página Ap.
   Retorna 1 e copia o registro para x se encontrar; senão retorna 0. */
int Pesquisa(TipoRegistro *x, TipoApontador Ap, AnaliseExperimental *metricas) {
    long i = 1;
    
    /* Página vazia: a chave não está na árvore */
    if(Ap == NULL){
        return 0;
    }
    /* Avança na página enquanto a chave procurada for maior */
    while(i < Ap->n && x->Chave > Ap->r[i-1].Chave){ 
        i++;
        metricas->numComparacoes++;
    }
    metricas->numComparacoes++;
    /* Achou a chave: copia o registro */
    if(x->Chave == Ap->r[i-1].Chave){
        *x = Ap->r[i-1];
        return 1;
    }
    /* Não achou: desce para o filho da esquerda ou da direita */
    if(x->Chave < Ap->r[i - 1].Chave){
        return Pesquisa(x, Ap->p[i - 1], metricas);
    }
    else{
        return Pesquisa(x, Ap->p[i], metricas);
    }
}
// insere o registro reg na arvore b, cujo o registro *ap//
// se a raiz se dividir cria uma nova raiz e a arvore sempre cresce lembrando que sempre pra cima//
void Insere(TipoRegistro Reg, TipoApontador* Ap, AnaliseExperimental *metricas) {
    short Cresceu;
    TipoRegistro RegRetorno;
    TipoPagina *ApRetorno, *ApTemp;
    // faz a iserção descendo pela arvore//
    Ins (Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno, metricas);
    // a raiz se dividiu cria uma nova raiz, com o registro que subiu//
    if(Cresceu) {
        ApTemp = (TipoPagina *)malloc((sizeof(TipoPagina)));
        ApTemp->n = 1;
        ApTemp->r[0] = RegRetorno; 
        ApTemp->p[1] = ApRetorno; // filho direito: nova pagina da divisao//
        ApTemp->p[0] = *Ap; // filho esquerdo: raiz antiga// 
        *Ap = ApTemp; // a nova pagina passa a ser a raiz//
    }

}

// Função recursiva que localiza a página folha para a inserção e realiza o split //
void Ins(TipoRegistro Reg, TipoApontador Ap, short *Cresceu, TipoRegistro *RegRetorno, TipoApontador *ApRetorno, AnaliseExperimental *metricas){
    
    long i = 1;
    long j;
    TipoApontador ApTemp;

    // Se a página atual for nula, o item é inserido na página pai //
    if (Ap == NULL) {
        *Cresceu = TRUE;
        *RegRetorno  = Reg;
        *ApRetorno = NULL;
        return;
    }
    // Encontra a subárvore correta para descer //
    while (i < Ap->n){
        metricas->numComparacoes++;
        if (Reg.Chave > Ap->r[i - 1].Chave){
            i++;
        }
        else{
            break;
        }
    }
    metricas->numComparacoes++;
    // Evita chaves duplicadas //
    if (Reg.Chave == Ap->r[i - 1].Chave){
        printf("Erro: Registro ja esta presente\n");
        *Cresceu = FALSE;
        return;
    }
    metricas->numComparacoes++;
    if(Reg.Chave < Ap->r[i - 1].Chave){
        i--;
    }
    // Descida recursiva // 
    Ins(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno, metricas);
    // Se não houve split encerra //
    if(!*Cresceu){
        return;
    }
    metricas->numComparacoes++;
    // Se a página atual tiver espaço, insere o registro e encerra //
    if(Ap->n < MM) {
        InsereNaPagina(Ap, *RegRetorno, *ApRetorno, metricas);
        *Cresceu = FALSE;
        return;
    }
    // Se a página atual estiver cheia, realiza o split //
    ApTemp = (TipoApontador)malloc(sizeof(TipoPagina));
    ApTemp->n = 0;
    ApTemp->p[0] = NULL;

    metricas->numComparacoes++;
    if(i < M + 1) {
        InsereNaPagina(ApTemp, Ap->r[MM -1], Ap->p[MM], metricas);
        Ap->n--;
        InsereNaPagina(Ap, *RegRetorno, *ApRetorno, metricas);
    }
    else{
        InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno, metricas);
    }
    // Copia os registros da página cheia para a nova página //
    for(j = M + 2; j <= MM; j++){
        InsereNaPagina(ApTemp, Ap->r[j - 1], Ap->p[j], metricas);
        Ap->n = M;
        ApTemp->p[0] = Ap->p[M + 1];
        *RegRetorno = Ap->r[M];
        *ApRetorno = ApTemp;
    }   
}

// Função para imprimir a árvore B em ordem //
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

// Função para inserir um registro em uma página da árvore B //
void InsereNaPagina(TipoApontador Ap, TipoRegistro Reg, TipoApontador ApDir, AnaliseExperimental *metricas) {
    short NaoAchouPosicao;
    int k;
    k = Ap->n;
    NaoAchouPosicao = (k > 0);
    while (NaoAchouPosicao) {
        metricas->numComparacoes++;
        if (Reg.Chave >= Ap->r[k - 1].Chave){
            NaoAchouPosicao = FALSE;
            break;
        }
        Ap->r[k] = Ap->r[k - 1];
        Ap->p[k + 1] = Ap->p[k];
        k--;
        metricas->numComparacoes++;
        if (k < 1) NaoAchouPosicao = FALSE;
    }
    Ap->r[k] = Reg;
    Ap->p[k+1] = ApDir;
    Ap->n++;
}

// Função para criar a árvore B a partir de um arquivo de registros //
void CriaArvore(FILE * arq, TipoApontador *arvore, AnaliseExperimental *analise) {
    clock_t inicio, fim;
    inicio = clock();

    Item reg; 
    TipoRegistro itemIndice;
    long pos = 0;

    inicializa(arvore);
    rewind(arq);

    // Lê os registros do arquivo e insere na árvore B //
    while(fread(&reg, sizeof(Item), 1, arq) == 1) {
        analise->numTransferencias++; 
        itemIndice.Chave = reg.chave;
        itemIndice.posicaoNoArquivo = pos; 
        
        Insere(itemIndice, arvore, analise);
        pos++;
    }

    fim = clock();
    analise->tempoExecucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
}

// Função para criar a árvore B e realiza a pesquisa de uma chave específica a partir da entrada no terminal //
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
            printf("Chave: %d , Dado1: %ld , Dado2: %.50s\n", regTemp.chave, regTemp.dado1, regTemp.dado2);
        }
        printf("\n");
    }

    fclose(arq);
}