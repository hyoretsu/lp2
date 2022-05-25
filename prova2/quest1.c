#include <pthread.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define threadsN 10

atomic_int count = 0;
int lockX = 0, lockY = 0;
float x = 0, y = 0;

// Se tirar o \n dos printf trava, e às vezes o programa não termina. Antes da adição tem uma gambiarra, se não às vezes extrapolava a condição do while.

void* funcX(void* args) {
    while (x + y <= 100) {
        int randomN = rand() % 5 + 1;

        // Protocolo de entrada. Necessita de uma instrução atômica
        while (__atomic_test_and_set(&lockX, 1)) usleep(50);

        if ((x + y) <= 100) x += randomN;
        printf("total: %f (x)\n", x + y);

        // Protocolo de saída. Já que estamos em exclusão mútua, não precisa ser uma instrução atômica
        lockX = 0;

        usleep(250 * 1000);
        // Incrementa a barreira para sincronizar as threads através de uma função atômica, por ser mais rápida que mutex
        atomic_fetch_add(&count, 1);

        // Espera todas as threads terminarem (e garante que nenhuma vai incrementar 2 vezes no mesmo ciclo)
        while ((count % threadsN) > 0) usleep(50);
    }

}

void* funcY(void* args) {
    while (x + y <= 100) {
        int randomN = rand() % 25 + 2;

        // Protocolo de entrada. Necessita de uma instrução atômica
        while (__atomic_test_and_set(&lockY, 1)) usleep(50);

        if ((x + y) <= 100) y += randomN;
        printf("total: %f (y)\n", x + y);

        // Protocolo de saída. Já que estamos em exclusão mútua, não precisa ser uma instrução atômica
        lockY = 0;

        usleep(300 * 1000);
        // Incrementa a barreira para sincronizar as threads através de uma função atômica, por ser mais rápida que mutex
        atomic_fetch_add(&count, 1);

        // Espera todas as threads terminarem (e garante que nenhuma vai incrementar 2 vezes no mesmo ciclo)
        while ((count % threadsN) > 0) usleep(50);
    }

}

int main(void) {
    srand(time(NULL));
    pthread_t id[threadsN];

    for (int i = 0; i < threadsN; i++) {
        if (i % 2 == 0) {
            pthread_create(&id[i], NULL, funcX, NULL);
        } else {
            pthread_create(&id[i], NULL, funcY, NULL);
        }
    }

    for (int i = 0; i < threadsN; i++) {
        pthread_join(id[i], NULL);
    }

    return 0;
}
