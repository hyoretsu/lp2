#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include "utils/createChildren.c"

#ifdef _WIN32
#include <dos.h>
#else
#include <unistd.h>
#endif

#define minJump 10
int rabbitN;
/** Meters */
int trackSize;
char* mode;

typedef struct {
    int id;
    int maxJump;
} RabbitRunDTO;

void args(int argc, char* argv[]) {
    if (argc == 1) {
        printf("Por favor, passe algum argumento.\n");
        exit(0);
    }

    int modeChosen = 0;

    for (int i = 1; i < argc; i++) {
        if (!modeChosen) {
            int isProcess = !strcmp(argv[i], "-p");
            int isThread = !strcmp(argv[i], "-t");

            if (isProcess || isThread) {
                if (argc < 3) {
                    printf("Por favor, informe a quantidade de lebres e o tamanho da pista.\n");
                    exit(0);
                } else if (argc < 4) {
                    printf("Por favor, informe o tamanho da pista.\n");
                    exit(0);
                }

                if (isProcess) {
                    mode = (char*) malloc(sizeof(char) * 8);
                    snprintf(mode, 8, "process");
                } else {
                    mode = (char*) malloc(sizeof(char) * 7);
                    snprintf(mode, 7, "thread");
                }
                modeChosen = 1;

                rabbitN = strtod(argv[i + 1], NULL);
                trackSize = strtod(argv[i + 2], NULL);

                i += 2;
                continue;
            }
        }

        printf("O argumento %s não foi reconhecido, tente novamente.", argv[i]);
        exit(0);
    }
}

void rabbitRun(RabbitRunDTO* params) {
    /** Meters */
    float progress = 0;

    srand(params->id);

    while (progress < trackSize) {
        progress != 0 && sleep(1);

        /** Centimeters */
        int distance = rand() % params->maxJump + minJump;
        progress += (float) distance / 100;

        printf("A lebre %d saltou %dcm e está em %.2f/%dm\n", params->id, distance, progress, trackSize);
    }

    printf("A lebre %d venceu a partida!\n", params->id);
    // Pequeno bug, o processo-pai não é morto imediatamente após sair do loop
    kill(0, SIGKILL);
}

int main(int argc, char* argv[]) {
    args(argc, argv);

    int i, maxJump = (trackSize * 100) / 20;
    pid_t original_pid = getpid();

    if (strcmp(mode, "process") == 0) {
        createChildren(rabbitN);

        pid_t pid = getpid();
        if (pid != original_pid){ // Child process
            RabbitRunDTO params = {pid, maxJump};

            rabbitRun(&params);
        }

        wait(NULL);
    } else if (strcmp(mode, "thread") == 0) {
        RabbitRunDTO params[rabbitN];
        pthread_t id[rabbitN];

        for (i = 0; i < rabbitN; i++) {
            params[i].id = i;
            params[i].maxJump = maxJump;

            pthread_create(&id[i], NULL, (void*) rabbitRun, &params[i]);
        }
        for (i = 0; i < rabbitN; i++) {
            pthread_join(id[i], NULL);
        }
    }

    return 0;
}
