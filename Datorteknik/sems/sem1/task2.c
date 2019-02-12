#include <stdio.h>

int main() {
    int x = 0x00014000; // 01010 (13-17)
    int y = 0x00000005; // 101 (1-3) 
    int z = 0x12345678;

    // 101 01010 = 170
    z = (z & 0xFFFFFF00) | (((x & 0x0003D000) >> 13) | ((y & 7) << 5));
    char hex[8];
    sprintf(hex, "%x", z);
    printf("%s", hex);
}
