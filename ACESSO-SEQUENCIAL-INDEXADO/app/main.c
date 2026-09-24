#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "paginacao.h"

int main() {
    FILE* arquivo = fopen("data.dat", "wb");

    if(arquivo == NULL) {
        printf("Erro ao abrir arquivo\n");
        return -1;
    }

    fseek(arquivo, 0, SEEK_END);
    long tamanhoArquivo = ftell(arquivo);
    int tamanhoTabelaIndices = tamanhoArquivo / (sizeof(Registro)*TAM_PAGINA); 

    Pagina pagina;
    pagina.quantidade = 4;

    pagina.registros[0].chave = 10;
    strcpy(pagina.registros[0].nome, "Ana");

    pagina.registros[1].chave = 20;
    strcpy(pagina.registros[1].nome, "Bruno");

    pagina.registros[2].chave = 30;
    strcpy(pagina.registros[2].nome, "Carlos");

    pagina.registros[3].chave = 40;
    strcpy(pagina.registros[3].nome, "Daniel");

    fwrite(&pagina, sizeof(Pagina), 1, arquivo);

    fclose(arquivo);

    return 0;
}