#include<stdio.h>
#include<stdlib.h>
#include "paginacao.h"

Pagina* lerPagina(FILE* arquivo, int pagina) {
    if(arquivo == NULL)
        return;

    int desolcamento = TAM_PAGINA * pagina;

    if(fseek(arquivo, desolcamento, SEEK_SET) == 0) {
        Pagina pagina;

        fread(&pagina, sizeof(Pagina), 1, arquivo);

        return &pagina;
    }

    return NULL;
}

int pesquisa(Indice tabela[], int tam, Registro* item, FILE* arquivo) {

}

void criaTabelaIndices(FILE* arq, Indice* tabela) {
    Registro reg;

    int cont = 0;

    rewind(arq);

    while(fread(&reg, sizeof(Registro), 1, arq) == 1) {
        tabela->chaves[cont++] = reg.chave;

        fseek(arq, (TAM_PAGINA-1)*sizeof(Registro), SEEK_CUR);
    }
}