#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define SIZE 10  // Tamanho do vetor

// Função de ordenação por seleção
void selecao(int *v, int inicio, int fim) {
    int i, j, menor, temp;
    for (i = inicio; i < fim - 1; i++) {
        menor = i;
        for (j = i + 1; j < fim; j++) {
            if (v[j] < v[menor]) {
                menor = j;
            }
        }
        if (i != menor) {
            temp = v[i];
            v[i] = v[menor];
            v[menor] = temp;
        }
    }
}

// Estrutura de dados para passar argumentos para as threads
typedef struct {
    int *v;       // Vetor a ser ordenado
    int inicio;   // Índice inicial do subvetor
    int fim;      // Índice final do subvetor
} ThreadData;

// Função que cada thread irá executar
void *thread_sort(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    selecao(data->v, data->inicio, data->fim);
    pthread_exit(NULL);
}

int main() {
    int v[SIZE] = {100, 40, 60, 20, 30, 15, 0, 110, 105, 50};
    int meio = SIZE / 2;

    pthread_t threads[2];
    ThreadData thread_data[2];

    // Inicializa os dados da primeira metade
    thread_data[0].v = v;
    thread_data[0].inicio = 0;
    thread_data[0].fim = meio;

    // Inicializa os dados da segunda metade
    thread_data[1].v = v;
    thread_data[1].inicio = meio;
    thread_data[1].fim = SIZE;

    // Cria as threads
    pthread_create(&threads[0], NULL, thread_sort, (void *)&thread_data[0]);
    pthread_create(&threads[1], NULL, thread_sort, (void *)&thread_data[1]);

    // Aguarda o término das threads
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    // Exibe o vetor parcialmente ordenado
    printf("Vetor parcialmente ordenado: ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");

    // Aqui, você poderia fazer uma ordenação final no vetor completo, se necessário

    return 0;
}