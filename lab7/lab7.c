#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 3
#define N 100

// Variaveis globais
sem_t estado1, estado2;      //semaforos para coordenar a ordem de execucao das threads
FILE *arquivoEntrada;        //arquivo de entrada
char *buffer1, buffer2;

//funcao executada pela thread 1
void *t1 (void *arg) {
   pthread_exit(NULL);
}

//funcao executada pela thread 2
void *t2 (void *arg) {
   pthread_exit(NULL);
}

//funcao executada pela thread 3
void *t3 (void *arg) {
   pthread_exit(NULL);
}

//funcao principal
int main(int argc, char *argv[]) {
   pthread_t tid[NTHREADS];

   //verifica se o arquivo de entrada foi fornecido
   if (argc < 2) {
      printf("Uso: %s <arquivo de entrada>\n", argv[0]);
      exit(1);
   }

   //abre arquivo de entrada
   arquivoEntrada = fopen(argv[1], "r");
   if (arquivoEntrada == NULL) {
      printf("Erro ao abrir o arquivo %s\n", argv[1]);
      exit(1);
   }

   // Aloca dinamicamente os buffers
   buffer1 = (char *)malloc(N * sizeof(char));
   buffer2 = (char *)malloc(N * sizeof(char));

   if (buffer1 == NULL || buffer2 == NULL) {
      printf("Erro ao alocar memória para os buffers.\n"); exit(1);
   }

   //inicia os semaforos
   //sem_init(&estado1, 0, ?);
   //sem_init(&estado2, 0, ?); 

   //cria as tres threads
   if (pthread_create(&tid[2], NULL, t3, NULL)) {
      printf("--ERRO: pthread_create() da thread 1\n");
      exit(-1);
   }
   if (pthread_create(&tid[1], NULL, t2, NULL)) {
      printf("--ERRO: pthread_create() da thread 2\n");
      exit(-1);
   }
   if (pthread_create(&tid[0], NULL, t1, NULL)) {
      printf("--ERRO: pthread_create() da thread 3\n");
      exit(-1);
   }

   //--espera todas as threads terminarem
   for (int t=0; t<NTHREADS; t++) {
      if (pthread_join(tid[t], NULL)) {
         printf("--ERRO: pthread_join() da thread %d\n", t);
         exit(-1); 
      } 
   }

   //libera a memória dos buffers e fecha arquivo de entrada
   free(buffer1); free(buffer2);
   fclose(arquivoEntrada);

   return 0;
}
