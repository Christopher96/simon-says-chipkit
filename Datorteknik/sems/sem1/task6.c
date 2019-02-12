#include <stdio.h>

void square_reverse(const double *x, double *y, const int len) {
    for(int i = 0; i < len; i++) {
        double val = x[i];
        double ans = val*val;
        int index = len-1-i;
        y[index] = ans;
    }
}

int main() {
    double in[] = {11.0 , 20.0, 100.0};
    double out[3];

    square_reverse(in, out, 3);

    for(int i = 0; i < 3; i++) {
        printf("%f\n", out[i]);
    }
    return 0;
}
