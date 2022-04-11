#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include "../utils/createChildren.c"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define PROCESS_COUNT 30
#define SLEEP_TIME 2

int threadsTime = 0;

void* threadSearch(){
    printf("Nova thread\n");
    // sleep(SLEEP_TIME);

    pthread_exit(&threadsTime);
}

int main() {
    int i;
    clock_t startTime = clock();

    pthread_t id[30];
    for (i = 1; i <= 30; i++) {
        pthread_create(&id[i - 1], NULL, threadSearch, NULL);
        pthread_join(id[i - 1], (void*) &threadsTime);
    }

    clock_t endTime = clock();
    printf("Tempo necessário para criar 30 threads: %f\n", (double) (endTime - startTime) / CLOCKS_PER_SEC);

    createChildren(31);
    printf("Novo processo\n");
    // sleep(2);

    return 0;
}
