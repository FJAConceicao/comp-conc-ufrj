#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

// tamanho do buffer
#define TAMANHO_BUFFER 16

// número de threads
#define NUM_PRODUTORES 4
#define NUM_CONSUMIDORES 4

int buffer[TAMANHO_BUFFER];
int contador = 0;

sem_t cheio, vazio;
sem_t mutexProdutor, mutexConsumidor;

// inicializa o buffer
void inicializarBuffer(int n) {
  int i;
  for (i = 0; i < n; i++)
    buffer[i] = 0;
}

// imprime o buffer
void imprimirBuffer(int n) {
  int i;
  for (i = 0; i < n; i++)
    printf("%d ", buffer[i]);
  printf("\n");
}

void inserir(int item, int id) {
  static int in = 0;

  sem_wait(&vazio);
  sem_wait(&mutexProdutor);
  printf("Produtor [%d] bloqueou o semáforo 'vazio' e 'mutexProdutor'.\n", id);

  buffer[in] = item;
  in = (in + 1) % TAMANHO_BUFFER;
  contador++;
  printf("Produtor [%d] inseriu o item %d no buffer e atualizou 'in'.\n", id, item);

  imprimirBuffer(TAMANHO_BUFFER);
  printf("Produtor [%d] imprimiu o buffer.\n\n", id);

  sem_post(&mutexProdutor);
  if (contador == TAMANHO_BUFFER) {
    int i;
    printf("Produtor [%d] liberou o semáforo 'cheio' para os consumidores.\n\n", id);
    for (i = 0; i < TAMANHO_BUFFER; i++) {
      sem_post(&cheio);
    }
  }
}

int retirar(int id) {
  static int out = 0;
  int item;

  sem_wait(&cheio);
  sem_wait(&mutexConsumidor);
  printf("Consumidor [%d] bloqueou o semáforo 'cheio' e 'mutexConsumidor'.\n", id);

  item = buffer[out];
  buffer[out] = 0;
  out = (out + 1) % TAMANHO_BUFFER;
  contador--;
  printf("Consumidor [%d] removeu o item %d do buffer e atualizou 'out'.\n", id, item);

  imprimirBuffer(TAMANHO_BUFFER);
  printf("Consumidor [%d] imprimiu o buffer.\n\n", id);

  sem_post(&mutexConsumidor);
  if (contador == 0) {
    int i;
    printf("Consumidor [%d] liberou o semáforo 'vazio' para os produtores.\n\n", id);
    for (i = 0; i < TAMANHO_BUFFER; i++) {
      sem_post(&vazio);
    }
  }
  return item;
}

void *produtor(void *arg) {
  int *id = (int *)arg;
  // printf("Sou a thread produtora %d\n", *id);
  while (1) {
    // produzindo o item
    inserir(*id, *id);
  }
  pthread_exit(NULL);
}

void *consumidor(void *arg) {
  int *id = (int *)arg;
  int item;
  // printf("Sou a thread consumidora %d\n", *id);
  while (1) {
    item = retirar(*id);
  }
  pthread_exit(NULL);
}

int main(void) {
  int i;
  pthread_t threads[NUM_PRODUTORES + NUM_CONSUMIDORES];
  int ids[NUM_PRODUTORES + NUM_CONSUMIDORES];
  
  printf("Inicializando o buffer\n\n");
 inicializarBuffer(TAMANHO_BUFFER);

  printf("Iniciando os semáforos\n\n");
  sem_init(&mutexConsumidor, 0, 1);
  sem_init(&mutexProdutor, 0, 1);
  sem_init(&cheio, 0, 0);
  sem_init(&vazio, 0, TAMANHO_BUFFER);

  for (i = 0; i < NUM_PRODUTORES; i++) {
    ids[i] = i + 1;
    printf("Criando a thread produtora [%d]\n\n", i + 1);
    pthread_create(&threads[i], NULL, produtor, (void *)&ids[i]);
  }

  for (i = 0; i < NUM_CONSUMIDORES; i++) {
    ids[i + NUM_PRODUTORES] = i + 1;
    printf("Criando a thread consumidora [%d]\n\n", i + 1);
    pthread_create(&threads[i + NUM_PRODUTORES], NULL, consumidor, (void *)&ids[i + NUM_PRODUTORES]);
  }

  pthread_exit(NULL);
  return 0;
}
