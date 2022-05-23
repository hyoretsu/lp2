#include <string.h>

#include<stdio.h>
int checkPalindrome(const char* string, int length) {
    for (int i = 0; i < length; i++) {
        if (string[i] != string[(length - 1) - i]) return 0;
    }

    return 1;
}
