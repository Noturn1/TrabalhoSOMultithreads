#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void selecao(int *v, int n)
{

    int i, j, menor, temp; // menor e o menor elemento encontrado
    for (i = 0; i < n - 1; i++)
    {
        menor = i;
        for (j = i + 1; j < n; j++)
        {
            if (v[j] < v[menor])
            {
                menor = j;
            }
        }
        if (i != menor)
        {
            temp = v[i];
            v[i] = v[menor];
            v[menor] = temp;
        }
    }
}

int main(int argc, char **argv)
{
    int v[10] = {100, 40, 60, 20, 30, 15, 0, 110, 105};
    int n = 9;
    selecao(v, n);
    for (int i = 0; i < 9; i++)
        printf("%d ", v[i]);
    return 0;
}



