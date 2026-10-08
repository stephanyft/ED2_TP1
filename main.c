#include "ARVORE-B/arvoreb.h"
#include "ABP/abp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "item.h"
#include "geradorDeArquivos.h"

int main(int argc, char *argv[]){
    if(argc < 5 || argc > 6){
        printf("Uso: %s <metodo> <quantidade> <situacao> <chave> [-P]\n", argv[0]);
        return 1;
    }
    int metodo = atoi(argv[1]);
    int quantidade = atoi(argv[2]);
    int situacao = atoi(argv[3]);
    int chave = atoi(argv[4]);
    int imprimir = 0;

    if (argc == 6) {
        if(strcmp(argv[5], "-P") == 0)
            imprimir = 1;
            
        else{
            printf("Erro: argumento opcional invalido (use -P)\n");
            return 1;
        }

    }
    if(metodo < 1 || metodo > 4) {
        printf("Erro: metodo deve ser numerado de 1 a 4\n");
        return 1;
    }
    if(situacao < 1 || situacao > 3) {
        printf("Erro: situaçao deve ser 1 (asc), (des) ou 3 (aleatorio)\n");
        return 1;
    }   
    const char *nomeArq = "registros.bin";
    
    AnaliseExperimental metricas;
    metricas.numComparacoes = 0;
    metricas.numTransferencias = 0;
    metricas.tempoExecucao = 0.0;

    if (!gerarArquivo(quantidade, situacao, nomeArq)) {
        printf("Falha critica ao gerar o arquivo de testes.\n");
        return 1;
    }

    switch(metodo) {
        case 1:
            printf("Metodo 1 ainda nao implemtnado\n");
            break;
        case 2:
            executarMetodo2(quantidade, situacao, chave, imprimir, nomeArq, &metricas);
            break;
        case 3: 
            executarMetodo3(quantidade, situacao, chave, imprimir, nomeArq, &metricas);
            break;
    }
    return 0; 
}