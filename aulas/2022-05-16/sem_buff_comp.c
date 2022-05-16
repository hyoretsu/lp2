#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define BUFFER_SIZE 8
#define QTD_GENERATE 10

int buffer[BUFFER_SIZE];
int front = 0, rear = 0;
sem_t empty, full;

void* prod(void* p) {
    int count = 0;

    while (count < QTD_GENERATE) {
        sem_wait(&empty);

        int valor_prod = rand() % 255;
        printf("[PROD] %d\n", valor_prod);

        buffer[rear] = valor_prod;
        rear = (rear + 1) % BUFFER_SIZE;

        sleep(1);

        sem_post(&full);
        count +=1;
    }
}

void* cons(void* p) {
    int count = 0;

    while (count < QTD_GENERATE) {
        sem_wait(&full);

        int valor_cons = buffer[front];
        front = (front + 1) % BUFFER_SIZE;

        printf("[CONS] %d\n", valor_cons);
        sleep(2);

        sem_post(&empty);
        count +=1;
    }
}

int main(void) {
    srand(time(NULL));

    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = 0;
    }

    sem_init(&empty, 0, BUFFER_SIZE);
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
