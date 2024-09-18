/* Disciplina: Programacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Codigo: Comunicação entre threads usando variável compartilhada e exclusao mutua com bloqueio */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define QUANT_MULTIPLOS_IMPRIMIR 20

long int soma = 0; //variavel compartilhada entre as threads
int quantMultiplosImpressos = 0;
pthread_mutex_t mutex; //variavel de lock para exclusao mutua
pthread_cond_t condSoma, condExtra;

//funcao executada pelas threads
void *ExecutaTarefa (void *arg)
{
  long int id = (long int) arg;
  printf("Thread : %ld esta executando...\n", id);

  for (int i=0; i<100000; i++)
  {
    //--entrada na SC
    pthread_mutex_lock(&mutex);
    //--SC (seção critica)

    /* Verifica se soma é múltiplo de 10 e se não atingiu os 20 primeiros múltiplos
        Realiza wait para esperar thread extra imprimir
          Neste caso, a thread extra estará bloqueada enquanto quantMultiplosImpressos < 20 e recebe um sinal de ExecutaTarefa para se desbloquear e imprimir
    */
    if(soma%10 == 0)
    {
      if(quantMultiplosImpressos < QUANT_MULTIPLOS_IMPRIMIR)
      {
        //Manda um sinal para a thread extra imprimir o múltiplo encontrado e pausa ExecutaTarefa até que o número seja impresso
        pthread_cond_signal(&condExtra); pthread_cond_wait(&condSoma, &mutex);
        quantMultiplosImpressos++; //Incrementa quantidade de múltiplos impressos
      }
    }

    soma++; //incrementa a variavel compartilhada 

    //--saida da SC
    pthread_mutex_unlock(&mutex);
  }

  printf("Thread : %ld terminou!\n", id);
  pthread_cond_signal(&condExtra); //Para desbloquear thread extra caso ainda esteja bloqueada e garantir que ela termine
  pthread_exit(NULL);
}

//funcao executada pela thread de log
void *extra (void *args) {
  printf("Extra : esta executando...\n");

  while(quantMultiplosImpressos < QUANT_MULTIPLOS_IMPRIMIR)
  {
    //--entrada na SC
    pthread_mutex_lock(&mutex);
    //--SC (seção critica)
    if(soma%10 == 0)
    {
      printf("soma = %ld \n", soma); //imprime 'soma', pois é multipla de 10

      //Desbloqueia threads que executam ExecutaTarefa e pausa thread extra
      pthread_cond_signal(&condSoma); pthread_cond_wait(&condExtra, &mutex);
    }
    //--saida da SC
    pthread_mutex_unlock(&mutex);
  }

  printf("Extra : terminou!\n");
  pthread_cond_broadcast(&condSoma); //Para desbloquear threads ExecutaTarefa caso ainda estejam bloqueadas e garantir terminem
  pthread_exit(NULL);
}

//fluxo principal
int main(int argc, char *argv[]) {
   pthread_t *tid; //identificadores das threads no sistema
   int nthreads; //qtde de threads (passada linha de comando)

   //--le e avalia os parametros de entrada
   if(argc<2) {
      printf("Digite: %s <numero de threads>\n", argv[0]);
      return 1;
   }
   nthreads = atoi(argv[1]);

   //--aloca as estruturas
   tid = (pthread_t*) malloc(sizeof(pthread_t)*(nthreads+1));
   if(tid==NULL) {puts("ERRO--malloc"); return 2;}

   //--inicializa o mutex (lock de exclusao mutua) e a variaveis de condicao
   pthread_mutex_init(&mutex, NULL); pthread_cond_init(&condSoma, NULL); pthread_cond_init(&condExtra, NULL);

   //--cria as threads
   for(long int t=0; t<nthreads; t++) {
     if (pthread_create(&tid[t], NULL, ExecutaTarefa, (void *)t)) {
       printf("--ERRO: pthread_create()\n"); exit(-1);
     }
   }

   //--cria thread de log
   if (pthread_create(&tid[nthreads], NULL, extra, NULL)) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
   }

   //--espera todas as threads terminarem
   for (int t=0; t<nthreads+1; t++) {
     if (pthread_join(tid[t], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
     } 
   } 

   //--finaliza o mutex e as variáveis de condição
   pthread_mutex_destroy(&mutex); pthread_cond_destroy(&condSoma); pthread_cond_destroy(&condExtra);
   
   printf("Valor de 'soma' = %ld\n", soma);

   return 0;
}
