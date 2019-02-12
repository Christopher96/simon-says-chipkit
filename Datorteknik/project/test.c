#include <stdio.h>

void print_binary(int bin) {
    int max = 32;
    char str[max];
    int i = 0;

    while(bin >> i) {
        i++;
    }

    str[i] = '\0';

    while(i > 0) {
        i -= 1;
        if(bin & 1) {
            str[i] = '1';
        } else {
            str[i] = '0';
        }
        bin >>= 1;
    }

    printf("%s", str);
}

int main() {
    int bin = 10;
    print_binary(bin);
}
