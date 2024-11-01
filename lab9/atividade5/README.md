# Identificação de Números Primos com Concorrência em Go

Este programa em Go encontra números primos em uma sequência de `1` a `N` utilizando concorrência com goroutines e canais. Ele compara o resultado da contagem concorrente com uma contagem sequencial para verificar a corretude.

## Funcionamento

1. **Função `ehPrimo`**: Verifica se um número é primo, retornando `true` se for primo e `false` caso contrário.
2. **Contagem Sequencial**:
   - Um loop verifica de `1` até `N` se cada número é primo, usando `ehPrimo`.
   - A contagem é armazenada em `contagemPrimosSequencial` para comparação com o resultado concorrente.
3. **Contagem Concorrente**:
   - Utiliza `M` goroutines que verificam a primalidade de números de forma concorrente.
   - `canalNumeros` envia os números a serem verificados.
   - `canalResultados` recebe valores dos números identificados como primos.
   - A contagem total de primos identificados pelas goroutines é armazenada em `contagemPrimosConcorrente`.
4. **Verificação de Corretude**:
   - Compara os resultados da contagem sequencial e concorrente.
   - Exibe uma mensagem confirmando a corretude se os resultados coincidirem.

## Casos de Teste

Casos de testes utilizados para quantidades de N, em que as contagens de primos são esperadas/conhecidas.

| Valor de N | Primos Esperados |
| ---------- | ---------------- |
| 10         | 4                |
| 20         | 8                |
| 50         | 15               |
| 100        | 25               |
| 200        | 46               |
| 1000       | 168              |

### Execução e exemplos de saída:

Altere os valores de `N` e `M` no código e execute com: ``go run atividade5.go``

Teste para `N = 20` e `M = 4`:

```
Contagem de primos sequencial: 8
Contagem de primos concorrente: 8
As contagens estão corretas!
```

Teste para `N = 100` e `M = 8`:

```
Contagem de primos sequencial: 25
Contagem de primos concorrente: 25
As contagens estão corretas!
```

Teste para `N = 200` e `M = 8`:

```
Contagem de primos sequencial: 46
Contagem de primos concorrente: 46
As contagens estão corretas!
```

Teste para `N = 1000` e `M = 8`:

```
Contagem de primos sequencial: 168
Contagem de primos concorrente: 168
As contagens estão corretas!
```
