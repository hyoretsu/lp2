#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#ifdef _WIN32
#include <dos.h>
#else
#include <unistd.h>
#endif

// Concorrência é executar diferentes programas em paralelo. Se dão, pelo menos principalmente, através de processos e threads. Na sua criação, a ordem de criação/execução fica a cargo do escalonador, que pode executá-las durante ou somente após o processo pai. No caso de processos, toda a região de memória é copiada. Processos podem esperar outros processos terminar de executar através de waitpid() ou wait() e threads através de pthread_join().

// static int *random;
double count = 80;

void func(double* n) {
    int num = (int) *n;
    for (int i = 0; i < 50; i++) {
        if (num % 2 == 0) {
            count += 1;
        } else {
            if (i >= 30) break;
            count -= 1;
        }

        printf("Thread %d, novo valor da variável global: %lf\n", num + 1, count);
    }

    // if (n % 2 == 0) {
    //     for (int i = 0; i < 50; i++) {
    //         count += 1;
    //     }
    // } else {
    //     for (int i = 0; i < 30; i++) {
    //         count -= 1;
    //     }
    // }

    // printf("Thread %d, novo valor da variável global: %lf", n + 1, count);

    if (num % 2 == 0) {
        int random = rand() % 4 + 1;
        sleep(random);
    } else {
        sleep(2);
    }

// - depois dos laços, as threads cujo identificador for par devem dormir um tempo
// aleatório entre 1s e 4s; as threads cujo identificador for impar dormem um
// tempo fixo de 2s.
// A cada iteração essa função deve exibir o identificador da thread e o novo valor da variável global
}

int main(void) {
    // random = mmap(NULL, sizeof *random, PROT_READ | PROT_WRITE,
    //                 MAP_SHARED | MAP_PRIVATE, -1, 0);

    // shmget();
    int random = 0;

    int status = 0;

    // Forked process
    // if (fork() == 0) {
        srand(time(NULL));
        random = rand() % 99 + 0;

    //     printf("%d\n", random);

    //     exit(0);
    // }

    wait(NULL);

    pthread_t id[10];
    for (int i = 0; i <= 0; i++) {
        pthread_create(&id[i - 1], NULL, (void*) func, (double*) &i);
        pthread_join(id[i - 1], NULL);
    }

    printf("%lf\n", count);

    return 0;
}

// - faça o programa criar um processo que gere um valor aleatório (entre 0 e 99)
// e faça este processo retornar o valor para o processo original, que estará
// bloqueado, aguardando.
