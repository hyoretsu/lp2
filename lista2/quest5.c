#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

#define bufferSize 10

const int fullSize = 100, maxValue = 255;
int buffer[bufferSize];
sem_t empty, full;

void prod() {
    int count = 0;

    while (count < fullSize) {
        sem_wait(&empty);

        int valor_prod = rand() % (maxValue + 1);
        buffer[count % bufferSize] = valor_prod;

        sem_post(&full);
        count += 1;
    }
}

void cons() {
    int count = 0;

    while (count < fullSize) {
        sem_wait(&full);

        printf("%d° número: %d\n", count + 1, buffer[count % bufferSize]);

        sem_post(&empty);
        count += 1;
    }
}

int main(void) {
    srand(time(NULL));

    sem_init(&empty, 0, bufferSize);
    sem_init(&full, 0, 0);

    pthread_t _prod, _cons;
    pthread_create(&_prod, NULL, (void*) prod, NULL);
    pthread_create(&_cons, NULL, (void*) cons, NULL);

    pthread_join(_prod, NULL);
    pthread_join(_cons, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
