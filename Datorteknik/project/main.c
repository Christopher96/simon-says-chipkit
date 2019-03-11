
#include <stddef.h> 
#include <stdint.h> 
#include <pic32mx.h>

#include "header.h"

void u32init(void);
void display_init(void);
void display_string(int line, char *s);
void display_update(void);

void print_binary(int bin);
void init(void);

// PORT F
#define RS  2
#define RW  3

// PORT D
#define E   8

int main() {

    init();

    resetTimer(2);
    startTimer(2);

    initGame();

    return 0;
    initlcd();
    char* msg = "swag";
    lcdprintstring(msg);
    return 0;
}

void init() {
    u32init();
    display_init();
}


