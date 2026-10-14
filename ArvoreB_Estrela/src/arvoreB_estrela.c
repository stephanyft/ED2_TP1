#include "arvoreB_estrela.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void CriaArvoreB_estrela(
    FILE *arq,
    TipoApontador *raiz,
    AnaliseExperimental *analise
) {
    if (arq == NULL || raiz == NULL || analise == NULL) {
        return;
    }

    clock_t inicio = clock();

    rewind(arq);
    *raiz = NULL;

    Item registro;
    long posicao = 0;

    while (fread(&registro, sizeof(Item), 1, arq) == 1) {
        analise->numTransferencias++;

        RegistroIndiceBEstrela indice;
        indice.chave = registro.chave;
        indice.posicaoNoArquivo = posicao;

        InsereArvoreB_estrela(raiz, indice, analise);

        posicao++;
    }

    analise->tempoExecucao +=
        (double)(clock() - inicio) / CLOCKS_PER_SEC;
}

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

//Insere um registro de índice ordenadamente em uma página externa que ainda possui espaço.
int inserePaginaExterna( TipoPagina *pagina, RegistroIndiceBEstrela *item,AnaliseExperimental *analise){
    if (pagina == NULL || item == NULL ||
        pagina->Pt != Externa) {
        return 0;
    }

    int n = pagina->UU.U1.ne;
    int i = 0;

    if (n < 0 || n >= MM) {
        return 0;
    }

    //Localiza a posição correta e verifica duplicidade.
    while (i < n) {
        if (analise != NULL) {
            analise->numComparacoes++;
        }

        if (pagina->UU.U1.re[i].chave >= item->chave) {
            break;
        }

        i++;
    }

    if (i < n) {
        if (analise != NULL) {
            analise->numComparacoes++;
        }

        if (pagina->UU.U1.re[i].chave == item->chave) {
            return 0;
        }
    }

    //Desloca os registros para abrir espaço.
    for (int j = n; j > i; j--) {
        pagina->UU.U1.re[j] =
            pagina->UU.U1.re[j - 1];
    }

    pagina->UU.U1.re[i] = *item;
    pagina->UU.U1.ne++;

    return 1;
}

//Insere recursivamente. Quando uma página precisa ser dividida, retorna a chave separadora e o ponteiro para a nova página à direita.
int insRecEstrela( TipoApontador pagina, RegistroIndiceBEstrela item, int *cresceu, int *chaveRetorno, TipoApontador *apontadorRetorno, AnaliseExperimental *analise){
    if (pagina == NULL || cresceu == NULL ||
        chaveRetorno == NULL || apontadorRetorno == NULL) {
        return -1;
    }

    *cresceu = 0;
    *apontadorRetorno = NULL;

    /* Caso 1: página externa (folha). */
    if (pagina->Pt == Externa) {
        int n = pagina->UU.U1.ne;

        if (n < 0 || n > MM) {
            return -1;
        }

        // Se há espaço, delega a inserção à função auxiliar
        if (n < MM) {
            return inserePaginaExterna(
                pagina, &item, analise
            );
        }

        //Página cheia: encontra a posição e verifica duplicidade
        int i = 0;

        while (i < n) {
            if (analise != NULL) {
                analise->numComparacoes++;
            }

            if (pagina->UU.U1.re[i].chave >= item.chave) {
                break;
            }

            i++;
        }

        if (i < n) {
            if (analise != NULL) {
                analise->numComparacoes++;
            }

            if (pagina->UU.U1.re[i].chave == item.chave) {
                return 0;
            }
        }

        //Reúne os MM registros existentes e o novo
        RegistroIndiceBEstrela temporarios[MM + 1];

        for (int j = 0; j < i; j++) {
            temporarios[j] = pagina->UU.U1.re[j];
        }

        temporarios[i] = item;

        for (int j = i; j < n; j++) {
            temporarios[j + 1] = pagina->UU.U1.re[j];
        }

        TipoApontador direita =
            calloc(1, sizeof(TipoPagina));

        if (direita == NULL) {
            return -1;
        }

        direita->Pt = Externa;

        //Divisão convencional em duas folhas
        int quantidadeEsquerda = (MM + 1) / 2;

        pagina->UU.U1.ne = quantidadeEsquerda;

        for (int j = 0; j < quantidadeEsquerda; j++) {
            pagina->UU.U1.re[j] = temporarios[j];
        }

        direita->UU.U1.ne =
            MM + 1 - quantidadeEsquerda;

        for (int j = 0; j < direita->UU.U1.ne; j++) {
            direita->UU.U1.re[j] =
                temporarios[quantidadeEsquerda + j];
        }

        // A primeira chave da folha direita separa os dois lados.
        *chaveRetorno = direita->UU.U1.re[0].chave;
        *apontadorRetorno = direita;
        *cresceu = 1;

        return 1;
    }

    /* Caso 2: página interna. Localiza o filho adequado. */
    if (pagina->Pt != Interna ||
        pagina->UU.U0.ni < 0 ||
        pagina->UU.U0.ni > MM) {
        return -1;
    }

    int i = 0;
    int n = pagina->UU.U0.ni;

    while (i < n) {
        if (analise != NULL) {
            analise->numComparacoes++;
        }

        if (item.chave >= pagina->UU.U0.ri[i]) {
            i++;
        } else {
            break;
        }
    }

    if (pagina->UU.U0.pi[i] == NULL) {
        return -1;
    }

    int cresceuFilho = 0;
    int chaveFilho = 0;
    TipoApontador novaPaginaFilho = NULL;

    int resultado = insRecEstrela(
        pagina->UU.U0.pi[i],
        item,
        &cresceuFilho,
        &chaveFilho,
        &novaPaginaFilho,
        analise
    );

    if (resultado != 1) {
        return resultado;
    }

    //Se o filho não se dividiu, não há chave a propagar
    if (!cresceuFilho) {
        return 1;
    }

    //O filho se dividiu e a página interna ainda tem espaço
    if (n < MM) {
        for (int j = n; j > i; j--) {
            pagina->UU.U0.ri[j] =
                pagina->UU.U0.ri[j - 1];
        }

        for (int j = n + 1; j > i + 1; j--) {
            pagina->UU.U0.pi[j] =
                pagina->UU.U0.pi[j - 1];
        }

        pagina->UU.U0.ri[i] = chaveFilho;
        pagina->UU.U0.pi[i + 1] = novaPaginaFilho;
        pagina->UU.U0.ni++;

        return 1;
    }

    //Página interna cheia: prepara as chaves e os filhos temporários
    int chaves[MM + 1];
    TipoApontador filhos[MM + 2];

    for (int j = 0; j < n; j++) {
        chaves[j] = pagina->UU.U0.ri[j];
    }

    for (int j = 0; j <= n; j++) {
        filhos[j] = pagina->UU.U0.pi[j];
    }

    for (int j = n; j > i; j--) {
        chaves[j] = chaves[j - 1];
    }

    for (int j = n + 1; j > i + 1; j--) {
        filhos[j] = filhos[j - 1];
    }

    chaves[i] = chaveFilho;
    filhos[i + 1] = novaPaginaFilho;

    TipoApontador direita =
        calloc(1, sizeof(TipoPagina));

    if (direita == NULL) {
        //A divisão do filho já ocorreu. O chamador precisa tratar esse erro
        return -1;
    }

    direita->Pt = Interna;

    int meio = (MM + 1) / 2;

    //Promove a chave central para a página pai
    *chaveRetorno = chaves[meio];

    pagina->UU.U0.ni = meio;

    for (int j = 0; j < meio; j++) {
        pagina->UU.U0.ri[j] = chaves[j];
    }

    for (int j = 0; j <= meio; j++) {
        pagina->UU.U0.pi[j] = filhos[j];
    }

    direita->UU.U0.ni = MM - meio;

    for (int j = 0; j < direita->UU.U0.ni; j++) {
        direita->UU.U0.ri[j] =
            chaves[meio + 1 + j];
    }

    for (int j = 0; j <= direita->UU.U0.ni; j++) {
        direita->UU.U0.pi[j] =
            filhos[meio + 1 + j];
    }

    *apontadorRetorno = direita;
    *cresceu = 1;

    return 1;
}


