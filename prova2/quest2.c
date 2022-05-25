#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/signal.h>
#include <time.h>
#include "../utils/initString.c"

// Faça um programa em C que 4 threads geram uma letra aleatória (caractere), e as depositam em uma palavra (string) na thread; então, a thread principal (função  main), depois de aguardar cada uma das threads depositar sua letra, deve exibir a string gerada. Posteriormente esta string deverá ser consumida, caractere por caractere, por uma thread consumidora (5a thread criada além da main). Esta thread receberá a string e contará quantas vogais nela constam, exibindo o valor. O processo deverá se repetir 5 vezes; finalmente, o programa deverá exibir o total de vogais de todas as palavras geradas e encerrar. Use barreiras (pthread.h) e semáforos (semaphore.h) e comente como suas funções foram utilizadas.

// Tava com pouco tempo mas eu ia tentar desenrolar o resto, só que aí o programa dá pau antes da 1a linha

#define threadN 4

pthread_barrier_t barrier;
int count, loopN = 5, totalVogals = 0;
char word[threadN];
sem_t finished;

void* genChar(void* arg) {
    long id = (long) arg;

    while (count < loopN) {
        char randomLetter = (rand() % 26) + 'A';
        word[id] = randomLetter;

        // Aguarda todas as threads gerarem e a consumidora
        pthread_barrier_wait(&barrier);
    }

    return NULL;
}

void* cons(void* args) {
    while (count < loopN) {
        pthread_barrier_wait(&barrier);
        printf("boy");

        int vogalCount = 0;

        for (int i = 0; i < threadN; i++) {
            if (word[i] == 'A' || word[i] == 'E' || word[i] == 'I' || word[i] == 'O' || word[i] == 'U') {
                vogalCount += 1;
            }
        }

        __atomic_fetch_add(&totalVogals, 1, 0);

        printf("A string (%s) gerada possui %d vogais.\n", word, vogalCount);
    }

    return NULL;
}

int main(void) {
        printf("a");
    pthread_barrier_init(&barrier, NULL, threadN + 1);
        printf("a");
    sem_init(&finished, 0, loopN);
        printf("a");
    srand(time(NULL));

    pthread_t id[threadN];
    for (long i = 0; i < threadN; i++) {
        pthread_create(&id[i], NULL, (void*) genChar, (void*) i);
    }

    pthread_barrier_destroy(&barrier);
    sem_destroy(&finished);

    return 0;
}
