/* Disciplina: Programacao Concorrente */
/* Profa.: Silvana Rossetto */
/* Laboratório: 1 */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

// Definindo uma struct para os parâmetros da thread
typedef struct {
    int *vetor;
    int indiceInicio;
    int indiceFim;
} t_Args;

// Função para exibir valores do vetor
void imprimirVetor(int vetor[], int tamanho) {
    printf("Vetor: ");
    for (int i = 0; i < tamanho; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\n");
}

// Função para inicializar o vetor
// TODO: Implementar geração de maneira aleatória
void inicializaVetor(int* vetor, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        vetor[i] = i;
    }
    imprimirVetor(vetor, tamanho); //TODO: Ver de colocar essa função para a main
}

// Função que cada thread irá executar
void* incrementaUm(void* arg) {
    t_Args *argsThread = (t_Args*)arg;

    for (int i = argsThread->indiceInicio; i < argsThread->indiceFim; i++) {
        argsThread->vetor[i] += 1;
    }

    pthread_exit(NULL);
}

// Função para verificar se o vetor foi incrementado corretamente
void checaVetor(int* vetorInicial, int* vetorParaVerificar, int tamanho) {
    imprimirVetor(vetorParaVerificar, tamanho); //TODO: Ver de colocar essa função para a main
    for (int i = 0; i < tamanho; i++) {
        if (vetorParaVerificar[i] != (vetorInicial[i] + 1)) {
            printf("Erro: elemento %d não foi incrementado corretamente\n", i);
            return;
        }
    }
    printf("Vetor processado corretamente!\n");
}

int main(int argc, char* argv[]) {

    if (argc != 3) {
        printf("Uso: %s <N> <M>\n", argv[0]);
        printf("  N: Número de elementos do vetor\n");
        printf("  M: Número de threads a serem utilizadas\n");
        return 1;
    }

    int N = atoi(argv[1]);
    int M = atoi(argv[2]);
    int vetorInicial[N], vetor[N];

    // Inicializa vetorInicial e copia para vetor (Em que os indices serão incrementados)
    inicializaVetor(vetorInicial, N);
    for (int i = 0; i < N; i++) {
        vetor[i] = vetorInicial[i];
    }

    // Calcula quantidade de indices por thread e indices restantes
    int indicesPorThread = N / M;
    int resto = N % M;

    // Cria as threads
    pthread_t tid_sistema[M];
    t_Args argsThread[M];
    for (int indiceThread = 0; indiceThread < M; indiceThread++) {
        argsThread[indiceThread].vetor = vetor;

        if (indiceThread < resto) {
            argsThread[indiceThread].indiceInicio = indiceThread * indicesPorThread + indiceThread;
            argsThread[indiceThread].indiceFim = argsThread[indiceThread].indiceInicio + indicesPorThread + 1;
        } else {
            argsThread[indiceThread].indiceInicio = indiceThread * indicesPorThread + resto;
            argsThread[indiceThread].indiceFim = argsThread[indiceThread].indiceInicio + indicesPorThread;
        }

        pthread_create(&tid_sistema[indiceThread], NULL, incrementaUm, (void*)&argsThread[indiceThread]);
    }

    for (int indiceThread = 0; indiceThread < M; indiceThread++) {
        pthread_join(tid_sistema[indiceThread], NULL);
    }

    checaVetor(vetorInicial, vetor, N);

    return 0;
}
