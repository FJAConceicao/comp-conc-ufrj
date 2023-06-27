from threading import Thread
import math, time, sys

class ThreadCalculatePi(Thread):
    def __init__(self, pos_terms_start, number_thread_terms):
        super().__init__()
        self.pos_terms_start = pos_terms_start
        self.number_thread_terms = number_thread_terms
        self.total_sum_terms_threads = 0.0

    # Efetua o calculo de n séries de pi {self.number_thread_terms} a partir de um determinado ponto {self.pos_terms_start}
    def run(self):
        for n in range(self.pos_terms_start, self.pos_terms_start + self.number_thread_terms):
            self.total_sum_terms_threads += math.pow(-1, n) / (2*n + 1)

def calculate_pi_concurrently(quant_series, quant_threads):
    # Divide igualmente a quantidade de séries para cada thread
    series_por_thread = quant_series // quant_threads

    # Cria as threads passando o ponto de inicio relativo e quantos termos cada deve calcular
    list_of_threads = []
    for i in range(quant_threads):
        list_of_threads.append(ThreadCalculatePi(i * series_por_thread, series_por_thread))
    
    # Inicia as threads
    for thread in list_of_threads:
        thread.start()
    
    # Espera a finalização e guarda o resultado de cada thread
    list_sums_threads = []
    for thread in list_of_threads:
        thread.join()
        list_sums_threads.append(thread.total_sum_terms_threads)
    
    # Retorna o somatório do resultado de todas as threads multiplicado por quatro para encontar o valor aproximado de pi
    return sum(list_sums_threads) * 4

# Verifica os argumentos de entrada do programa
if sys.argv.__len__() != 3:
    print("Argumentos incorretos! Entre com lab7.py <número de séries> <quantidade de threads>")
    exit()

quant_series = int(sys.argv[1])
quant_threads = int(sys.argv[2])

# Efetua o cálculo do valor aproximado de pi, registra o tempo e mostra os resultados na tela
start_time = time.time()
pi_value = 3.14
pi_value_conc = calculate_pi_concurrently(quant_series, quant_threads)
total_time = time.time() - start_time

print("Valor pi do módulo math:", math.pi)
print("Aproximação concorrente:", pi_value_conc)
print("Total de séries........:", quant_series, "| Threads:", quant_threads)
print("Cálculo executado em", total_time, "segundos.")
