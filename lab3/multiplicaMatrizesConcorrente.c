#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"
#define SALVA_TEMPOS_EXECUCAO

typedef struct {
    int linhas, colunas;
    float *dados;
} Matriz;

int nthreads;
double inicio, fim, delta;
Matriz *matriz1, *matriz2, *matrizResultante;

int leMatrizDeArquivoBinario(const char *nomeArquivo, Matriz *matriz) {
    long long int tam; //qtde de elementos na matriz
    FILE * descritorArquivo; //descritor do arquivo de entrada
    size_t ret; //retorno da funcao de leitura no arquivo de entrada

    //abre o arquivo para leitura binaria
    descritorArquivo = fopen(nomeArquivo, "rb");
    if (!descritorArquivo) {fprintf(stderr, "Erro de abertura do arquivo\n"); return 1;}

    //le as dimensoes da matriz
    ret = fread(&matriz->linhas, sizeof(int), 1, descritorArquivo);
    if (!ret) {fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n"); return 2;}
    ret = fread(&matriz->colunas, sizeof(int), 1, descritorArquivo);
    if (!ret) {fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n"); return 3;}
    tam = matriz->linhas * matriz->colunas; //calcula a qtde de elementos da matriz

    //aloca memoria para a matriz
    matriz->dados = (float*) malloc(sizeof(float) * tam);
    if (!matriz->dados) {fprintf(stderr, "Erro de alocação de memória da matriz\n"); return 4;}

    //carrega a matriz de elementos do tipo float do arquivo
    ret = fread(matriz->dados, sizeof(float), tam, descritorArquivo);
    if (ret < tam) {fprintf(stderr, "Erro de leitura dos elementos da matriz\n");return 5;}

    //finaliza o uso das variaveis
    fclose(descritorArquivo);
    return 0;
}

void *multiplicaMatrizesPorLinhas(void *threadId) {
    long int threadID = (long int) threadId;

    //Cada thread começa processando a linha de seu ID e processam as próximas linhas de nthreads em nthreads
    for (long int indiceLinha = threadID; indiceLinha < matriz1->linhas; indiceLinha += nthreads) {
        for (long int indiceColuna = 0; indiceColuna < matriz2->colunas; indiceColuna++) {
            matrizResultante->dados[indiceLinha * matriz2->colunas + indiceColuna] = 0.0;
            for (long int indiceElemLinhaColuna = 0; indiceElemLinhaColuna < matriz1->colunas; indiceElemLinhaColuna++) {
                float resultMultElemLinhaColuna = matriz1->dados[indiceLinha * matriz1->colunas + indiceElemLinhaColuna] * matriz2->dados[indiceElemLinhaColuna * matriz2->colunas + indiceColuna];
                matrizResultante->dados[indiceLinha * matriz2->colunas + indiceColuna] += resultMultElemLinhaColuna;
            }
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

    GET_TIME(inicio);
    if (argc < 5) {printf("Uso: %s <arquivo da matriz 1> <arquivo da matriz 2> <arquivo da matriz de saída> <quantidade de threads>\n", argv[0]); return 1;}

    //aloca memória para matrizes de entrada
    matriz1 = (Matriz*) malloc(sizeof(Matriz));
    if(matriz1 == NULL) {printf("Erro de alocação de memória para a matriz"); return 2;}
    matriz2 = (Matriz*) malloc(sizeof(Matriz));
    if(matriz2 == NULL) {printf("Erro de alocação de memória para a matriz"); return 2;}

    //le as matrizes dos arquivos de entrada
    if (leMatrizDeArquivoBinario(argv[1], matriz1) != 0 || leMatrizDeArquivoBinario(argv[2], matriz2) != 0) {
        printf("Erro na leitura das matrizes de entrada"); return 3;
    }
    
    //le o número de threads da entrada do usuário
    nthreads = atoi(argv[4]);
    if (nthreads > matriz1->linhas) nthreads = matriz1->linhas;

    // Aloca espaço para os identificadores das threads no sistema
    pthread_t *tid_sistema;
    tid_sistema = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if (tid_sistema == NULL) {printf("Erro na alocação de memória para identificadores das threads no sistema\n"); exit(-1);}

    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempos de execução - Concorrente:\n\n");
    printf("Tempo de inicialização: %lf\n", delta);

    //salva tempo de inicialização
    #ifdef SALVA_TEMPOS_EXECUCAO
    FILE *descritorArquivoTemposExecucao = fopen("temposExecucao.csv", "a");
    if (descritorArquivoTemposExecucao == NULL) {fprintf(stderr, "Erro ao abrir arquivo de tempos de execução"); exit(-1);}
    fprintf(descritorArquivoTemposExecucao, "%d, %d, %f,", nthreads, matriz1->colunas, delta);
    #endif

    GET_TIME(inicio);
    //verificar se as dimensões das matrizes combinam para multiplicação
    if (matriz1->colunas != matriz2->linhas) {printf("Erro durante a verificação das dimensões das matrizes, incompatíveis para multiplicação.\n"); return 4;}

    //criar, alocar memória e inserir informações na matriz resultante
    matrizResultante = (Matriz*) malloc(sizeof(Matriz));
    if(matriz2 == NULL) {printf("Erro de alocação de memória para a matriz"); return 5;}

    matrizResultante->linhas = matriz1->linhas;
    matrizResultante->colunas = matriz2->colunas;
    long long int qtdeElemenMatResult = matrizResultante->linhas * matrizResultante->colunas; //qtde de elementos na matriz resultante
    matrizResultante->dados = (float *)malloc(sizeof(float) * qtdeElemenMatResult);
    if(matrizResultante->dados == NULL) {printf("Erro na alocação de memória para a matriz resultante"); return 5;}

    //cria as threads
    for (long int i = 0; i < nthreads; i++) {
        if (pthread_create(&tid_sistema[i], NULL, multiplicaMatrizesPorLinhas, (void*) i)) {
            printf("Erro na criação de threads\n"); exit(-1);
        }
    }

    //espera todas as threads terminarem
    for (int i = 0; i < nthreads; i++) {pthread_join(tid_sistema[i], NULL);}

    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo de multiplicação: %lf\n", delta);

    //salva tempo de multiplicação
    #ifdef SALVA_TEMPOS_EXECUCAO
    fprintf(descritorArquivoTemposExecucao, " %f,", delta);
    #endif

    GET_TIME(inicio);
    //abrir arquivo de saída para escrever dados do resultado das duas matrizes
    FILE * descritorArquivo; //descritor do arquivo de saida
    size_t ret; //retorno da funcao de escrita no arquivo de saida
    descritorArquivo = fopen(argv[3], "wb"); //abre o arquivo para escrita binaria
    if(!descritorArquivo) {fprintf(stderr, "Erro de abertura do arquivo\n"); return 6;}

    //escrever número de linhas e colunas da matriz resultante
    //escreve numero de linhas e de colunas
    ret = fwrite(&matrizResultante->linhas, sizeof(int), 1, descritorArquivo);
    ret = fwrite(&matrizResultante->colunas, sizeof(int), 1, descritorArquivo);
    //escreve os elementos da matriz
    ret = fwrite(matrizResultante->dados, sizeof(float), qtdeElemenMatResult, descritorArquivo);
    if(ret < qtdeElemenMatResult) {fprintf(stderr, "Erro de escrita no  arquivo\n"); return 7;}

    //libera a memória alocada para as matrizes
    fclose(descritorArquivo);
    free(matriz1->dados); free(matriz1);
    free(matriz2->dados); free(matriz2);
    free(matrizResultante->dados); free(matrizResultante);

    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo de finalização: %lf\n", delta);

    //salva tempo de finalização e fecha o arquivo
    #ifdef SALVA_TEMPOS_EXECUCAO
    fprintf(descritorArquivoTemposExecucao, " %f\n", delta);
    fclose(descritorArquivoTemposExecucao);
    #endif

    return 0;
}
