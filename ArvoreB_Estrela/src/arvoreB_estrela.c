#include "arvoreB_estrela.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void InserePaginaInt(TipoPagina *pag, TipoApontador apontador, int ch)
{
    /* Verifica se a página está cheia */
    if (pag->UU.U0.ni >= MM) {
        printf("Erro: página cheia!\n");
        return;
    }

    int i = 0;
    int j = 0;

    while (i < pag->UU.U0.ni && ch > pag->UU.U0.ri[i])
        i++; /* procura o local de inserção da chave */

    for (j = pag->UU.U0.ni; j > i; j--) {
        pag->UU.U0.ri[j] = pag->UU.U0.ri[j - 1];
    }

    pag->UU.U0.ri[i] = ch;

    for (j = pag->UU.U0.ni; j > i; j--) {
        pag->UU.U0.pi[j + 1] = pag->UU.U0.pi[j];
    }

    pag->UU.U0.pi[i + 1] = apontador;
    pag->UU.U0.ni++;
}

int PesquisaRecEstrela( TipoApontador ap, int chave, RegistroIndiceBEstrela *indice, AnaliseExperimental *analise) {
    int i = 0;

    if (ap == NULL) {
        return 0;
    }

    /* Pesquisa na página interna */
    if (ap->Pt == Interna) {

        while (i < ap->UU.U0.ni) {
            analise->numComparacoes++;
            if (chave >= ap->UU.U0.ri[i]) {
                i++;
            } else {
                break;
            }
        }

        return PesquisaRecEstrela( ap->UU.U0.pi[i], chave, indice, analise);
    }

    /* Pesquisa na página externa */
    while (i < ap->UU.U1.ne) {
        analise->numComparacoes++;
        if (chave > ap->UU.U1.re[i].chave) {
            i++;
        } else {
            break;
        }
    }

    if (i < ap->UU.U1.ne) {
        analise->numComparacoes++;
        if (chave == ap->UU.U1.re[i].chave) {
            *indice = ap->UU.U1.re[i];
            return 1;
        }
    }

    return 0;
}

int PesquisarChaveB_estrela(TipoApontador raiz, FILE *arq, int chave, Item *resultado, AnaliseExperimental *analise) {
    clock_t inicio = clock();
    RegistroIndiceBEstrela indice;
    int encontrado = 0;

    if (arq == NULL || resultado == NULL || analise == NULL) {
        return 0;
    }

    encontrado = PesquisaRecEstrela(raiz,chave,&indice,analise);

    if (!encontrado) {
        analise->tempoExecucao +=((double)(clock() - inicio)) / CLOCKS_PER_SEC;
        return 0;
    }
    /* A posição armazenada é o número do registro, começando em zero */
    if (fseek(arq, indice.posicaoNoArquivo * (long)sizeof(Item),SEEK_SET) != 0) {

        analise->tempoExecucao +=((double)(clock() - inicio)) / CLOCKS_PER_SEC;
        return 0;
    }
    if (fread(resultado, sizeof(Item), 1, arq) != 1) {
        analise->tempoExecucao +=((double)(clock() - inicio)) / CLOCKS_PER_SEC;
        return 0;
    }
    analise->numTransferencias++;
    analise->tempoExecucao += ((double)(clock() - inicio)) / CLOCKS_PER_SEC;

    return 1;
}


/*Função recursina auxiliar*/
int InsB_estrela(RegistroIndiceBEstrela item, TipoApontador PagAutal, TipoApontador PagPai, int IndiceFilho, int *Dividiu, TipoApontador *NovaPag, int *chavePromovida, AnaliseExperimental *analise){
}



void InsereArvoreB_estrela(TipoApontador *raiz, RegistroIndiceBEstrela item, AnaliseExperimental *analise){
    /*Trata arvores vazias*/
    if(*raiz == NULL){
        TipoPagina *nova_pag = malloc(sizeof(TipoPagina));
        if(nova_pag == NULL){
            return;
        }

        nova_pag->Pt = Externa;
        nova_pag->UU.U1.ne = 1;
        nova_pag->UU.U1.re[0] = item;
        *raiz = nova_pag;
        return;
    }
    /*Trata paginas externas com espaço*/

    /*INSERIR FUNÇAO INSEREPAGEXTRENA*/

    /*pesquisa pelas paginas internas*/




    
}

