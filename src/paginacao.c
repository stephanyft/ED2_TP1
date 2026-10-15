#include "../headers/paginacao.h"

#include <stdlib.h>
#include <time.h>

static void zerarMetricas(AnaliseExperimental *metricas) {
    metricas->numTransferencias = 0;
    metricas->numComparacoes = 0;
    metricas->tempoExecucao = 0.0;
}

int criaTabelaIndices(FILE *arquivo, Indice **tabela, int *tamanhoTabela,AnaliseExperimental *metricas) {
    if (arquivo == NULL || tabela == NULL || tamanhoTabela == NULL ||
        metricas == NULL) {
        return 0;
    }

    clock_t inicio = clock();

    *tabela = NULL;
    *tamanhoTabela = 0;

    /* Posiciona no final para descobrir quantos registros existem no arquivo. */
    if (fseek(arquivo, 0, SEEK_END) != 0) {
        metricas->tempoExecucao = (double)(clock() - inicio) / CLOCKS_PER_SEC;
        return 0;
    }

    // pega o byte atual, como nesse caso o ponteiro vai estar no fim do arquivo, esse é o nosso tamanho em bytes do arquivo
    long tamanhoArquivo = ftell(arquivo);

    /* Um arquivo válido deve conter apenas registros Item completos. */
    if (tamanhoArquivo < 0 || tamanhoArquivo % (long)sizeof(Item) != 0) {
        metricas->tempoExecucao = (double)(clock() - inicio) / CLOCKS_PER_SEC;
        return 0;
    }

    rewind(arquivo);
    long int quantidadeRegistros = tamanhoArquivo / sizeof(Item);

    /* Divisão arredondada para cima, incluindo uma possível página incompleta. */
    long int quantidadePaginas = (quantidadeRegistros + ITENS_POR_PAGINA - 1) / ITENS_POR_PAGINA;

    /* Arquivo vazio é válido e produz uma tabela vazia. */
    if (quantidadePaginas == 0) {
        metricas->tempoExecucao = (clock() - inicio) / CLOCKS_PER_SEC;
        return 1;
    }

    /* Reserva uma entrada em memória principal para cada página do arquivo. */
    Indice *novaTabela = malloc(quantidadePaginas * sizeof(*novaTabela));
    if (novaTabela == NULL) {
        metricas->tempoExecucao = (double)(clock() - inicio) / CLOCKS_PER_SEC;
        return 0;
    }

    Item primeiroItem;
    int paginaAtual = 0;

    while (paginaAtual < quantidadePaginas) {
        /* Lê somente o primeiro registro da página para formar o índice. */
        if (fread(&primeiroItem, sizeof(Item), 1, arquivo) != 1) {
            break;
        }

        /* Cada leitura do arquivo conta como uma transferência externa. */
        metricas->numTransferencias++;
    
        /* Guarda a chave inicial e a posição lógica do começo da página. */
        novaTabela[paginaAtual].chave = primeiroItem.chave;
        novaTabela[paginaAtual].primeiroRegistro = paginaAtual * ITENS_POR_PAGINA;
        paginaAtual++;

        /*
         * Pula os outros três registros sem lê-los. Na última página não é
         * necessário saltar, pois ela pode possuir menos de quatro registros.
         */
        if (paginaAtual < quantidadePaginas && fseek(arquivo, (ITENS_POR_PAGINA - 1) * sizeof(Item), SEEK_CUR) != 0) {
            break;
        }
    }

    /* Uma falha de leitura ou uma tabela incompleta invalida a construção. */
    if (ferror(arquivo) || paginaAtual != quantidadePaginas) {
        free(novaTabela);
        metricas->tempoExecucao = (clock() - inicio) / CLOCKS_PER_SEC;
        return 0;
    }

    /* Entrega a tabela alocada e seu tamanho para a função chamadora. */
    *tabela = novaTabela;
    *tamanhoTabela = paginaAtual;
    metricas->tempoExecucao = (double)(clock() - inicio) / CLOCKS_PER_SEC;
    return 1;
}

/*
 * Pesquisa primeiro no índice em memória e depois apenas na página candidata
 * do arquivo, reduzindo a quantidade de acessos à memória externa.
 */
