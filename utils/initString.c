#include <stdlib.h>
#include <string.h>

/**
 * Initializes a string.
 *
 * @param string Pointer to string being initialized.
 * @param length Length of the string (including '\0').
 */
int initString(char** string, int length) {
    *string = (char*) malloc(sizeof(char) * length);
    if (*string == NULL) {
        return 0;
    }

    for (int i = 0; i < length; i++) {
        (*string)[i] = '\0';
    }

    return 1;
}
