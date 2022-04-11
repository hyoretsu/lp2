#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Substring size */
int strSize = 9;
/** Number of substring occurences */
int count = 0;

struct funcArgs {
    char* str;
    int n;
};
typedef struct funcArgs FuncArgs;

void* threadSearch(FuncArgs* args){
    char content[strSize];
    printf("a");

    char filename[10] = "tmp/";
    snprintf(filename, 10, "%d.txt", args->n);
    printf("b");

    FILE* file = fopen(filename, "r");
    printf("c");

    while(fgets(content, strSize, file)) {
		if(strstr(content, args->str) != NULL) {
			count += 1;
		}
    };

    fclose(file);

    pthread_exit(&count);
}

int main(void) {
    char str[9];

    printf("Qual string você gostaria de procurar? (máx. 8 caractéres)\n");
    fgets(str, 9, stdin);
    printf("a");

    strSize = strlen(str);

    pthread_t id[5];

    for (int i = 1; i <= 5; i++) {
        FuncArgs funcArgs = {str, i};

        pthread_create(&id[i - 1], NULL, (void*) threadSearch, &funcArgs);
        pthread_join(id[i - 1], (void*) &count);
    }

    printf("%d", count);

    return 0;
}
