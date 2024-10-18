#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 3
#define N 20

//variáveis globais
FILE *arquivoEntrada; //arquivo de entrada
char *buffer1, *buffer2;
int in1=0, out1=0, in2=0, out2=0;
sem_t slotsVazio1, slotsCheio1, slotsVazio2, slotsCheio2; //semáforos para controlar a ordem de execução das threads

//função para inserir caractere em qualquer buffer
void insere_buffer(char *buffer, int *in, char caractere, sem_t *slotVazio, sem_t *slotCheio) {
   //aguarda slot vazio
    sem_wait(slotVazio);
    buffer[*in] = caractere;
    *in = (*in + 1) % N;
    //sinaliza um slot cheio
    sem_post(slotCheio);
}

//função para remover caractere de qualquer buffer
char remove_buffer(char *buffer, int *out, sem_t *slotVazio, sem_t *slotCheio) {
   //aguarda slot cheio
    sem_wait(slotCheio);
    char caractere = buffer[*out];
    *out = (*out + 1) % N;
    //sinaliza um slot vazio
    sem_post(slotVazio);
    return caractere;
}

//thread 1: lê caracteres do arquivo de entrada e coloca no buffer1.
void *thread1(void *arq) {
    FILE* arqEntrada = (FILE*) arq;
    char caractere = fgetc(arqEntrada);  //lê primeiro caractere do arquivo
    while (caractere != EOF) {
        //insere caractere lido no buffer1
        insere_buffer(buffer1, &in1, caractere, &slotsVazio1, &slotsCheio1);
        caractere = fgetc(arqEntrada);
    }

    //parte para inserir EOF no buffer1 para sinalizar que a leitura terminou
    insere_buffer(buffer1, &in1, EOF, &slotsVazio1, &slotsCheio1);

    //fclose(arqEntrada);
    pthread_exit(NULL);
}

//thread 2: processa caracteres do buffer1, a qual adiciona quebras de linha, e deposita no buffer2.
void *thread2(void *arg) {
    int n = 0;  //para controlar quantidade de quebras de linha
    int contCaracteresLidos = 0;  //contador de caracteres lidos

    while (1) {
        //retira(consume) um caractere do buffer1
        char caractere = remove_buffer(buffer1, &out1, &slotsVazio1, &slotsCheio1);

        //insere (produz) caractere no buffer2
        insere_buffer(buffer2, &in2, caractere, &slotsVazio2, &slotsCheio2);

        contCaracteresLidos += 1;  //incrementa quantidade de caracteres processados

        //verifica se o arquivo terminou e finaliza thread
        if (caractere == EOF)
            break;

        //insere uma quebra de linha quando 2n + 1 (para n <= 10) lidos é atingido
        if ((n <= 10) && (contCaracteresLidos == (2*n + 1))) {
            //insere quebra de linha no buffer2
            insere_buffer(buffer2, &in2, '\n', &slotsVazio2, &slotsCheio2);

            //reiniciamos contagem de caracteres e incrementamos n para ajustar quantidade de quebras de linhas
            contCaracteresLidos = 0; n++;
        }

        //insere uma quebra de linha a cada 10 caracteres lidos após n > 10
        if (n > 10 && (contCaracteresLidos == 10)) {
            //espera espaço disponível no buffer2 para inserir quebra de linha
            insere_buffer(buffer2, &in2, '\n', &slotsVazio2, &slotsCheio2);

            //reiniciamos contagem de caracteres
            contCaracteresLidos = 0;
        }

    }
    pthread_exit(NULL);
}

//thread 3: para imprimir caracteres do buffer2
void *thread3(void *arg) {
    char caractere;
    while (1) {
        //pega caractere do buffer2
        caractere = remove_buffer(buffer2, &out2, &slotsVazio2, &slotsCheio2);

        //verifica se o arquivo terminou e finaliza thread
        if (caractere == EOF)
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

   //aloca dinamicamente os buffers
   buffer1 = (char *)malloc(N*sizeof(char));
   buffer2 = (char *)malloc(N*sizeof(char));
   if (buffer1 == NULL || buffer2 == NULL) {
      printf("Erro ao alocar memória para os buffers.\n"); exit(1);
   }

   //inicializa os semáforos
   sem_init(&slotsVazio1, 0, N); sem_init(&slotsCheio1, 0, 0);
   sem_init(&slotsVazio2, 0, N); sem_init(&slotsCheio2, 0, 0);

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
   sem_destroy(&slotsVazio1); sem_destroy(&slotsCheio1);
   sem_destroy(&slotsVazio2); sem_destroy(&slotsCheio2);

   return 0;
}
