#include "arvoreb.h"

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
    if(situacao < 1 || situacao > 3) {
        printf("Erro: situacao deve ser 1 (asc), 2 (des) ou 3 (aleatorio)\n");
        return 1;
    }
    switch(metodo) {
        case 1:
            printf("Metodo 1 ainda nao implemtnado\n");
            break;
        case 2:
            printf("Metodo ainda nao implementado\n");
            break;
        case 3:
            FILE * arq = fopen("registros.bin", "rb");
            if(arq == NULL) {
                printf("Erro: nao foi possivel abrir o arquivo\n");
                return 1;
            }
            TipoApontador arvore;
            clock_t ini = clock();
            CriaArvore(arq, &arvore);
            clock_t fim = clock();
            double tempoCriacao = (double)(fim - ini) / CLOCKS_PER_SEC;

            long transfCriacao = transferencias;
            long compCriacao = comparacoes;
            transferencias = 0;
            comparacoes = 0;
             TipoRegistro item;
            Registro reg;
            item.Chave = chave;

            clock_t iniP = clock();
            int achou = Pesquisa(&item, arvore);
            if (achou) {
                fseek(arq, item.Posicao * (long)sizeof(Registro), SEEK_SET);
                fread(&reg, sizeof(Registro), 1, arq);
                transferencias++;
            }
            clock_t fimP = clock();
            double tempoPesquisa = (double)(fimP - iniP) / CLOCKS_PER_SEC;

            /* ---- Resultados ---- */
            if (achou) {
                printf("Chave %d encontrada.\n", chave);
                printf("dado1: %ld\n", reg.dado1);
                printf("dado2: %.50s\n", reg.dado2);
            } else {
                printf("Chave %d NAO encontrada.\n", chave);
            }

            printf("\n--- Criacao da arvore ---\n");
            printf("Transferencias: %ld\n", transfCriacao);
            printf("Comparacoes:    %ld\n", compCriacao);
            printf("Tempo:          %f s\n", tempoCriacao);

            printf("\n--- Pesquisa ---\n");
            printf("Transferencias: %ld\n", transferencias);
            printf("Comparacoes:    %ld\n", comparacoes);
            printf("Tempo:          %f s\n", tempoPesquisa);

            if (imprimir) {
                printf("\nChaves da arvore:\n");
                Imprime(arvore);
                printf("\n");
                
            }
            fclose(arq);
            break;
        }
        case 4:
            printf("Metodo 4 ainda nao implementado\n");
            break;
        default:
            printf("Erro: metodo deve ser númerado de 1 a 4\n");
            return 1;
    return 0; 
}