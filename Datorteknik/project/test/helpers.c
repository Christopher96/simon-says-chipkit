#include "header.h"

int pinMode(int pin) {
    return (0x1 << pin); 
}

void pinSetMode(volatile int* port, int pin, Pinmode md) {
    int bit = (0x1 << pin);

    if(md == INPUT) {
        *port |= bit; 
    } else {
        *port ^= bit;
    }
}

int pinRead(int val, int pin) {
    return val & (0x1 << pin);
}

int pinMask(int from, int to) {
    int numbits = to-from;
    int mask = ~0;

    mask = ~((mask >> numbits) << numbits);
    mask <<= from;
    return mask;
}

int pinsMode(int from, int to) {
    return pinMask(from, to);
}

int pinsRead(int val, int from, int to) {
    return val & pinMask(from, to);
}


void delay(int ms) {
    int c = 4711;
    for(int i = 0; i < ms; i++)
        for(int j = c; j > 0; j--)
            ;
}

void print_binary(int bin) {
    print_bin_row(0, bin);
}

void print_bin_row(int row, int bin) {
    int max = 32;
    char str[max];
    int i = 1;

    while(bin >> i) {
        i++;
    }

    str[i] = '\0';

    while(i > 0) {
        i -= 1;
        if(bin & 1) {
            str[i] = '1';
        } else {
            str[i] = '0';
        }
        bin >>= 1;
    }

    display_string(row, str);
    display_update();
}
