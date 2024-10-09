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

Como o objetivo é priorizar os escritores, criei a variável de controle `querEscrever`. Ela assume o valor 1 quando algum escritor está escrevendo ou solicitando escrita, e 0 quando todos os escritores foram atendidos. Dessa forma, uma thread leitora só pode iniciar sua leitura se não houver nenhum escritor ativo ou aguardando para escrever.

Para facilitar os testes, ajustei a distribuição das operações:

* 50% para consultas (leituras);
* 40% para inserções;
* 10% para remoções.

A configuração original de apenas 1% para inserção e 1% para remoção dificultava a análise.


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

![telegram-cloud-photo-size-1-4916103843306122556-y](https://github.com/user-attachments/assets/fb69671e-97c2-4fd1-98a1-cad617ac5ccd)

...

![telegram-cloud-photo-size-1-4916103843306122557-y](https://github.com/user-attachments/assets/9e4cbbc1-39d0-46df-b0a9-dc7ae0c98b3e)

Em algumas partes do log é possível perceber a prioridade para escrita, em que leitor ao tentar ler e ainda tem threads escritoras ativas, leitor fica em espera aguardando as threads escritoras terminarem suas operações.
