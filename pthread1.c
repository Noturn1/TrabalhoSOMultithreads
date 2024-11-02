#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define SIZE 100  // Tamanho do vetor

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
} ThreadArgs;

// Mutex para sincronização
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Função para threads sem sincronização
void* thread_func(void* args) {
    ThreadArgs* targs = (ThreadArgs*) args;
    selecao(targs->v, targs->inicio, targs->fim);
    return NULL;
}

// Função para threads com sincronização
void* thread_func_sync(void* args) {
    ThreadArgs* targs = (ThreadArgs*) args;

    pthread_mutex_lock(&mutex);
    selecao(targs->v, targs->inicio, targs->fim);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

// Função para ordenação sequencial
void ordenacao_sequencial(int *v, int *v_ordenado) {
    for (int i = 0; i < SIZE; i++) {
        v_ordenado[i] = v[i];
    }
    selecao(v_ordenado, 0, SIZE);
}

// Função para ordenação multithread sem sincronização
void ordenacao_multithread(int *v, int *v_ordenado) {
    pthread_t threads[4];
    ThreadArgs args[4];
    int part_size = SIZE / 4;

    for (int i = 0; i < SIZE; i++) {
        v_ordenado[i] = v[i];
    }

    for (int i = 0; i < 4; i++) {
        args[i].v = v_ordenado;
        args[i].inicio = i * part_size;
        args[i].fim = (i == 3) ? SIZE : (i + 1) * part_size;
        pthread_create(&threads[i], NULL, thread_func, &args[i]);
    }

    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    // Combinar as partes ordenadas
    int temp[SIZE];
    int indices[4] = {0, part_size, 2 * part_size, 3 * part_size};
    int k = 0;

    while (k < SIZE) {
        int min_index = -1;
        for (int i = 0; i < 4; i++) {
            if (indices[i] < ((i == 3) ? SIZE : (i + 1) * part_size) &&
                (min_index == -1 || v_ordenado[indices[i]] < v_ordenado[indices[min_index]])) {
                min_index = i;
            }
        }
        temp[k++] = v_ordenado[indices[min_index]++];
    }

    for (int i = 0; i < SIZE; i++) {
        v_ordenado[i] = temp[i];
    }
}

// Função para ordenação multithread com sincronização
void ordenacao_multithread_sync(int *v, int *v_ordenado) {
    pthread_t threads[4];
    ThreadArgs args[4];
    int part_size = SIZE / 4;

    for (int i = 0; i < SIZE; i++) {
        v_ordenado[i] = v[i];
    }

    for (int i = 0; i < 4; i++) {
        args[i].v = v_ordenado;
        args[i].inicio = i * part_size;
        args[i].fim = (i == 3) ? SIZE : (i + 1) * part_size;
        pthread_create(&threads[i], NULL, thread_func_sync, &args[i]);
    }

    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    // Combinar as partes ordenadas
    int temp[SIZE];
    int indices[4] = {0, part_size, 2 * part_size, 3 * part_size};
    int k = 0;

    while (k < SIZE) {
        int min_index = -1;
        for (int i = 0; i < 4; i++) {
            if (indices[i] < ((i == 3) ? SIZE : (i + 1) * part_size) &&
                (min_index == -1 || v_ordenado[indices[i]] < v_ordenado[indices[min_index]])) {
                min_index = i;
            }
        }
        temp[k++] = v_ordenado[indices[min_index]++];
    }

    for (int i = 0; i < SIZE; i++) {
        v_ordenado[i] = temp[i];
    }
}

int main() {
    int v[SIZE];
    int v_ordenado[SIZE];

    // Inicializar o vetor com valores aleatórios
    for (int i = 0; i < SIZE; i++) {
        v[i] = rand() % 1000;
    }

    // Ordenação sequencial
    ordenacao_sequencial(v, v_ordenado);
    printf("Vetor ordenado sequencialmente: ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", v_ordenado[i]);
    }
    printf("\n");

    // Ordenação multithread sem sincronização
    ordenacao_multithread(v, v_ordenado);
    printf("Vetor ordenado multithread sem sincronização: ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", v_ordenado[i]);
    }
    printf("\n");

    // Ordenação multithread com sincronização
    ordenacao_multithread_sync(v, v_ordenado);
    printf("Vetor ordenado multithread com sincronização: ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", v_ordenado[i]);
    }
    printf("\n");

    return 0;
}