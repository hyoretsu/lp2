#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/signal.h>
#include <time.h>
#include "../utils/checkPalindrome.c"
#include "../utils/initString.c"

#define threadN 6

pthread_barrier_t barrier;
int count, loopN = 10, wordSize = threadN + 1;
char *word;

void genChar(int id) {
    while (count < loopN) {
        char randomLetter = (rand() % 26) + 'A';
        word[id] = randomLetter;

        pthread_barrier_wait(&barrier);
    }
}

int main(void) {
    pthread_barrier_init(&barrier, NULL, wordSize);
    srand(time(NULL));

    initString(&word, wordSize);
    pthread_t id[threadN];
    for (long i = 0; i < threadN; i++) {
        pthread_create(&id[i], NULL, (void*) genChar, (void*) i);
    }

    for (count = 0; count < loopN; count++) {
        pthread_barrier_wait(&barrier);

        int isPalindrome = checkPalindrome(word, wordSize);
        printf("A palavra \"%s\" %s um palíndromo\n", word, isPalindrome ? "é" : "não é");
    }

    pthread_barrier_destroy(&barrier);

    return 0;
}
