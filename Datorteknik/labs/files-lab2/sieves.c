#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define COLUMNS 6

int col = 0;

void print_number(int n) {
    printf("%10d ", n);
    col++;
    if(col == COLUMNS) {
        col = 0;
        printf("\n");
    }
}

void print_sieves(int n) {
    char primes[n+1];

    for(int i = 0; i <= n; i++) {
        primes[i] = 1;
    }

    for(int i = 2; i <= sqrt(n); i++) {
        if(primes[i]) {
            for(int j = i*i; j <= n; j += i) {
                primes[j] = 0;
            }
        }
    }

    for(int i = 2; i <= n; i++) {
        if(primes[i]) {
            print_number(i);
        }
    }
}

int main(int argc, char *argv[]){
    if(argc == 2)
        print_sieves(atoi(argv[1]));
    else
        printf("Please state an integer number.\n");
    return 0;
}


