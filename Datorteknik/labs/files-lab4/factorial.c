#include <stdio.h>

int main() {
    int n = 5;
    int sum = 1;
    int mult = 1;
    int multadd = 1;
    int i = 2;

    int j = 0;
    
    while(1) {
        j += i;
        j -= 1;
        while(1) {
            if(j == 0) break;
            /* printf("%d + %d\n", sum, multiplier); */
            sum += mult;
            j -= 1;
            if(j == 1) continue;
            multadd += mult;
        }
        printf("%d, %d\n", multadd, mult);
        if(i == n) break;
        mult = sum;
        i++;
    }

    printf("%d\n", sum);
}
