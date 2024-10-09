//Programa concorrente que cria e faz operacoes sobre uma lista de inteiros

#include <stdio.h>
#include <stdlib.h>
#include "list_int.h"
#include <pthread.h>
#include "timer.h"

#define QTDE_OPS 700 //quantidade de operacoes sobre a lista (insercao, remocao, consulta)
#define QTDE_INI 100  //quantidade de insercoes iniciais na lista
#define MAX_VALUE 100 //valor maximo a ser inserido

//lista compartilhada iniciada 
struct list_node_s* head_p = NULL; 

//qtde de threads no programa, leitores e escritores e contador de quantidade de escritores esperando para serem atendidos
int nthreads, leit, escr, escritoresEmEspera = 0;

//locks de exclusao mutua e condicionais de leitura e escrita
pthread_mutex_t mutex;
pthread_cond_t cond_leit, cond_escr;

void EntraLeitura(long int id) {
	pthread_mutex_lock(&mutex);
   printf("Thread leitora %ld tenta ler\n", id);
	/* Se tem escritor escrevendo (escr > 0) ou quantidadade de escritores em espera for maior que 0
      (alguma thread está querendo escrever), então thread que chega para ler fica em espera */
	while((escr > 0) || (escritoresEmEspera > 0)) {
      printf("Thread leitora %ld fica esperando, pois tem escritor ativo ou em espera\n", id);
		pthread_cond_wait(&cond_leit, &mutex);
	}
	leit++;
   printf("Thread leitora %ld começa leitura\n", id);
	pthread_mutex_unlock(&mutex);
}

void SaiLeitura(long int id) {
	pthread_mutex_lock(&mutex);
	leit--;
   printf("Thread leitora %ld termina de ler\n", id);
	if(leit==0) pthread_cond_signal(&cond_escr);
	pthread_mutex_unlock(&mutex);
}

void EntraEscrita(long int id) {
	pthread_mutex_lock(&mutex);
   escritoresEmEspera++;
   printf("Thread escritora %ld tenta escrever\n", id);
	while((leit>0) || (escr>0)) {
      printf("Thread escritora %ld fica esperando, pois tem threads escritoras ou leitoras ativas\n", id);
		pthread_cond_wait(&cond_escr, &mutex);
	}
	escr++;
   escritoresEmEspera--;
   printf("Thread escritora %ld começa escrita\n", id);
	pthread_mutex_unlock(&mutex);
}

void SaiEscrita(long int id) {
	pthread_mutex_lock(&mutex);
	escr--;
   printf("Thread escritora %ld termina escrita\n", id);

   /*
      Para priorizar escritor, libera um caso escr > 0
      Libera leitores esperando caso contrário
   */
   if(escritoresEmEspera > 0) { 
      pthread_cond_signal(&cond_escr);
   } else {
      pthread_cond_broadcast(&cond_leit);
   }

	pthread_mutex_unlock(&mutex);
}

//tarefa das threads
void* tarefa(void* arg) {
   long int id = (long int) arg; 
   int op;
   int in, out, read; 
   in=out=read = 0; 

   //realiza operacoes de consulta (98%), insercao (1%) e remocao (1%)
   for(long int i=id; i<QTDE_OPS; i+=nthreads) {
      op = rand() % 100;
      if(op<50) {
        EntraLeitura(id);
        Member(i%MAX_VALUE, head_p);   /* Ignore return value */
        SaiLeitura(id);
	    read++;
      } else if(50<=op && op<90) {
        EntraEscrita(id);
        Insert(i%MAX_VALUE, &head_p);  /* Ignore return value */
        SaiEscrita(id);
        in++;
      } else if(99<=op) {
        EntraEscrita(id);
        Delete(i%MAX_VALUE, &head_p);  /* Ignore return value */
        SaiEscrita(id);
        out++;
      }
   }
   //registra a qtde de operacoes realizadas por tipo
   printf("Thread %ld: in=%d out=%d read=%d\n", id, in, out, read);
   pthread_exit(NULL);
}

/*-----------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   pthread_t *tid;
   double ini, fim, delta;
   
   //verifica se o numero de threads foi passado na linha de comando
   if(argc<2) {
      printf("Digite: %s <numero de threads>\n", argv[0]); return 1;
   }
   nthreads = atoi(argv[1]);

   //insere os primeiros elementos na lista
   for(int i=0; i<QTDE_INI; i++)
      Insert(i%MAX_VALUE, &head_p);  /* Ignore return value */
   
   //aloca espaco de memoria para o vetor de identificadores de threads no sistema
   tid = malloc(sizeof(pthread_t)*nthreads);
   if(tid==NULL) {  
      printf("--ERRO: malloc()\n"); return 2;
   }

   //inicializa a variavel mutex
   pthread_mutex_init(&mutex, NULL);
   
   //tomada de tempo inicial
   GET_TIME(ini);
   //cria as threads
   for(long int i=0; i<nthreads; i++) {
      if(pthread_create(tid+i, NULL, tarefa, (void*) i)) {
         printf("--ERRO: pthread_create()\n"); return 3;
      }
   }
   
   //aguarda as threads terminarem
   for(int i=0; i<nthreads; i++) {
      if(pthread_join(*(tid+i), NULL)) {
         printf("--ERRO: pthread_join()\n"); return 4;
      }
   }

   //tomada de tempo final
   GET_TIME(fim);
   delta = fim-ini;
   printf("Tempo: %lf\n", delta);

   //libera o mutex
   pthread_mutex_destroy(&mutex);
   //libera o espaco de memoria do vetor de threads
   free(tid);
   //libera o espaco de memoria da lista
   Free_list(&head_p);

   return 0;
}  /* main */