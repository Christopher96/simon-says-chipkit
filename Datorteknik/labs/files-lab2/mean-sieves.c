#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void mean_sieves(int n) {
    int *primes = (int *) malloc(n * sizeof(int));

    if(primes == NULL) {
        printf("No memory was allocated.");
        return;
    }

    for(int i = 0; i < n; i++) {
        *(primes + i) = 1;
    }

    for(int i = 2; i < sqrt(n); i++) {
        if(*(primes + i)) {
            for(int j = pow(i,2), k = 0; j < n; j = pow(i,2) + k*i, k++) {
                *(primes + j) = 0;
            }
        }
    }

    int sum = 0;
    int count = 0;

    // Only add primes to the sum and count them
    for(int i = 1; i < n; i++) {
        if(*(primes + i)) {
            sum += i;
            count++;
        }
    }

    // Divide the sum of all primes by the number of elements to receive the mean value
    double mean = (double)sum / (double)count;

    // Print out mean value
    printf("%f", mean);

    free(primes);
}

int main(int argc, char *argv[]){
    if(argc == 2)
        mean_sieves(atoi(argv[1]));
    else
        printf("Please state an integer number.\n");
    return 0;
}


