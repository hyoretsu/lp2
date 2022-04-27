#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int lock = 0, var_global = 0;

void* cs(void* p) {
    long idx = (long)p;

    while (1) {
        while(__atomic_test_and_set(&lock, 1));
        var_global++;
        printf("sc %ld vg: %d\n", idx, var_global);
        puts("..");
        lock = 0;
        // sleep(5);
        usleep(50);
        printf("sc %ld vg: %d\n", idx, var_global);
    }
}

int main(void) {
    pthread_t threads[2];

    pthread_create(&threads[0], NULL, cs, (void*)1);
    pthread_create(&threads[1], NULL, cs, (void*)2);
    sleep(10);

    return 0;
}
