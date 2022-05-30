#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define QTD_THREADS 5

int running = 1;
sem_t garfos[QTD_THREADS];
int eat_count[QTD_THREADS], think_count[QTD_THREADS];

void eat(long i) {
    eat_count[i] += 1;
    printf("[%ld] Comendo (%d)...\n", i, eat_count[i]);
    sleep(i);
}

void think(long i) {
    think_count[i] += 1;
    printf("[%ld] Pensando (%d)...\n", i, think_count[i]);
    sleep(i);
}

void* filosofo(void* p) {
    long idx = (long) p;

    // Special philosopher
    if (idx == (QTD_THREADS - 1)) {
        while(running) {
                sem_wait(&garfos[0]);
                sem_wait(&garfos[idx]);
                eat(idx);
                sem_post(&garfos[0]);
                sem_post(&garfos[idx]);
                think(idx);
        }
    } else { // Normal philosophers
        while (running) {
                sem_wait(&garfos[idx]);
                sem_wait(&garfos[idx + 1]);
                eat(idx);
                sem_post(&garfos[idx]);
                sem_post(&garfos[idx + 1]);
                think(idx);
        }
    }
}

int main(void) {
    pthread_t phi[QTD_THREADS];

    for (int i = 0; i < QTD_THREADS; i++) {
        sem_init(&garfos[i], 0, 1);

        eat_count[i] = 0;
        think_count[i] = 0;
    }

    for (long i = 0; i < QTD_THREADS; i++) {
        pthread_create(&phi[i], NULL, filosofo, (void*) i);
    }

    sleep(30);
    running = 0;

    for (int i = 0; i < QTD_THREADS; i++) pthread_join(phi[i], 0);

    for (int i = 0; i < QTD_THREADS; i++) {
        sem_destroy(&garfos[i]);
    }

    return 0;
}
