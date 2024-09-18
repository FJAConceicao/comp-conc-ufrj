#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

// Variáveis globais
long int n;
float *vetor1, *vetor2;
int nthreads;

// Função executada pelas threads
void *CalcProdInt(void *tid) {
    long int id = (long int) tid;
    int ini, fim, bloco;
    float *ret; // produto interno calculado de maneira local
    double prodIntParcial = 0.0;

    bloco = n/nthreads;
    ini = id*bloco;
    fim = ini+bloco;
    if (id == (nthreads - 1)) fim = n;

    for (int i = ini; i < fim; i++) {
        prodIntParcial += vetor1[i] * vetor2[i];
    }

    //retorna o resultado do produto interno 
    ret = malloc(sizeof(float));
    if (ret!=NULL) *ret = prodIntParcial;
    else printf("--ERRO: malloc() thread\n");
    pthread_exit((void*) ret);
}

int main(int argc, char *argv[]) {
    FILE *arq; //arquivo de entrada
    size_t ret; //retorno da funcao de leitura no arquivo de entrada
    double prodIntGlobal = 0.0;
    float *prodIntRetThread; //Para pegar valor parcial do produto interno retornado pelas threads

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
        fprintf(stderr, "--ERRO: Leitura da dimensão dos vetores\n");
        return 3;
    }

    // Aloca espaço de memória e carrega os vetores de entrada
    int tamanhoVetores = sizeof(float)*n;
    vetor1 = malloc(tamanhoVetores);
    vetor2 = malloc(tamanhoVetores);
    if (vetor1 == NULL || vetor2 == NULL) {
        printf("--ERRO: Alocação de memória para os vetores\n"); exit(-1);
    }
    ret = fread(vetor1, sizeof(float), n, arq);
    if (ret < n) {
        fprintf(stderr, "--ERRO: Leitura dos elementos do vetor 1\n");
        return 4;
    }

    ret = fread(vetor2, sizeof(float), n, arq);
    if (ret < n) {
        fprintf(stderr, "--ERRO: Leitura dos elementos do vetor 2\n");
        return 4;
    }

    // Lê o número de threads da entrada do usuário
    nthreads = atoi(argv[2]);
    if (nthreads > n) nthreads = n;

    // Aloca espaço para os identificadores das threads no sistema
    pthread_t *tid_sistema;
    tid_sistema = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if (tid_sistema == NULL) {
        printf("--ERRO: malloc()\n"); exit(-1);
    }

    // Cria as threads
    for (long int i = 0; i < nthreads; i++) {
        if (pthread_create(&tid_sistema[i], NULL, CalcProdInt, (void*) i)) {
            printf("--ERRO: Criação de threads\n"); exit(-1);
        }
    }

    // Espera todas as threads terminarem
    for (int i = 0; i < nthreads; i++) {
        if (pthread_join(tid_sistema[i], (void *) &prodIntRetThread)) {
            printf("--ERRO: Espera da finalização das threads\n"); exit(-1);
        }
        prodIntGlobal += *prodIntRetThread; free(prodIntRetThread);
    }

    // Lê o produto interno registrado no arquivo
    double prodIntArq;
    ret = fread(&prodIntArq, sizeof(double), 1, arq);
    if (ret == 0) {
        printf("--ERRO: Leitura do produto interno registrado no arquivo\n"); exit(-1);
    }

    // Imprime os resultados
    printf("Calculos do Produto Interno:\n");
    printf("De maneira concorrente..........: %.26f\n", prodIntGlobal);
    printf("Registrado no arquivo...........: %.26lf\n", prodIntArq);

    // Calcula e exibe variância relativa
    float varianciaRelativa = (fabs(prodIntArq - prodIntGlobal) / fabs(prodIntArq)) * 100;
    printf("Variância Relativa: %.14f\n", varianciaRelativa);

    // Desaloca os espaços de memória
    free(vetor1);
    free(vetor2);
    free(tid_sistema);
    fclose(arq);

    return 0;
}