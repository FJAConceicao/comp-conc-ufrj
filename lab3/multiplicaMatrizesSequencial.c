#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int linhas, colunas;
    float *dados;
} Matriz;

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

int main(int argc, char *argv[]) {

    if (argc < 4) {printf("Uso: %s <arquivo da matriz 1> <arquivo da matriz 2> <arquivo da matriz de saída>\n", argv[0]); return 1;}

    //aloca memória para matrizes de entrada
    Matriz *matriz1 = (Matriz*) malloc(sizeof(Matriz));
    if(matriz1 == NULL) {printf("Erro de alocação de memória para a matriz"); return 2;}
    Matriz *matriz2 = (Matriz*) malloc(sizeof(Matriz));
    if(matriz2 == NULL) {printf("Erro de alocação de memória para a matriz"); return 2;}

    //le as matrizes dos arquivos de entrada
    if (leMatrizDeArquivoBinario(argv[1], matriz1) != 0 || leMatrizDeArquivoBinario(argv[2], matriz2) != 0) {
        printf("Erro na leitura das matrizes de entrada"); return 3;
    }

    //verificar se as dimensões das matrizes combinam para multiplicação
    if (matriz1->colunas != matriz2->linhas) {printf("Erro durante a verificação das dimensões das matrizes, incompatíveis para multiplicação.\n"); return 4;}

    //criar, alocar memória e inserir informações na matriz resultante
    Matriz *matrizResultante = (Matriz*) malloc(sizeof(Matriz));
    if(matriz2 == NULL) {printf("Erro de alocação de memória para a matriz"); return 5;}

    matrizResultante->linhas = matriz1->linhas;
    matrizResultante->colunas = matriz2->colunas;
    long long int qtdeElemenMatResult = matrizResultante->linhas * matrizResultante->colunas; //qtde de elementos na matriz resultante
    matrizResultante->dados = (float *)malloc(sizeof(float) * qtdeElemenMatResult);
    if(matrizResultante->dados == NULL) {printf("Erro na alocação de memória para a matriz resultante"); return 5;}

    //multiplicar as matrizes de maneira sequencial
    for (long int indiceLinha = 0; indiceLinha < matriz1->linhas; indiceLinha++) {
        for (long int indiceColuna = 0; indiceColuna < matriz2->colunas; indiceColuna++) {
            matrizResultante->dados[indiceLinha * matriz2->colunas + indiceColuna] = 0.0;
            for (long int indiceElemLinhaColuna = 0; indiceElemLinhaColuna < matriz1->colunas; indiceElemLinhaColuna++) {
                float resultMultElemLinhaColuna = matriz1->dados[indiceLinha * matriz1->colunas + indiceElemLinhaColuna] * matriz2->dados[indiceElemLinhaColuna * matriz2->colunas + indiceColuna];
                matrizResultante->dados[indiceLinha * matriz2->colunas + indiceColuna] += resultMultElemLinhaColuna;
            }
        }
    }

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

    return 0;
}
