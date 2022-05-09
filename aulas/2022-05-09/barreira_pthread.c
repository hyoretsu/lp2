#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define QTD_THREADS 5
#define LIMITE_CONJ_JOBS 3

pthread_barrier_t barreira;
int contador_conj_jobs = 0;

void* worker(void* p) {
    while (contador_conj_jobs < LIMITE_CONJ_JOBS) {
        long idx = (long) p;
        int sleep_time = rand() % 8 + 1;

        printf("[%ld] Job: %d\n", idx, sleep_time);
        // sleep(idx + 1);
        sleep(sleep_time);
        printf("[%ld] Terminei o job\n", idx);

        pthread_barrier_wait(&barreira);

        printf("[%ld] Passei da barreira\n", idx);
        fflush(stdout);
    }
}

int main(void) {
    srand(time(NULL));

    pthread_t threads[QTD_THREADS];
    pthread_barrier_init(&barreira, NULL, QTD_THREADS + 1);

    for (long i = 0; i < QTD_THREADS; i++) {
        pthread_create(&threads[i], NULL, worker, (void*) i);
    }

    while (contador_conj_jobs < LIMITE_CONJ_JOBS) {
        printf("[main] Conjunto: %d\n", contador_conj_jobs);
        pthread_barrier_wait(&barreira);

        __atomic_fetch_add(&contador_conj_jobs, 1, 0);
    }

    // for (int i = 0; i < QTD_THREADS; i++) {
    //     pthread_join(threads[i], NULL);
    // }

    int ret = pthread_barrier_destroy(&barreira);

    printf("[main] BARRIER DESTROY: %d", ret);

    return 0;
}
