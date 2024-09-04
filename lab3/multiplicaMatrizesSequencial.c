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
    if (!descritorArquivo) {
        fprintf(stderr, "Erro de abertura do arquivo\n");
        return 2;
    }

    //le as dimensoes da matriz
    ret = fread(&matriz->linhas, sizeof(int), 1, descritorArquivo);
    if (!ret) {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
        return 3;
    }
    ret = fread(&matriz->colunas, sizeof(int), 1, descritorArquivo);
    if (!ret) {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
        return 3;
    }
    tam = matriz->linhas * matriz->colunas; //calcula a qtde de elementos da matriz

    //aloca memoria para a matriz
    matriz->dados = (float*) malloc(sizeof(float) * tam);
    if (!matriz->dados) {
        fprintf(stderr, "Erro de alocação de memória da matriz\n");
        return 3;
    }

    //carrega a matriz de elementos do tipo float do arquivo
    ret = fread(matriz->dados, sizeof(float), tam, descritorArquivo);
    if (ret < tam) {
        fprintf(stderr, "Erro de leitura dos elementos da matriz\n");
        free(matriz->dados);
        return 4;
    }

    //finaliza o uso das variaveis
    fclose(descritorArquivo);
    return 0;
}

int main(int argc, char *argv[]) {

    if (argc < 4) {
        printf("Uso: %s <arquivo da mattriz 1> <arquivo da matriz 2> <arquivo da matriz de saída>\n", argv[0]);
        return 1;
    }

    //aloca memória para matrizes de entrada
    Matriz *matriz1 = (Matriz*) malloc(sizeof(Matriz));
    if(matriz1 == NULL) {printf("Erro de alocação de memória para a matriz"); return 2;}
    Matriz *matriz2 = (Matriz*) malloc(sizeof(Matriz));
    if(matriz2 == NULL) {printf("Erro de alocação de memória para a matriz"); return 2;}

    //le as matrizes dos arquivos de entrada
    if (leMatrizDeArquivoBinario(argv[1], &matriz1) != 0 || leMatrizDeArquivoBinario(argv[2], &matriz2) != 0) {
        printf("Erro na leitura das matrizes de entrada"); return 3;
    }

    //verificar se as dimensões das matrizes combinam para multiplicação

    //criar, alocar memória e inserir informações na matriz resultante

    //multiplicar as matrizes de maneira sequencial

    //abrir arquivo de saída para escrever dados do resultado das duas matrizes

    //escrever número de linhas e colunas da matriz resultante

    //escrever elementos da matriz

    //libera a memória alocada para as matrizes
    
    return 0;
}
