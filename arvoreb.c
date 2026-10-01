#include <stdio.h>
#include "arvoreb.h"

void inicializa(TipoApontador* Arvore) {
    Arvore = NULL;
}
void Pesquisa(TipoRegistro *x, TipoApontador Ap) {
    long i = 1;
    if(Ap == NULL){
        printf("TipoRegistro nao esta presente na arovre\n");
        return;
    }
    while(i < Ap->n && x->Chave > Ap->r[i-1].Chave) 
        i++;
    if(x->Chave == Ap->r[i-1].Chave){
        *x = Ap->r[i-1];
        return;
    }
    if(x->Chave < Ap->r[i - 1].Chave)
        Pesquisa(x, Ap->p[i - 1]);
        else Pesquisa(x, Ap->p[i]);
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
    if (Ap == NULL) {
        *Cresceu = TRUE;
        (*RegRetorno)  = Reg;
        (*ApRetorno) = NULL;
        return;
    }
    while (i < Ap->n && Reg.Chave > Ap->r[i - 1].Chave)
        i++;
        if (Reg.Chave == Ap->r[i - 1].Chave) {
            printf("Erro: Registro ja esta presente\n");
            *Cresceu = FALSE;
            return;
        }
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