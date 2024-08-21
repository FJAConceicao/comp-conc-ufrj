/* Disciplina: Programacao Concorrente */
/* Profa.: Silvana Rossetto */
/* Laboratório: 1 */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

// Definindo uma struct para os parâmetros da thread
typedef struct {
    int *vetor;
    int indiceInicio, indiceFim;
} t_Args;

// Função para exibir valores do vetor
void imprimirVetor(int vetor[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\n");
}

// Função para inicializar o vetor com valores aleatórios entre 0 e 1000
void inicializaVetor(int* vetor, int tamanho) {
    srand(time(NULL));
    for (int i = 0; i < tamanho; i++) {
        vetor[i] = rand() % 1000;
    }
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
    for (int i = 0; i < tamanho; i++) {
        if (vetorParaVerificar[i] != (vetorInicial[i] + 1)) {
            printf("--ERRO: elemento %d não foi incrementado corretamente\n", i);
            return;
        }
    }
    printf("Vetor incrementado corretamente!\n");
}

int main(int argc, char* argv[]) {

    if (argc != 3) {
        printf("--ERRO: informe, respectivamente, a quantidade de elementos e a quantidade de threads do vetor: <%s> <N> <M>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    int M = atoi(argv[2]);

    // Alocando memória para os vetores
    int *vetorInicial = (int*)malloc(N * sizeof(int));
    int *vetorParaIncrementar = (int*)malloc(N * sizeof(int));

    // Verifica se a alocação foi bem-sucedida
    if (vetorInicial == NULL || vetorParaIncrementar == NULL) {
        printf("--ERRO: Na alocação de memória para os vetores.\n");
        return 1;
    }

    // Inicializa vetorInicial e copia para vetor (Em que os indices serão incrementados)
    inicializaVetor(vetorInicial, N);
    printf("Vetor Inicial.....: "); imprimirVetor(vetorInicial, N);
    for (int i = 0; i < N; i++) {
        vetorParaIncrementar[i] = vetorInicial[i];
    }

    // Calcula quantidade de indices por thread e indices restantes
    int indicesPorThread = N / M;
    int indicesRestantes = N % M;

    // Cria as threads
    pthread_t tid_sistema[M];
    t_Args *argsThread[M];
    for (int indiceThread = 0; indiceThread < M; indiceThread++) {

        argsThread[indiceThread] = malloc(sizeof(t_Args));
        if (argsThread[indiceThread] == NULL) {
            printf("--ERRO: malloc()\n"); 
            return 1;
        }

        argsThread[indiceThread]->vetor = vetorParaIncrementar;

        /*  O balanceamento é feito nessa estrutura condicional de if-else abaixo.

            No if, as primeiras indicesRestantes threads processem um indice a mais (indicesPorThread + 1)
            para compensar qualquer divisão não exata de N por M.

            No else, as threads restantes (a partir de indiceThread >= indicesRestantes) processam 
            apenas indicesPorThread elementos.
        */
        if (indiceThread < indicesRestantes) {
            argsThread[indiceThread]->indiceInicio = indiceThread * indicesPorThread + indiceThread;
            argsThread[indiceThread]->indiceFim = argsThread[indiceThread]->indiceInicio + indicesPorThread + 1;
        } else {
            argsThread[indiceThread]->indiceInicio = indiceThread * indicesPorThread + indicesRestantes;
            argsThread[indiceThread]->indiceFim = argsThread[indiceThread]->indiceInicio + indicesPorThread;
        }

        pthread_create(&tid_sistema[indiceThread], NULL, incrementaUm, (void*)argsThread[indiceThread]);
    }

    // Espera todas as threads terminarem de executar
    for (int indiceThread = 0; indiceThread < M; indiceThread++) {
        pthread_join(tid_sistema[indiceThread], NULL);
    }

    // Checar se vetor foi processado de maneira correta
    printf("Vetor Incrementado: "); imprimirVetor(vetorParaIncrementar, N);
    checaVetor(vetorInicial, vetorParaIncrementar, N);

    // Libera memória alocada
    free(vetorInicial);
    free(vetorParaIncrementar);

    return 0;
}
