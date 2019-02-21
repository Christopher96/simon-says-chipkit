#include <stdio.h>

int main() {
    int n = 5;
    int sum = 1;
    int mult = 1;
    int multadd = 0;
    int i = 2;

    int j = 0;
    
    while(1) {
        j += i;
        j -= 1;
        while(1) {
            if(j == 0) break;
            sum += mult;
            j -= 1;
            multadd += sum;
        }
        multadd += mult;
        printf("%d, %d\n", mult, multadd);
        if(i == n) break;
        mult = sum;
        i++;
    }

    printf("%d\n", sum);
}
