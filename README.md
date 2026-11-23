<div align="center">
  <h1>🔍 Pesquisa Externa — Análise Experimental de Estruturas de Dados</h1>
  <p>Implementação modular em C e estudo comparativo de desempenho entre métodos de indexação e pesquisa em memória secundária aplicados a arquivos binários de até 2.000.000 de registros (~10 GB).</p>
</div>

<br/>

## 📖 Sobre o Projeto
Desenvolvido para a disciplina de **Estrutura de Dados II (BCC203)** da **Universidade Federal de Ouro Preto (UFOP)**, este projeto implementa e avalia experimentalmente quatro métodos clássicos de **Pesquisa em Memória Externa** manipulando grandes arquivos binários em disco.

📄 **[Acesse o Relatório Técnico Completo (PDF)](./RelatorioFinal.pdf)**.

### Estrutura dos Registros (`registros.bin`)
Os experimentos utilizam arquivos binários gerados sinteticamente contendo volumes de $N \in \{100, 200, 2.000, 20.000, 200.000, 2.000.000\}$ registros de tamanho fixo (~5 KB cada, alcançando **~10 GB** no maior volume):
* **Chave de pesquisa:** valor inteiro (`int`)
* **Dado 1:** valor inteiro longo (`long`)
* **Dado 2:** cadeia de `5.000` caracteres (`char[5000]`)

---

## ⚙️ Métodos de Pesquisa Implementados

1. **Acesso Sequencial Indexado (ASI):**
   * Exige arquivo ordenado ascendentemente. Agrupa os registros em **páginas de 4 itens** e mantém uma tabela esparsa de índices alocada dinamicamente na memória principal ($\lceil N/4 \rceil$ entradas).
   * Realiza apenas **1 transferência de página** por busca bem-sucedida, com varredura sequencial no índice ($O(N)$ comparações).

2. **Árvore Binária de Pesquisa Externa (ABP):**
   * Estrutura armazenada integralmente em disco (`abp.bin`), onde cada nó guarda o registro completo e apontadores de posição (`menor` e `maior`) para os filhos no arquivo.
   * Sem balanceamento: degenera em lista encadeada quando submetida a arquivos previamente ordenados.

3. **Árvore B:**
   * Índice multicaminho balanceado mantido em memória principal, armazenando chaves e referências diretas de posição no arquivo binário.
   * Garante altura logarítmica e **1 única leitura em disco** para recuperar o registro completo após localizar a chave.

4. **Árvore $B^{*}$:**
   * Variação da Árvore B em que **todos os registros de índice residem nas folhas** (páginas externas), enquanto páginas internas atuam exclusivamente como guias de roteamento.
   * Utiliza divisão de páginas e redistribuição entre folhas irmãs para manter alta taxa de ocupação e estabilidade logarítmica independentemente da ordem de inserção.

---

## 📊 Resultados Experimentais (2.000.000 de Registros — ~10 GB)

A metodologia consiste na média aritmética de **20 buscas automáticas** por chaves distintas garantidamente presentes no arquivo, avaliadas em entradas crescentes, decrescentes e aleatórias.

### Comparação na Pesquisa ($N = 2.000.000$ — Ordem Ascendente)

| Método | Transferências (Leituras) | Comparações (Média) | Tempo Médio de Pesquisa | Observação de Arquitetura |
| :--- | :---: | :---: | :---: | :--- |
| **Acesso Sequencial Indexado** | **`1`** | `253.576` | `0,000819 s` | Exige arquivo ordenado ascendentemente |
| **Árvore Binária Externa (ABP)** | *Inviável* | *Inviável* | *Inviável* | Degenera em lista encadeada para $N > 10.000$ ordenado |
| **Árvore B** | **`1`** | **`21`** | **`0,000054 s`** | Estável e logarítmica em qualquer ordem inicial |
| **Árvore $B^{*}$** | **`1`** | `22` | `0,000439 s` | Folhas sequenciais + acesso direto ao registro em disco |

### Comparação na Pesquisa ($N = 2.000.000$ — Ordem Aleatória)

| Método | Transferências (Leituras) | Comparações (Média) | Tempo Médio de Pesquisa |
| :--- | :---: | :---: | :---: |
| **Acesso Sequencial Indexado** | *N/A* | *N/A* | *N/A* |
| **Árvore Binária Externa (ABP)** | `23` *(em 200k)* | `67` *(em 200k)* | `0,000430 s` *(em 200k)* |
| **Árvore B** | **`1`** | **`22`** | **`0,000047 s`** |
| **Árvore $B^{*}$** | **`1`** | `24` | `0,000139 s` |

### Principais Conclusões:
* **Estruturas Balanceadas vs. ABP:** Enquanto a Árvore Binária Externa exige dezenas a milhares de acessos a disco por consulta (e torna-se inviável em arquivos ordenados), as **Árvores B e $B^{*}$** garantem **1 única leitura em disco** e cerca de **21 a 24 comparações** mesmo em bases de 2 milhões de registros.
* **Trade-off do ASI:** O Acesso Sequencial Indexado iguala o custo mínimo de E/S (1 leitura de página), mas a busca linear no índice esparso eleva as comparações para ~253 mil no maior volume.

---

## 🚀 Como Compilar e Executar

### 1. Compilação
```bash
# Clone o repositório
git clone [https://github.com/stephanyft/NOME-DO-REPOSITORIO.git](https://github.com/stephanyft/NOME-DO-REPOSITORIO.git)
cd NOME-DO-REPOSITORIO

# Compile os módulos em C
gcc -std=c99 -Wall -Wextra *.c -o pesquisa