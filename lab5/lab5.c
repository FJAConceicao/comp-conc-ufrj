#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 5

/* Variaveis globais */
int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

/* Thread 1 */
void *thread_um (void *t) {
    printf("Oi Maria!\n");
    pthread_exit(NULL);
}

/* Thread 2 */
void *thread_dois (void *t) {
    printf("Oi José!\n");
    pthread_exit(NULL);
}

/* Thread 3 */
void *thread_tres (void *t) {
    printf("Sente-se por favor.\n");
    pthread_exit(NULL);
}

/* Thread 4 */
void *thread_quatro (void *t) {
    printf("Até mais José!\n");
    pthread_exit(NULL);
}

/* Thread 5 */
void *thread_cinco (void *t) {
    printf("Até mais Maria!\n", id);
    pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
  
  int i;
  pthread_t threads[NTHREADS];

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond, NULL);

  /* Cria as threads */
  pthread_create(&threads[0], NULL, thread_um, NULL);
  pthread_create(&threads[1], NULL, thread_dois, NULL);
  pthread_create(&threads[2], NULL, thread_tres, NULL);
  pthread_create(&threads[3], NULL, thread_quatro, NULL);
  pthread_create(&threads[4], NULL, thread_cinco, NULL);

  /* Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
}
