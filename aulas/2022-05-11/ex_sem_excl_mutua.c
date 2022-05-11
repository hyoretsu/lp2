#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define QTD_THREADS 5

sem_t mutex;
int var_global = 0;

void* worker(void* p) {
    long idx = (long) p;

    sem_wait(&mutex);

    printf("[%ld] Dentro da seção crítica\n", idx);
    var_global++;
    sleep(1);

    sem_post(&mutex);

    printf("[%ld] Fora da seção crítica: %d\n", idx, var_global);
}

int main(void) {
    pthread_t w[QTD_THREADS];

    sem_init(&mutex, 0, 1);

    for (long i = 0; i < QTD_THREADS; i ++) {
        pthread_create(&w[i], 0, worker, (void*)i);
    }

    for (int i = 0; i < QTD_THREADS; i ++) {
        pthread_join(w[1], 0);
    }

    sem_destroy(&mutex);

    return 0;
}