int pesquisaSequencialIndexada(FILE *arquivo, const Indice tabela[], int tamanhoTabela, int chave, Item *resultado, AnaliseExperimental *metricas) {
    /* Sem uma tabela válida não existe página que possa ser pesquisada. */
    if (arquivo == NULL || tabela == NULL || tamanhoTabela <= 0 || resultado == NULL || metricas == NULL) {
        return 0;
    }

    clock_t inicio = clock();
    int i = 0;

    /*
     * Localiza a última página cuja primeira chave é menor ou igual à chave
     * procurada. As comparações ocorrem em memória principal.
     */
    while (i < tamanhoTabela) {
        metricas->numComparacoes++;
        if (tabela[i].chave > chave) {
            break;
        }
        i++;
    }

    /* A chave é menor que a primeira chave do arquivo. */
    if (i == 0) {
        metricas->tempoExecucao = (double)(clock() - inicio) / CLOCKS_PER_SEC;
        return 0;
    }

    /* Como i avançou uma posição, a página candidata está em i - 1. */
    int paginaEscolhida = i - 1;

    /* Converte a posição lógica do registro em deslocamento de bytes. */
    long deslocamento = tabela[paginaEscolhida].primeiroRegistro * sizeof(Item);
    if (fseek(arquivo, deslocamento, SEEK_SET) != 0) {
        metricas->tempoExecucao = (clock() - inicio) / CLOCKS_PER_SEC;
        return 0;
    }

    /* Carrega somente a página candidata; a última pode estar incompleta. */
    Item pagina[ITENS_POR_PAGINA];
    size_t quantidadeLida =
        fread(pagina, sizeof(Item), ITENS_POR_PAGINA, arquivo);

    if (quantidadeLida == 0) {
        metricas->tempoExecucao = (clock() - inicio) / CLOCKS_PER_SEC;
        return 0;
    }

    /* Toda a página é obtida por uma única operação de leitura. */
    metricas->numTransferencias++;

    /* Faz a busca sequencial entre os registros carregados da página. */
    for (size_t j = 0; j < quantidadeLida; j++) {
        metricas->numComparacoes++;
        if (pagina[j].chave == chave) {
            /* Devolve ao chamador uma cópia completa do registro encontrado. */
            *resultado = pagina[j];
            metricas->tempoExecucao =
                (double)(clock() - inicio) / CLOCKS_PER_SEC;
            return 1;
        }
    }

    metricas->tempoExecucao = (double)(clock() - inicio) / CLOCKS_PER_SEC;
    return 0;
}

/* Exibe de maneira uniforme as métricas de uma etapa do método. */
static void imprimirMetricas(const char *etapa, const AnaliseExperimental *metricas) {
    printf("\n--- METRICAS DE DESEMPENHO DE %s ---\n", etapa);
    printf("Numero de transferencias: %d\n", metricas->numTransferencias);
    printf("Numero de comparacoes: %d\n", metricas->numComparacoes);
    printf("Tempo de execucao: %.5f segundos\n", metricas->tempoExecucao);
}

/* Coordena a criação do índice, a pesquisa e a apresentação dos resultados. */
void executarMetodo1(int quantidade, int situacao, int chaveBusca, int imprimir, const char *nomeArquivo, AnaliseExperimental *metricas) {
    /* A quantidade já foi usada para gerar o arquivo no programa principal. */
    (void)quantidade;

    /* O método depende da ordenação crescente das chaves. */
    if (situacao != 1) {
        printf("Erro: o acesso sequencial indexado requer arquivo ordenado ascendentemente\n");
        return;
    }

    /* O arquivo contém os registros; somente o índice permanece em memória. */
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        return;
    }

    /* As duas etapas precisam ter métricas independentes, conforme o TP. */
    AnaliseExperimental metricasCriacao;
    zerarMetricas(&metricasCriacao);
    zerarMetricas(metricas);

    Indice *tabela = NULL;
    int tamanhoTabela = 0;

    /* O endereço de tabela é passado para que a função possa alocá-la. */
    printf("\nConstruindo o indice esparso (Metodo 1)...\n");
    if (!criaTabelaIndices(arquivo, &tabela, &tamanhoTabela,
                           &metricasCriacao)) {
        printf("Erro ao construir a tabela de indices.\n");
        fclose(arquivo);
        return;
    }

    printf("Pesquisando pela chave %d...\n", chaveBusca);
    Item resultado;
    int encontrou = pesquisaSequencialIndexada(arquivo, tabela, tamanhoTabela,
                                                chaveBusca, &resultado, metricas);

    if (encontrou) {
        printf("\n--- REGISTRO ENCONTRADO! ---\n");
        printf("Chave: %d\n", resultado.chave);
        printf("Dado 1: %ld\n", resultado.dado1);
        printf("Dado 2: %.50s...\n", resultado.dado2);
    } else {
        printf("Chave %d nao encontrada.\n", chaveBusca);
    }

    imprimirMetricas("CRIACAO", &metricasCriacao);
    imprimirMetricas("PESQUISA", metricas);

    if (imprimir) {
        /* A opção -P apresenta somente as chaves existentes no arquivo. */
        printf("\n--- CHAVES DE PESQUISA DO ARQUIVO ---\n");
        rewind(arquivo);
        Item registro;
        while (fread(&registro, sizeof(Item), 1, arquivo) == 1) {
            printf("%d ", registro.chave);
        }
        printf("\n");
    }

    /* A tabela pertence a esta função após sua criação. */
    free(tabela);
    fclose(arquivo);
}
