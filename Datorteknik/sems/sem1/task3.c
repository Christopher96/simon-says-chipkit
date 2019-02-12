#include <stdio.h>

void adder(const int *x, const int *y, int *z) {
    *z = *x + *y;
}

int foo(int a) {
    const int k = 10;

    int z;
    adder(&a, &k, &z);
    return z;
}

int main() {
    int result = foo(7);
    printf("%d", result);
    return 0;
}
