#include "geradorDeArquivos.h"

void gerarString(char *str, int tamanho){
    memset(str, 'a', tamanho - 1);
    str[tamanho - 1] = '\0';
}

void embaralhar(int *vetor, int n){
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1); 
        int temp = vetor[i];
        vetor[i] = vetor[j];
        vetor[j] = temp;
    }
}

int gerarArquivo(int quantReg, int situacao, const char *nomeArq){
    srand(time(NULL));

    int *chaves = malloc(sizeof(int) * quantReg);
    Item item;
    if (situacao == 2){
        for (int j = quantReg; j > 0; j--){
            chaves[quantReg - j] = j; 
        }
    }
    else {
        for (int i = 0; i < quantReg; i++){
            chaves[i] = i + 1; 
        }
        if (situacao == 3){
            embaralhar(chaves, quantReg); 
        }
    }
    FILE *arquivo = fopen(nomeArq, "wb");
    if (arquivo == NULL) {
        free(chaves);
        return 0; 
    }
    for (int i = 0; i < quantReg; i++){
        item.chave = chaves[i];
        item.dado1 = rand(); 
        gerarString(item.dado2, sizeof(item.dado2));
        fwrite(&item, sizeof(Item), 1, arquivo);
    }
    free(chaves);
    fclose(arquivo);
    return 1; 
}

void mostraArquivo(const char *nomeArq){
    FILE *arquivo = fopen(nomeArq, "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeArq);
        return;
    }
    Item item;
    while (fread(&item, sizeof(Item), 1, arquivo) == 1) {
        printf("Chave: %d, Dado1: %ld, Dado2: %.50s...\n", item.chave, item.dado1, item.dado2);
    }

    fclose(arquivo);
}