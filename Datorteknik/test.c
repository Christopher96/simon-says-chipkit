#include <stdlib.h>
#include <stdio.h>

typedef enum { OUTPUT, INPUT } Pinmode;
typedef enum { LOW, HIGH } Signal;

/* int arrPush(int* arr, int* size, int n) { */
/*     *size = *size+1; */
/*     *(arr++) = n; */
/* } */
/* int* arrPush(int* arr, int* size, int n) { */
/*     *size = *size+1; */
/*     int new_arr[*size]; */
/*  */
/*     for (int i = 0; i < *size; i++) { */
/*         new_arr[i] = arr[i]; */
/*     } */
/*  */
/*     new_arr[*size] = n; */
/*     return new_arr; */
/* } */
/*  */
/* int arrPush(int* arr, int* size, int n) { */
/*     int* new_arr = (int *) malloc(*size * sizeof(int)); */
/*     for(int i = 0; i < *size; i++) { */
/*         *(new_arr + i) = *(arr + i); */
/*     } */
/*     *(new_arr + *size) = n; */
/*     free(arr); */
/*     arr = new_arr; */
/*  */
/*     *size = *size + 1; */
/* } */
/*  */

void pinFromToMode(int *port, int from, int to, Pinmode md) {
    int numbits = to-from;

    if(numbits < 0) return; // invalid range

    int mask = (md == INPUT) ? ~0 : 0;

    mask = ~((mask >> numbits) << numbits);
    mask <<= from;

    *port = mask;
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
    
    /* addPin(10); */

    int size = 0;
    int game_leds[] = { 1, 2, 3, 4, 5 };

    game_leds = arrPush(game_leds, &size, 10);

    for (int i = 0; i < size; i++) {
        printf("%d\n", game_leds[i]);
    }
}
