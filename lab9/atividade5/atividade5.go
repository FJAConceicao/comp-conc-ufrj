package main

import (
	"fmt"
	"math"
	"sync"
)

// Função que verifica a primalidade de um número
func ehPrimo(numero int) bool {
	if numero <= 1 {
		return false
	}
	if numero == 2 {
		return true
	}
	if numero%2 == 0 {
		return false
	}
	for i := 3; i <= int(math.Sqrt(float64(numero)))+1; i += 2 {
		if numero%i == 0 {
			return false
		}
	}
	return true
}

// Função verificaNumeros, usada pelas goroutines para verificar a primalidade dos números
func verificaNumeros(canalNumeros <-chan int, canalResultados chan<- int, wg *sync.WaitGroup) {
	defer wg.Done()
	for numero := range canalNumeros {
		if ehPrimo(numero) {
			canalResultados <- numero //Envia numero identificado como primo para canalResultados
		}
	}
}

func main() {
	N := 1000 //Quantidade N de números que serão gerados pela sequência
	M := 8   //Quantidade M de goroutines

	/* Contagem Sequencial (Para auxiliar na verificação da corretude) */
	contagemPrimosSequencial := 0
	for i := 1; i <= N; i++ {
		if ehPrimo(i) {
			contagemPrimosSequencial++
		}
	}

	/* Contagem Concorrente */
	canalNumeros := make(chan int, M)
	canalResultados := make(chan int, M)
	var wg sync.WaitGroup

	//Inicia M goroutines usando a função verificaNumeros
	for i := 0; i < M; i++ {
		wg.Add(1)
		go verificaNumeros(canalNumeros, canalResultados, &wg)
	}

	//Gera números de 1 a N e envia para canalNumeros
	go func() {
		for numero := 1; numero <= N; numero++ {
			canalNumeros <- numero
		}
		close(canalNumeros) //Fecha o canal após enviar todos os números
	}()

	//Aguarda todas as goroutines terminarem e fecha canal de resultados
	go func() {
		wg.Wait()
		close(canalResultados)
	}()

	//Conta quantidade de primos encontrados
	contagemPrimosConcorrente := 0
	for range canalResultados {
		contagemPrimosConcorrente++
	}

	//Exibe resultados
	fmt.Printf("Contagem de primos sequencial: %d\n", contagemPrimosSequencial)
	fmt.Printf("Contagem de primos concorrente: %d\n", contagemPrimosConcorrente)

	//Verificando a corretude
	if contagemPrimosSequencial == contagemPrimosConcorrente {
		fmt.Println("As contagens estão corretas!")
	} else {
		fmt.Println("As contagens estão incorretas.")
	}
}
