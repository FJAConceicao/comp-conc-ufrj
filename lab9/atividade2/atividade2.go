package main

import "fmt"

func tarefa(canalMensagens chan string, canalSinalFinalizacao chan bool) {

	//Através do canal, lê e imprime 1ª mensagem da main, depois envia a 2ª mensagem para ser lida e impressa pela main
	fmt.Println("Main envia para Goroutine:", <-canalMensagens)
	canalMensagens <- "'Oi Main, bom dia, tudo bem?' (impresso pela Main)"

	//Através do canal, lê e imprime 2ª mensagem da main, depois envia a 4ª mensagem para ser lida e impressa pela main
	fmt.Println("Main envia para Goroutine:", <-canalMensagens)
	canalMensagens <- "'Certo, entendido.' (impresso pela Main)"

	//Aguarda sinal de finalização da main, imprime mensagem que finalizou e sinaliza isso para main
	<-canalSinalFinalizacao
	fmt.Println("Goroutine imprime: finalizando")
	canalSinalFinalizacao <- true
}

func main() {

	//Canais de mensagens e de sinalização de término da goroutine
	canalMensagens := make(chan string)
	canalSinalFinalizacao := make(chan bool)

	//Executa da goroutine
	go tarefa(canalMensagens, canalSinalFinalizacao)

	//Manda a 1ª mensagem para ser lida pela goroutine através do canal. Em seguida, recebe e imprime a 2ª mensagem enviada pela goroutine.
	canalMensagens <- "'Olá, Goroutine, bom dia!' (impresso pela Goroutine)"
	fmt.Println("Goroutine envia para Main:", <-canalMensagens)

	//Envia a 3ª mensagem para a goroutine através do canal e recebe a 4ª mensagem de resposta para imprimir
	canalMensagens <- "'Tudo bem! Vou terminar tá?' (impresso pela Goroutine)"
	fmt.Println("Goroutine envia para Main:", <-canalMensagens)

	//Envia sinal para goroutine finalizar, aguarda sinal de finalização da goroutine e finaliza em seguida também
	canalSinalFinalizacao <- true
	<-canalSinalFinalizacao
	fmt.Println("Main imprime: finalizando")
}
