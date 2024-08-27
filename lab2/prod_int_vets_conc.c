#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Variáveis globais
long int n;
float *vet1, *vet2;
int nthreads;
double produto_concorrente_global = 0.0;

// Função executada pelas threads
void *ProdutoInterno(void *tid) {
    long int id = (long int) tid;
    int ini, fim, bloco;
    double produto_local = 0.0;

    bloco = n/nthreads;
    ini = id*bloco;
    fim = ini+bloco;
    if (id == (nthreads - 1)) fim = n;

    for (int i = ini; i < fim; i++) {
        produto_local += vet1[i] * vet2[i];
    }

    //retorna o resultado do produto interno 
    ret = malloc(sizeof(float));
    if (ret!=NULL) *ret = produto_local;
    else printf("--ERRO: malloc() thread\n");
    pthread_exit((void*) ret);
}

int main(int argc, char *argv[]) {
    FILE *arq;
    size_t ret;

    double produto_interno_arquivo;
    pthread_t *tid_sistema;

    // Valida e recebe os valores de entrada
    if (argc < 3) {
        printf("Use: %s <arquivo de entrada> <numero de threads>\n", argv[0]);
        exit(-1);
    }

    // Abre o arquivo de entrada com os vetores
    arq = fopen(argv[1], "rb");
    if (arq == NULL) {
        printf("--ERRO: fopen()\n");
        exit(-1);
    }

    // Lê o tamanho dos vetores
    ret = fread(&n, sizeof(long int), 1, arq);
    if (!ret) {
        fprintf(stderr, "Erro de leitura da dimensão dos vetores\n");
        return 3;
    }

    // Aloca espaço de memória e carrega os vetores de entrada
    int tamanhoVetores = sizeof(float)*n;
    vet1 = malloc(tamanhoVetores);
    vet2 = malloc(tamanhoVetores);
    if (vet1 == NULL || vet2 == NULL) {
        printf("--ERRO: malloc()\n");
        exit(-1);
    }
    ret = fread(vet1, sizeof(float), n, arq);
    if (ret < n) {
        fprintf(stderr, "Erro de leitura dos elementos do vetor 1\n");
        return 4;
    }

    ret = fread(vet2, sizeof(float), n, arq);
    if (ret < n) {
        fprintf(stderr, "Erro de leitura dos elementos do vetor 2\n");
        return 4;
    }

    // Lê o número de threads da entrada do usuário
    nthreads = atoi(argv[2]);
    if (nthreads > n) nthreads = n;

    // Aloca espaço para os identificadores das threads no sistema
    tid_sistema = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if (tid_sistema == NULL) {
        printf("--ERRO: malloc()\n");
        exit(-1);
    }

    // Cria as threads
    for (long int i = 0; i < nthreads; i++) {
        if (pthread_create(&tid_sistema[i], NULL, ProdutoInterno, (void*) i)) {
            printf("--ERRO: pthread_create()\n");
            exit(-1);
        }
    }

    // Espera todas as threads terminarem
    for (int i = 0; i < nthreads; i++) {
        if (pthread_join(tid_sistema[i], NULL)) {
            printf("--ERRO: pthread_join()\n");
            exit(-1);
        }
    }

    // Lê o produto interno registrado no arquivo
    ret = fread(&produto_interno_arquivo, sizeof(double), 1, arq);
    if (ret == 0) {
        printf("--ERRO: fread()\n");
        exit(-1);
    }

    // Imprime os resultados
    printf("Produto Interno calculado de maneira concorrente: %.26f\n", produto_concorrente_global);
    printf("Produto Interno registrado no arquivo...........: %.26lf\n", produto_interno_arquivo);

    // Calcula e exibe variância relativa
    float varRelativa = ((produtoInternoArquivo - produtoInternoGlobal) / produtoInternoArquivo) * 100;
    printf("Variância Relativa: %.14f\n", varRelativa);

    // Desaloca os espaços de memória
    free(vet1);
    free(vet2);
    free(tid_sistema);
    fclose(arq);

    return 0;

}