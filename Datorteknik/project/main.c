
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
    enableChangeNotification();

    int led_btns = pinModeAll(4, B_BLUE, B_GREEN, B_RED, B_YELLOW);
    TRISDSET = led_btns;

    while(1) {
        printBinaryRow(0, IFS(1));
        printBinaryRow(1, IFS(0));
    }
    return 0;

    initlcd();
    char* msg = "swag";
    lcdprintstring(msg);
    return 0;


    char* text;
    int switches;

    TRISD = 0xFF00; // set RD[7:0] to output and RD[11:8] to input

    while(1) {
        switches = (PORTD >> 8) && 0xF;  // Read and mask switches from RD[11:8]
        PORTD = switches;                // display on the LEDs
        
        print_binary(PORTD);

        delay(10);
    }
}

void init() {
    u32init();
    display_init();
}


