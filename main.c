#include "headers/arvoreb.h"
#include "headers/abp.h"
#include "headers/arvoreB_estrela.h"
#include "headers/paginacao.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "item.h"
#include "headers/geradorDeArquivos.h"

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
        printf("Erro: situaçao deve ser 1 (asc), 2 (des) ou 3 (aleatorio)\n");
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

    int chavesBusca[20];
    int numChaves = 1;

    if (chave == -1) {
        numChaves = 20;
        printf("\n=== MODO DE EXPERIMENTO (20 CHAVES DISTINTAS) ===\n");
        sortearChaves(nomeArq, quantidade, chavesBusca, numChaves);
    } else {
        chavesBusca[0] = chave;
    }

    double tempoTotal = 0.0;
    long transfTotal = 0;
    long compTotal = 0;

    for (int i = 0; i < numChaves; i++) {
        if (chave == -1) {
            printf("\n=======================================================\n");
            printf("--- TESTANDO CHAVE [%d/20]: %d ---", i+1, chavesBusca[i]);
            printf("\n=======================================================\n");
        }
        
        // Se a flag -P foi passada, passa 1 apenas na última iteração para não imprimir o arquivo 20 vezes //
        int impAtual = (imprimir && i == numChaves - 1) ? 1 : 0;

        switch(metodo) {
            case 1:
                executarMetodo1(quantidade, situacao, chavesBusca[i], impAtual, nomeArq, &metricas);
                break;
            case 2:
                executarMetodo2(quantidade, situacao, chavesBusca[i], impAtual, nomeArq, &metricas);
                break;
            case 3: 
                executarMetodo3(quantidade, situacao, chavesBusca[i], impAtual, nomeArq, &metricas);
                break;
            case 4:
                executarMetodo4(quantidade, situacao, chavesBusca[i], impAtual, nomeArq, &metricas);
                break;
        }
        
        // Acumulando as métricas de pesquisa retornadas da função //
        if (chave == -1) {
            tempoTotal += metricas.tempoExecucao;
            transfTotal += metricas.numTransferencias;
            compTotal += metricas.numComparacoes;
        }
    }

    // Imprime as médias consolidadas no final do programa //
    if (chave == -1) {
        printf("\n\n=============== RESULTADO DO EXPERIMENTO ==============\n");
        printf("Metodo Escolhido: %d | Quantidade: %d | Situacao: %d\n", metodo, quantidade, situacao);
        printf("Media de Transferencias da Pesquisa: %ld\n", transfTotal / numChaves);
        printf("Media de Comparacoes da Pesquisa: %ld\n", compTotal / numChaves);
        printf("Tempo Medio da Pesquisa: %.6f segundos\n", tempoTotal / numChaves);
        printf("=======================================================\n");
    }

    return 0; 
}