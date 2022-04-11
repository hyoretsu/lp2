#include <math.h>
#include <unistd.h>

/**
 * Multiplies the process until it reaches a desired number.
 *
 * @param quantity Number of total processes to be created, including parent.
 */
void createChildren(int quantity) {
    int originalProcess = getpid(), iteration = 1;

    while(quantity != 0) {
        // Register nearest power of 2 and calculate remainder
        int remaining = log2(quantity);
        quantity -= pow(2, remaining);

        // Create as many processes at once as possible
        for (int i = 0; i < remaining; i++) {
            fork();
        }

        // Create an extra process, otherwise we'll be missing 1 each *new* iteration
        if (getpid() == originalProcess) {
            iteration != 1 && fork();
            iteration += 1;
        }

        // Stop loop for all clones, including the newly created one
        if (getpid() != originalProcess) break;
    }
}

