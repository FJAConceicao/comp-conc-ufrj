# Instruções para Compilação e Execução

## Compilação dos Programas

1. **Compilação do Gerador de Matrizes Binárias**  
   Para compilar o programa que gera matrizes aleatórias:
   ```bash
   gcc geraMatrizBinario.c -o geraMatrizBinario.out -Wall
   ```

2. **Compilação da Multiplicação Sequencial de Matrizes**  
   Para compilar o programa de multiplicação sequencial de matrizes:
   ```bash
   gcc multiplicaMatrizesSequencial.c -o multiplicaMatrizesSequencial -Wall
   ```

3. **Compilação da Multiplicação Concorrente de Matrizes**  
   Para compilar o programa de multiplicação concorrente de matrizes:
   ```bash
   gcc multiplicaMatrizesConcorrente.c -o multiplicaMatrizesConcorrente -Wall
   ```

---

## Execução dos Programas

### 1. Gerador de Matriz Aleatória
   Para gerar uma matriz aleatória, execute o seguinte comando:
   ```bash
   ./geraMatrizBinario <número de linhas> <número de colunas> <arquivo de saída>
   ```

### 2. Multiplicação de Matrizes

- **Multiplicação Sequencial de Matrizes**  
  Para multiplicar duas matrizes sequencialmente:
  ```bash
  ./multiplicaMatrizesSequencial <arquivo da matriz 1> <arquivo da matriz 2> <arquivo da matriz de saída>
  ```

- **Multiplicação Concorrente de Matrizes**  
  Para multiplicar duas matrizes utilizando múltiplas threads:
  ```bash
  ./multiplicaMatrizesConcorrente <arquivo da matriz 1> <arquivo da matriz 2> <arquivo da matriz de saída> <quantidade de threads>
  ```

**OBS.:** Com a definição SALVA_TEMPOS_EXECUCAO será gerado um arquivo de saída temposExecucao.csv com os tempos de execução, caso contrário é necessário comentar o #define SALVA_TEMPOS_EXECUCAO no ínicio do código sequencial ou concorrente. O arquivo temposExecucao.csv salva quantidade de threads (zero para sequencial), as dimensões das matrizes e os tempos de execução (inicialização, processamento e finalização).

---

## Script sh para geração dos tempos de execução

O script `geraTemposExecucao.sh` serve para:

1. Criar as matrizes de entrada com os dimensões 500x500, 1000x1000 e 2000x2000 usando `./geraMatrizBinario`;
2. Executar `./multiplicaMatrizesSequencial` para multiplicação sequencial de matrizes 5 vezes para cada dimensão especificada e salva os tempos de execução no arquivo temposExecucao.csv;
3. Executar `./multiplicaMatrizesSequencial` para multiplicação sequencial de matrizes 5 vezes para cada dimensão especificada com diferentes números de threads e salva os tempos de execução no arquivo temposExecucao.csv.

Dessa forma os tempos de execução podem ser gerados para análise de desempenho (explicado na seção abaixo).

---

## Análise de Desempenho

Há um notebook Jupyter que realiza a análise de desempenho dos programas utilizando Python e as bibliotecas:

- **Pandas**
- **Matplotlib**
- **Seaborn**
- **Numpy**

O notebook calcula os tempos médios de execução para diferentes dimensões de matrizes (500, 1000 e 2000) e diferentes configurações de threads. Além disso, ele gera as métricas de **Aceleração** e **Eficiência**.
