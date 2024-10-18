#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 3
#define N 20

// Variaveis globais
FILE *arquivoEntrada; //arquivo de entrada
char *buffer1, *buffer2;
int in1=0, out1=0, in2=0, out2=0;
sem_t sem_prod1, sem_cons1, sem_prod2, sem_cons2; //semaforos para coordenar a ordem de execucao das threads

//thread 1: Lê caracteres do arquivo de entrada e coloca no buffer1.
void *thread1(void *arq) {
    FILE* arqEntrada = (FILE*) arq;
    char caractere = fgetc(arqEntrada);  //lê o primeiro caractere do arquivo
    while(caractere != EOF){
        //trecho abaixo até o sem_post é para inserção de caractere lido no buffer1 quando há espaço disponível neste
        sem_wait(&sem_prod1);
        buffer1[in1] = caractere;
        in1 = (in1 + 1) % N;
        sem_post(&sem_cons1);
        caractere = fgetc(arqEntrada);
    }
    
    //parte para inserir EOF no buffer1 para sinalizar que a leitura terminou
    sem_wait(&sem_prod1);
    buffer1[in1] = EOF;
    in1 = (in1 + 1) % N;
    sem_post(&sem_cons1);

    //fclose(arqEntrada);
    pthread_exit(NULL);
}

//thread 2: processa caracteres do buffer1, a qual adiciona quebras de linha, e deposita no buffer2.
void *thread2(void *arg) {
    int n = 0;  //para controlar quantidade de quebras de linha (modificado após n = 10)
    int i = 0;  //contador para inserir a quebra de linha

    while(1){
        //trecho até o sem_post para retirar(consumir) um caractere do buffer1
        sem_wait(&sem_cons1);
        char caractere = buffer1[out1];
        out1 = (out1 + 1) % N;
        sem_post(&sem_prod1);

        //trecho até o sem_post para inserir(produzir) caractere no buffer2
        sem_wait(&sem_prod2);
        buffer2[in2] = caractere;
        in2 = (in2 + 1) % N;
        sem_post(&sem_cons2);

        i += 1;  //incrementa quantidade de caracteres processados

        //verifica se o arquivo terminou e finaliza
        if(caractere == EOF)
            break;

        //insere uma quebra de linha sempre que 2n + 1 (para n <= 10) é atingido ou a cada 10 caracteres após n = 10
        if((n <= 10 && i == 2*n + 1) || (n > 10 && i % 10 == 0)){
            //espera espaço disponível no buffer2 para inserir quebra de linha
            sem_wait(&sem_prod2);
            buffer2[in2] = '\n';
            in2 = (in2 + 1) % N;
            sem_post(&sem_cons2);

            //reiniciamos contagem de caracteres e incrementamos n para ajustar quantidade de quebras de linhas
            i = 0; n++;
        }
    }
    pthread_exit(NULL);
}

//thread 3: para imprimir caracteres do buffer2
void *thread3(void *arg) {
    char caractere;
    while(1){
        //trecho abaixo até sem_post para retirar um caractere do buffer2 quando possível
        sem_wait(&sem_cons2);
        caractere = buffer2[out2];
        out2 = (out2 + 1) % N;
        sem_post(&sem_prod2);

        //verifica se o arquivo terminou e finaliza loop da thread
        if(caractere == EOF)
            break;

        printf("%c", caractere);  //imprime caractere obtido do buffer 2
    }
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
      printf("Erro ao abrir o arquivo %s\n", argv[1]); exit(1);
   }

   // Aloca dinamicamente os buffers
   buffer1 = (char *)malloc(N * sizeof(char));
   buffer2 = (char *)malloc(N * sizeof(char));
   if (buffer1 == NULL || buffer2 == NULL) {
      printf("Erro ao alocar memória para os buffers.\n"); exit(1);
   }

   // Inicializa os semáforos
   sem_init(&sem_prod1, 0, N); sem_init(&sem_cons1, 0, 0);
   sem_init(&sem_prod2, 0, N); sem_init(&sem_cons2, 0, 0);

   //cria as tres threads
   if (pthread_create(&tid[0], NULL, thread1, (void*) arquivoEntrada)) {
      printf("--ERRO: pthread_create() da thread 3\n"); exit(-1);
   }
   if (pthread_create(&tid[1], NULL, thread2, NULL)) {
      printf("--ERRO: pthread_create() da thread 2\n"); exit(-1);
   }
   if (pthread_create(&tid[2], NULL, thread3, NULL)) {
      printf("--ERRO: pthread_create() da thread 1\n"); exit(-1);
   }

   //--espera todas as threads terminarem
   for (int t=0; t<NTHREADS; t++) {
      if (pthread_join(tid[t], NULL)) {
         printf("--ERRO: pthread_join() da thread %d\n", t); exit(-1); 
      } 
   }

   //libera a memória dos buffers e fecha arquivo de entrada
   free(buffer1); free(buffer2);
   fclose(arquivoEntrada);
   sem_destroy(&sem_prod1); sem_destroy(&sem_cons1);
   sem_destroy(&sem_prod2); sem_destroy(&sem_cons2);

   return 0;
}
