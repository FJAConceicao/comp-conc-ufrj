# Implementação de Leitor-Escritor em Lista Encadeada com Prioridade para Escrita

## Descrição

Esta implementação resolve o clássico problema de sincronização entre leitores e escritores, priorizando as operações de escrita. Sempre que uma escrita é solicitada, as novas leituras são bloqueadas até que a escrita seja concluída. Para facilitar o monitoramento via logs, o número total de operações foi limitado a 700.

## Compilação e Execução

Para compilar o código, utilize o comando:

```bash
gcc list_int.c main_lock_rw_priority.c -o main_lock_rw_priority -Wall
```

Para executar o programa, utilize o seguinte comando:

```bash
./main_lock_rw_priority <número de threads>
```

## Alterações realizadas

Reaproveitei boa parte do código base disponibilizado pela professora, com algumas adaptações feitas para atender aos requisitos específicos do laboratório.

Como o objetivo é priorizar os escritores, criei a variável de controle `escritoresEmEspera`. Ela é incrementada em uma unidade sempre que algum escritor está querendo escrever, um sinal é enviado para liberar algum escritor em espera caso `escritoresEmEspera`maior que 0 ao sair de uma escrita, quando é 0 significa que não há escritores na fila de espera para terem prioridade na execução. Dessa forma, uma thread leitora só pode iniciar sua leitura se não houver nenhum escritor ativo ou aguardando para escrever.

Para facilitar os testes, ajustei a distribuição das operações:

* 50% para consultas (leituras);
* 40% para inserções;
* 10% para remoções.

A configuração original de apenas 1% para inserção e 1% para remoção dificultava a análise.

Modifiquei as quatro funções de leitores e escritores passadas nos slides pela professora:

- **EntraLeitura()**: As threads leitoras iniciam a leitura apenas quando nenhum escritor estiver ativo ou aguardando (ou seja, `escritoresEmEspera > 0`). Caso contrário, elas entram em estado de espera (`wait`) até receberem o sinal de que não há mais escritores atuando ou em espera. Adicionei um print para indicar que a thread leitora está aguardando devido à presença de uma escrita em andamento ou solicitada.
- **SaiLeitura()**: Aqui, adicionei um print para notificar que a thread leitora concluiu sua operação.
- **EntraEscrita()**: As threads escritoras logo que iniciam incrementam escritoresEmEspera em 1 unidade para serem priorizadas na execução e só começam a escrever quando não há leitores ativos ou outros escritores em ação. Caso contrário, elas entram em espera. A variável escritoresEmEspera é decrementada em 1 unidade assim que a thread consegue executar para indicar que ela saiu da fila de prioridade de execução. Adicionei também prints para indicar que a thread tenta escrever (inicio da função), está em espera (dentro do while) e começa escrita (depois do while).
- **SaiEscrita()**: Ao terminar a escrita, além da quantidade de escritores atuando ser decrementada em 1 unidade, verifica-se também se há escritores em espera, caso positivo um sinal é enviado para liberar uma thread escritora garantindo assim a prioridade na execução de escritores e caso contrário as threads leitoras são sinalizadas/liberadas. Um print é exibido para indicar que a thread escritora finalizou sua operação.

## Perguntas do Laboratório

### **Atividade 1**

**1. O TAD lista implementado nesses arquivos poderia ser compartilhado por threads de um programa concorrente? Com qual finalidade e de que forma?**

Sim, o TAD lista pode ser compartilhado entre threads. Isso porque ele oferece uma forma organizada de armazenar dados e realizar operações sobre eles, o que pode ajudar na resolução de problemas. As threads podem usar essa estrutura através de funções que escondem os detalhes da implementação, facilitando o uso da lista de forma segura e eficiente.

**4. O que poderia acontecer se o programa não implementasse exclusão mútua no
acesso às operações da lista encadeada?**

Sem exclusão mútua, podemos ter problemas de concorrência, onde duas ou mais threads tentam acessar ou modificar a lista ao mesmo tempo. Isso pode causar erros nos dados, como leituras e escritas incorretas, criando inconsistências.

**6. O que acontece com o tempo de execução do programa quando aumentamos o
número de threads? Por que isso ocorre?**

O tempo de execução pode aumentar à medida que adicionamos mais threads. Isso acontece porque o controle de exclusão mútua exige tempo extra para gerenciar o acesso das threads. O programa gasta mais tempo alternando entre as threads que tentam acessar a lista do que realizando operações na lista em si.

### **Atividade 2**

**4. Em quais cenários o uso do rwlock pode ser mais vantajoso que o uso do lock de
exclusão mútua?**

O rwlock é melhor em situações onde há muito mais leituras do que escritas. Isso porque ele permite que várias threads leiam os dados ao mesmo tempo, enquanto o lock de exclusão mútua bloqueia todas as threads, mesmo para leitura, o que pode ser menos eficiente.

# Log e Resultados

As imagem abaixo mostra os logs de execução do programa:

![telegram-cloud-photo-size-1-4920370528242479593-y](https://github.com/user-attachments/assets/e1acfab3-e9f1-44a3-8cf6-0f6ae911c2f1)

...

![telegram-cloud-photo-size-1-4920370528242479595-y](https://github.com/user-attachments/assets/2bca293b-d04c-4b34-9688-9b06f286ef10)

Em algumas partes do log é possível perceber a prioridade para escrita, em que leitor ao tentar ler e ainda tem threads escritoras ativas, leitor fica em espera aguardando as threads escritoras terminarem suas operações.