void InsereArvoreB_estrela(
    TipoApontador *raiz,
    RegistroIndiceBEstrela item,
    AnaliseExperimental *analise
) {
    if (raiz == NULL || analise == NULL) {
        return;
    }

    /* Caso 1: árvore vazia. */
    if (*raiz == NULL) {
        TipoApontador novaPagina =
            calloc(1, sizeof(TipoPagina));

        if (novaPagina == NULL) {
            return;
        }

        novaPagina->Pt = Externa;
        novaPagina->UU.U1.ne = 1;
        novaPagina->UU.U1.re[0] = item;

        *raiz = novaPagina;
        return;
    }

    /* Caso 2: árvore existente. */
    int cresceu = 0;
    int chaveRetorno = 0;
    TipoApontador apontadorRetorno = NULL;

    int resultado = insRecEstrela(
        *raiz,
        item,
        &cresceu,
        &chaveRetorno,
        &apontadorRetorno,
        analise
    );

    //A inserção falhou ou a chave já existia
    if (resultado != 1) {
        return;
    }

    //A raiz não se dividiu: a árvore continua com a mesma raiz
    if (!cresceu) {
        return;
    }

    /* Caso 3: a raiz se dividiu. Cria uma nova raiz interna com a chave separadora e os ponteiros para as páginas esquerda e direita.
    */
    TipoApontador novaRaiz =
        calloc(1, sizeof(TipoPagina));

    if (novaRaiz == NULL) {
        //A alocação falhou depois da divisão da raiz
        return;
    }

    novaRaiz->Pt = Interna;
    novaRaiz->UU.U0.ni = 1;

    novaRaiz->UU.U0.ri[0] = chaveRetorno;
    novaRaiz->UU.U0.pi[0] = *raiz;
    novaRaiz->UU.U0.pi[1] = apontadorRetorno;

    *raiz = novaRaiz;
}

//Impressão recursiva da árvore
void printArvoreEstrelaRec(
    TipoApontador raiz,
    int nivel
) {
    if (raiz == NULL) {
        return;
    }

    for (int j = 0; j < nivel; j++) {
        printf("  ");
    }

    if (raiz->Pt == Externa) {
        printf("Folha: ");

        for (int i = 0; i < raiz->UU.U1.ne; i++) {
            printf("%d", raiz->UU.U1.re[i].chave);

            if (i + 1 < raiz->UU.U1.ne) {
                printf(", ");
            }
        }

        printf("\n");
        return;
    }

    printf("Interna: ");

    for (int i = 0; i < raiz->UU.U0.ni; i++) {
        printf("%d", raiz->UU.U0.ri[i]);

        if (i + 1 < raiz->UU.U0.ni) {
            printf(", ");
        }
    }

    printf("\n");

    for (int i = 0; i <= raiz->UU.U0.ni; i++) {
        printArvoreEstrelaRec(
            raiz->UU.U0.pi[i],
            nivel + 1
        );
    }
}


//Função de entrada para imprimir a árvore
void printArvoreB_estrela(TipoApontador raiz) {
    if (raiz == NULL) {
        printf("Arvore vazia.\n");
        return;
    }

    printArvoreEstrelaRec(raiz, 0);
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
