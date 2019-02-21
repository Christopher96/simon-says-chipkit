#include <stdio.h>

int main() {
    int n = 5;
    int i = 1;
    int sum = 1;

    while(1) {
        if(i == n) break;
        sum += i*sum;
        i++;
    }
    printf("%d\n", sum);
}
