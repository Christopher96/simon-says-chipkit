#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

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
    to+=1;
    int numbits = to-from;
    int mask = ~0;

    mask = ~((mask >> numbits) << numbits);
    mask <<= from;
    return mask;
}

int pinModeFT(int from, int to) {
    return pinMask(from, to);
}

int pinReadFT(int val, int from, int to) {
    return val & pinMask(from, to);
}

int pinModeAll(int n_args, ...) {
    va_list ap;
    va_start(ap, n_args);

    int pin = 0;
    int mask = 0;
    for(int i = 0; i < n_args; i++) {
        pin = va_arg(ap, int);
        mask |= (1 << pin);
    }

    va_end(ap);
    return mask;
}


void delay(int ms) {
    int c = 4711;
    for(int i = 0; i < ms; i++)
        for(int j = c; j > 0; j--)
            ;
}

void print(int row, char* msg) {
    display_string(row, msg);
    display_update();
}

void printBinary(int bin) {
    printBinaryRow(0, bin);
}

void printBinaryRow(int row, int bin) {
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

unsigned long int next = 1;

int rand(void) {
    next = next * 1103515245 + 12345;
    return (unsigned int)(next/65536) % 32768;
}
