/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 1 */
/* Codigo: Programa com duas threads que multiplica por 2 cada elemento de um vetor de 10000 elementos. */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 2 //total de threads a serem criadas
#define SIZE 10000;

int v[10000];

//cria a estrutura de dados para armazenar os argumentos da thread
typedef struct {
   int idThread, nThreads;
} t_Args;

// Para visualizar e verificar se está correto
void imprimeVetor() {
  int i = 0;
  printf("Imprimindo vetor e seus elementos:\n");
  for (i = 0; i < 10000; ++i) {
     printf("|%d * 2 = %d|\n", i, v[i]);
  }
}

// funcao executada pelas threads
void *dobraValor (void *arg) {
  t_Args *args = (t_Args *) arg;

  int i = 0;
  if (args->idThread == 0) {
      for (i = 0; i < 5000; ++i) {
          v[i] = v[i]*2;
      }
  } else {
      for (i = 5000; i < 10000; ++i) {
          v[i] = v[i]*2;
      }
  }

  pthread_exit(NULL);
}

// funcao principal do programa
int main() {
  pthread_t tid_sistema[NTHREADS]; //identificadores das threads no sistema
  int thread; //variavel auxiliar
  t_Args *arg; //receberá os argumentos para a thread

  int i = 0;
  for (i = 0; i < 10000; ++i) {
    v[i] = i;
  }

  for (thread = 0; thread < NTHREADS; ++thread) {
    printf("--Aloca e preenche argumentos para thread %d\n", thread);
    arg = malloc(sizeof(t_Args));
    if (arg == NULL) {
      printf("--ERRO: malloc()\n"); exit(-1);
    }
    arg->idThread = thread;
    arg->nThreads = NTHREADS;

    printf("--Cria a thread %d\n", thread);
    if (pthread_create(&tid_sistema[thread], NULL, dobraValor, (void*) arg)) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
    }
  }

  //--espera todas as threads terminarem
  for (thread=0; thread < NTHREADS; thread++) {
    if (pthread_join(tid_sistema[thread], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1);
    }
  }

  imprimeVetor();

  printf("\n--Thread principal terminou\n");
  pthread_exit(NULL);
}
