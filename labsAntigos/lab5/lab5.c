#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 5

/* Variaveis globais de controle e sincronização */
int printed_message_counter = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond_t1_t2;
pthread_cond_t x_cond_t4_t5;

/* Thread 1 */
void *thread_um (void *t)
{
    printf("Oi Maria!\n");

    pthread_mutex_lock(&x_mutex);
    printed_message_counter++;

    if(printed_message_counter == 2)
    {
        pthread_cond_signal(&x_cond_t1_t2);
    }
    pthread_mutex_unlock(&x_mutex);

    pthread_exit(NULL);
}

/* Thread 2 */
void *thread_dois (void *t)
{
    printf("Oi José!\n");

    pthread_mutex_lock(&x_mutex);
    printed_message_counter++;

    if(printed_message_counter == 2)
    {
        pthread_cond_signal(&x_cond_t1_t2);
    }
    pthread_mutex_unlock(&x_mutex);

    pthread_exit(NULL);
}

/* Thread 3 */
void *thread_tres (void *t) {

    pthread_mutex_lock(&x_mutex);

    if(printed_message_counter < 2) {
        pthread_cond_wait(&x_cond_t1_t2, &x_mutex);
    }

    printf("Sente-se por favor.\n");
    pthread_cond_signal(&x_cond_t4_t5);

    pthread_mutex_unlock(&x_mutex);

    pthread_exit(NULL);
}

/* Thread 4 */
void *thread_quatro (void *t) {
    pthread_mutex_lock(&x_mutex);

    if(printed_message_counter < 3){
        pthread_cond_wait(&x_cond_t4_t5, &x_mutex);
    }
  
    printf("Até mais José!\n");
    pthread_mutex_unlock(&x_mutex);

    pthread_exit(NULL);
}

/* Thread 5 */
void *thread_cinco (void *t) {

    pthread_mutex_lock(&x_mutex);

    if(printed_message_counter < 3){
        pthread_cond_wait(&x_cond_t4_t5, &x_mutex);
    }
  
    printf("Até mais Maria!\n");
    pthread_mutex_unlock(&x_mutex);
    
    pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
  
  int i;
  pthread_t threads[NTHREADS];

  /* Inicializa o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond_t1_t2, NULL);
  pthread_cond_init (&x_cond_t4_t5, NULL);

  /* Cria as threads */
  pthread_create(&threads[0], NULL, thread_um, NULL);
  pthread_create(&threads[1], NULL, thread_dois, NULL);
  pthread_create(&threads[2], NULL, thread_tres, NULL);
  pthread_create(&threads[3], NULL, thread_quatro, NULL);
  pthread_create(&threads[4], NULL, thread_cinco, NULL);

  /* Espera todas as threads terminarem */
  for (i = 0; i < NTHREADS; i++)
  {
    pthread_join(threads[i], NULL);
  }

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond_t1_t2);
  pthread_cond_destroy(&x_cond_t4_t5);
}
