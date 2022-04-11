#include <stdio.h>
#include <stdlib.h>
#include "./utils/createChildren.c"

int main() {
    int quantity;

    printf("Quantos processos você deseja criar? ");
    scanf("%d", &quantity);

    createChildren(quantity);

    printf("alo\n");

    return 0;
}
