#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../timer.h"

float *vetor;
long long int numElemVetor; //numero de elementos do vetor
float limiteInferior; //limite inferior da faixa
float limiteSuperior; //limite superior da faixa
int contSeq; //numero de valores x que estao no intervalo calculado de forma sequencial
int contConc; //numero de valores x que estao no intervalo calculado de forma concorrente
int nThreads; //numero de threads do programa

// funcao em que as threads serão executadas
void *tarefa(void *arg) {
    int *contElemIntervalo; //variavel local da contagem de elementos no intevalo
    contElemIntervalo = (int*) malloc(sizeof(int));
    
    if(contElemIntervalo == NULL) {
        exit(1);
    }

    long int argx = * (int*) arg;
    float elemento;
    for(long int i = argx ; i < numElemVetor; i += nThreads) {
        elemento = vetor[i];
        if (elemento > limiteInferior && elemento < limiteSuperior) {
            *contElemIntervalo= (*contElemIntervalo) + 1;
        }
    }
   //retorna o resultado da contagem local
   pthread_exit((void *) contElemIntervalo); 
}

void verificaElementoSeq(void){
    int i;
    float elemento;
    for(i = 0; i < numElemVetor; i++) {
        elemento = vetor[i];
        if (elemento > limiteInferior && elemento < limiteSuperior) {
            contSeq++;
        }
    }
}

//fluxo principal
int main(int argc, char* argv[]) {
    pthread_t *tid; //identificadores das threads no sistema
    int *id; //identificadores locais das threads e dimensao
    int *retorno;

    double inicio, fim, delta;
   
    if(argc < 3) {
        printf("Digite: %s <numero de elementos do vetor> <numero de threads>\n", argv[0]);
        return 1;
    }

    numElemVetor = atoi(argv[1]);
    nThreads = atoi(argv[2]);
    
    if (nThreads > numElemVetor) 
        nThreads = numElemVetor;

    //alocacao de memoria para as estruturas de dados
    vetor = (float *) malloc(sizeof(float)*numElemVetor);
    
    if (vetor == NULL) {
        printf("ERRO--malloc\n");
        return 2;
    }
    
    //inicializacao das estruturas de dados de entrada e saida

    for(int i = 0; i < numElemVetor; i++) {
        vetor[i] = (rand() % 200 ) / 2.3;
    }

    printf("Insira o limite inferior da faixa: ");
    scanf("%f", &limiteInferior);

    printf("Insira o limite superior da faixa: ");
    scanf("%f", &limiteSuperior);

    //de forma sequencial, encontra quantidade de valores
    GET_TIME(inicio);

    verificaElementoSeq();
   
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo Sequencial: %lf\n", delta);

    //de forma concorrente, encontra quantidade de valores

    //alocacao das estruturas
    GET_TIME(inicio);

    tid = (pthread_t*) malloc(sizeof(pthread_t)*nThreads);
    if (tid == NULL) {
        puts("ERRO--malloc");
        return 2;
    }
    
    id = (int*) malloc(sizeof(int)*nThreads);
    if(id == NULL) {
        puts("ERRO--malloc");
        return 2;
    }
   
    //criacao das threads
    for(int i = 0; i < nThreads; i++) {
        id[i] = i;
        
        if(pthread_create(tid + i, NULL, tarefa, (void*) (id + i))) {
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
    }
   
    //espera pelo termino da threads
    for(int i = 0; i < nThreads; i++) {
        if(pthread_join(*(tid + i), (void**) &retorno)) {
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }

    //contador global de concorrencia
    contConc += *retorno;
    }

    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo Concorrente: %lf\n", delta);

    //corretude
    if(contConc != contSeq)
        printf("Erro!\n");

    //liberacao da memoria
    free(vetor);
    free(id);
    free(tid);
    
    return 0;
}
