#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define QTD_THREADS 5

int var_global = 0;

int number = 1, next = 1;
int turn[QTD_THREADS];

void* cs(void* p) {
    long idx = (long) p;
    printf("[%ld] iniciou\n", idx);

    while(1) {
        turn[idx] = __atomic_fetch_add(&number, 1, 0);
        while(turn[idx] != next) usleep(100);

        var_global += 1;
        printf("[%ld] vg: %d\n(ticket %d)", idx, var_global, turn[idx]);
        puts("..");

        __atomic_fetch_add(&next, 1, 0);
    }
}

int main(void) {
    pthread_t threads[QTD_THREADS];

    for (int i = 0; i < QTD_THREADS; i++) {
        turn[i] = 0;
    }

    for (long i = 0; i < QTD_THREADS; i++) {
        pthread_create(&threads[i], NULL, cs, (void*) i);
    }

    sleep(10);

    return 0;
}
