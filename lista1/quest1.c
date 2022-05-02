#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../utils/createChildren.c"

#ifdef _WIN32
#include <dos.h>
#else
#include <unistd.h>
#endif

#define QUANTITY 30
#define SLEEP_TIME 2

void threadSearch(){
    printf("Nova thread\n");
    sleep(SLEEP_TIME);
}

int main() {
    int i;
    pid_t originalPid = getpid();
    clock_t startTime = clock();

    pthread_t id[QUANTITY];
    for (i = 1; i <= QUANTITY; i++) {
        pthread_create(&id[i - 1], NULL, threadSearch, NULL);
    }

    clock_t endTime = clock();
    printf("Tempo necessário para criar %d threads: %f ms\n", QUANTITY, (double) (endTime - startTime) / (CLOCKS_PER_SEC / 1000));

    startTime = clock();

    createChildren(QUANTITY + 1);

    if (getpid() != originalPid) {
        printf("Novo processo\n");
        sleep(2);
        exit(0);
    }

    endTime = clock();

    printf("Tempo necessário para criar %d processos: %f\n ms", QUANTITY, (double) (endTime - startTime) / (CLOCKS_PER_SEC / 1000));

    return 0;
}
