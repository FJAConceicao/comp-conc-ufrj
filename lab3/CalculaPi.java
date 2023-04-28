/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 3 */
/* Codigo: Estendendo a classe Thread de Java */


//--PASSO 1: cria uma classe que estende a classe Thread 
class CalcSumTermsPhi extends Thread {
   private int idThread;
   private int numberOfTerms;
   private int numberOfThreads;
   private double totalSumTermsThread;

   //--construtor
   public CalcSumTermsPhi(int idThread, int numberOfTerms, int numberOfThreads) { 
      this.idThread = idThread;
      this.numberOfTerms = numberOfTerms;
      this.numberOfThreads = numberOfThreads;
      this.totalSumTermsThread = 0;
   }

   public double getTotalSumTermsThread() {
      return totalSumTermsThread;
   }

   //--metodo executado pela thread
   public void run() {
      for(int idThread=this.idThread; idThread < numberOfTerms; idThread += numberOfThreads) {
         totalSumTermsThread += (Math.pow(-1, idThread) * 1/(2*idThread + 1));
      } 
   }
}

//--classe do metodo main
class CalculaPi {

   static int numberTerms;
   static int numberThreads;

   public static void main (String[] args) {
      
      // variável acumuladora para o cálculo do pi  
      double pi = 0;

      //Receber e validar os dados
       if(args.length != 2) {
           System.out.println("Digite: java CalculaPi <Quantidade de Pi termos> <numero de threads>");
           System.exit(1);
       }
       numberTerms = Integer.parseInt(args[0]);
       numberThreads = Integer.parseInt(args[1]);

      //Reservar espaço para vetor de threads
      Thread[] threads = new Thread[numberThreads];

      // cria threads da classe CalcSumTermsPhi que estende Thread
      for (int idThread = 0; idThread < threads.length; idThread++) {
         threads[idThread] = new CalcSumTermsPhi(idThread, numberTerms, numberThreads);
      }

      // inicia as threads
      for (int idThread = 0; idThread < threads.length; idThread++) {
         threads[idThread].start();
      }

      // esperar pelo término de todas as threads
      for (int idThread = 0; idThread < threads.length; idThread++) {
         try {
            threads[idThread].join();
         } catch (InterruptedException e) { 
            return; 
         }
      }

      // Somar valores obtidos no cálculo de cada thread para obter o valor final de pi
      for (int idThread = 0; idThread < threads.length; idThread++) {
         pi += (4 * ((CalcSumTermsPhi) threads[idThread]).getTotalSumTermsThread());
      }

       //--Imprime resultado encontrado de pi
       System.out.println("Valor de pi com os dados informados: " + pi);
       System.out.println("Constante PI do módulo Math: " + Math.PI);
       System.out.println("Erro relativo: " + ((Math.PI - pi))/Math.PI);
   }
}
