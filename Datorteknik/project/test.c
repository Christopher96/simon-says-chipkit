#include <stdio.h>

typedef enum { OUTPUT, INPUT } Pinmode;
typedef enum { LOW, HIGH } Signal;


void pinFromToMode(int *port, int from, int to, Pinmode md) {
    int numbits = to-from;

    if(numbits < 0) return; // invalid range

    int mask = (md == INPUT) ? ~0 : 0;

    mask = ~((mask >> numbits) << numbits);
    mask <<= from;

    *port = mask;
    print_binary_full(*port);
}

void print_bin(int bin, int full) {
    int max = 32;
    char str[max];
    int i = (full) ? max : 0;

    if(!full) {
        while(bin >> i) {
            i++;
        }
    }

    str[(full) ? max : i] = '\0';

    while(i > 0) {
        i -= 1;
        if(bin & 1) {
            str[i] = '1';
        } else {
            str[i] = '0';
        }
        bin >>= 1;
    }

    printf("%s\n", str);
}

void print_binary_full(int bin) {
    print_bin(bin, 1);
}

void print_binary(int bin) {
    print_bin(bin, 0);
}

int main() {
    int port = 123;
    pinFromToMode(&port, 5, 10, OUTPUT); 
}
