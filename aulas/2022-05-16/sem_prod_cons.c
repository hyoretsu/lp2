#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define QTD_GENERATE 10

int buffer = 0;
sem_t empty, full;

void* prod(void* p) {
    int count = 0;

    while (count < QTD_GENERATE) {
        sem_wait(&empty);

        int valor_prod = rand() % 255;
        printf("[PROD] %d\n", valor_prod);

        buffer = valor_prod;

		sleep(2);

        sem_post(&full);
        count += 1;
    }
}

void* cons(void* p) {
    int count = 0;

    while (count < QTD_GENERATE) {
        sem_wait(&full);

        int valor_cons = buffer;
        printf("[CONS] %d\n", valor_cons);

        sleep(1);

        sem_post(&empty);
        count += 1;
    }
}

int main(void) {
    srand(time(NULL));

    sem_init(&empty, 0, 1);
    sem_init(&full, 0, 0);

    pthread_t _prod, _cons;
    pthread_create(&_prod, NULL, prod, NULL);
    pthread_create(&_cons, NULL, cons, NULL);

    pthread_join(_prod, NULL);
    pthread_join(_cons, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
