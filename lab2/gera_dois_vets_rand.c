#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 1000 // valor máximo de um elemento do vetor
#define TEXTO

int main(int argc, char* argv[]) {
    float *vetor1, *vetor2;
    long int n;
    double produtoInterno = 0.0;
    int fator = 1;
    FILE *descritorArquivo;
    size_t ret;

    // Recebe os argumentos de entrada
    if (argc < 3) {
        fprintf(stderr, "Digite: %s <dimensao> <arquivo saida>\n", argv[0]);
        return 1;
    }
    n = atoi(argv[1]);

    // Aloca memória para os vetores
    vetor1 = (float*) malloc(sizeof(float) * n);
    vetor2 = (float*) malloc(sizeof(float) * n);
    if (!vetor1 || !vetor2) {
        fprintf(stderr, "Erro de alocação de memória dos vetores\n");
        return 2;
    }

    // Preenche os vetores com valores float aleatórios e calcula o produto interno
    srand(time(NULL));
    for (long int i = 0; i < n; i++) {
        vetor1[i] = (rand() % MAX) / 3.0 * fator;
        vetor2[i] = (rand() % MAX) / 3.0 * fator;
        produtoInterno += vetor1[i] * vetor2[i];
        fator *= -1;
    }

    // Escreve os vetores e o produto interno no arquivo binário
    descritorArquivo = fopen(argv[2], "wb");
    if (!descritorArquivo) {
        fprintf(stderr, "Erro de abertura do arquivo\n");
        return 3;
    }

    //escreva a dimensão no arquivo
    ret = fwrite(&n, sizeof(long int), 1, descritorArquivo);
    if (ret < n) {
        fprintf(stderr, "Erro de escrita da dimensão no arquivo\n");
        return 4;
    }

    ret = fwrite(vetor1, sizeof(float), n, descritorArquivo);
    if (ret < n) {
        fprintf(stderr, "Erro de escrita do vetor 1 no arquivo\n");
        return 5;
    }

    ret = fwrite(vetor2, sizeof(float), n, descritorArquivo);
    if (ret < n) {
        fprintf(stderr, "Erro de escrita do vetor 1 no arquivo\n");
        return 5;
    }

    ret = fwrite(&produtoInterno, sizeof(double), 1, descritorArquivo);
    if (ret < n) {
        fprintf(stderr, "Erro de escrita do produto interno no arquivo\n");
        return 6;
    }

    #ifdef TEXTO
    fprintf(stdout, "%ld\n", n);
    for(long int i=0; i<n; i++) { //mostra valores do vetor1
        fprintf(stdout, "%f ", vetor1[i]);
    }
    fprintf(stdout, "\n");
    for(long int i=0; i<n; i++) { //mostra valores do vetor2
        fprintf(stdout, "%f ", vetor2[i]);
    }
    fprintf(stdout, "\n");
    fprintf(stdout, "Produto Interno = %lf\n", produto_interno);
    #endif

    // Finaliza o uso das variáveis
    fclose(descritorArquivo);
    free(vetor1);
    free(vetor2);

    return 0;
}
