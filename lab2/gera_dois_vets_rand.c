#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 1000 // valor máximo de um elemento do vetor
#define TEXTO // para imprimir vetores gerados gerados e produto interno calculado

int main(int argc, char* argv[]) {
    float *vetor1, *vetor2; //vetores que serão gerados
    long int n; //qtde de elementos dos vetores que serão gerados
    double prodInt = 0.0; //guarda valor do produto interno dos vetores gerados
    int fator = 1; //fator multiplicador para gerar números negativos
    FILE *descritorArquivo; //descritor do arquivo de saida
    size_t ret; //retorno da funcao de escrita no arquivo de saida

    // Recebe os argumentos de entrada
    if (argc < 3) {
        fprintf(stderr, "Digite: %s <dimensao> <arquivo saida>\n", argv[0]);
        return 1;
    }
    n = atoi(argv[1]);

    // Aloca memória para os vetores
    int tamanhoVetores = sizeof(float)*n;
    vetor1 = (float*) malloc(tamanhoVetores);
    vetor2 = (float*) malloc(tamanhoVetores);
    if (!vetor1 || !vetor2) {
        fprintf(stderr, "--ERRO: Na alocação de memória dos vetores\n");
        return 2;
    }

    // Preenche os vetores com valores float aleatórios e calcula o produto interno
    srand(time(NULL));
    for (long int i = 0; i < n; i++) {
        vetor1[i] = (rand() % MAX) / 3.0 * fator;
        vetor2[i] = (rand() % MAX) / 3.0 * fator;
        prodInt += vetor1[i] * vetor2[i];
        fator *= -1;
    }

    // Escreve os vetores e o produto interno no arquivo binário
    descritorArquivo = fopen(argv[2], "wb");
    if (!descritorArquivo) {
        fprintf(stderr, "--ERRO: Abertura do arquivo\n");
        return 3;
    }

    //escreva a dimensão no arquivo
    ret = fwrite(&n, sizeof(long int), 1, descritorArquivo);

    ret = fwrite(vetor1, sizeof(float), n, descritorArquivo);
    if (ret < n) {
        fprintf(stderr, "--ERRO: Escrita do vetor 1 no arquivo\n");
        return 5;
    }

    ret = fwrite(vetor2, sizeof(float), n, descritorArquivo);
    if (ret < n) {
        fprintf(stderr, "--ERRO: Escrita do vetor 2 no arquivo\n");
        return 5;
    }

    ret = fwrite(&prodInt, sizeof(double), 1, descritorArquivo);

    #ifdef TEXTO
    fprintf(stdout, "%ld\n", n);
    for(long int i=0; i<n; i++) { //mostra valores do vetor1
        fprintf(stdout, "%f ", vetor1[i]);
    }

    fprintf(stdout, "\n");
    for(long int i=0; i<n; i++) { //mostra valores do vetor2
        fprintf(stdout, "%f ", vetor2[i]);
    }

    fprintf(stdout, "\nProduto Interno = %lf\n", prodInt);
    #endif

    // Finaliza o uso das variáveis
    fclose(descritorArquivo);
    free(vetor1);
    free(vetor2);

    return 0;
}
