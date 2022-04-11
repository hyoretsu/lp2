#include <stdlib.h>
#include <string.h>

void strtrim(char *str) {
    while(isspace(*str)) str++;

    char* back = str + strlen(str);
    while(isspace(*--back));
    *(back+1) = '\0';
}
