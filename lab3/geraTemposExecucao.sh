#!/bin/bash

quantidadeExecucoes=5

#dados para a geração dos resultados
dimensoesMatrizes=("500" "1000" "2000")
quantidadeThreads=("1" "2" "4" "8")

#gera as matrizes de entrada
for dimensaoMatriz in "${dimensoesMatrizes[@]}"; do
    ./geraMatrizBinario $dimensaoMatriz $dimensaoMatriz "matrix_A_${dimensaoMatriz}.bin"
    ./geraMatrizBinario $dimensaoMatriz $dimensaoMatriz "matrix_B_${dimensaoMatriz}.bin"
done

#roda multiplicação sequencial e concorrente (com diferentes números de threads)
for dimensaoMatriz in "${dimensoesMatrizes[@]}"; do
    for i in $(seq 1 $quantidadeExecucoes); do
        ./multiplicaMatrizesSequencial "matrix_A_${dimensaoMatriz}.bin" "matrix_B_${dimensaoMatriz}.bin" "matriz_saida_sequencial_${dimensaoMatriz}.bin"
    done
done

#roda multiplicação concorrente (com diferentes números de threads)
for thread in "${quantidadeThreads[@]}"; do
    for dimensaoMatriz in "${dimensoesMatrizes[@]}"; do
        echo "Matrix Size: ${dimensaoMatriz}x${dimensaoMatriz} - ${thread} Threads"
        for i in $(seq 1 $quantidadeExecucoes); do
            ./multiplicaMatrizesConcorrente "matrix_A_${dimensaoMatriz}.bin" "matrix_B_${dimensaoMatriz}.bin" "matriz_saida_concorrente_${dimensaoMatriz}.bin" $thread
        done
    done
done