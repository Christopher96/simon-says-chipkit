#include "header.h"

#include <stddef.h> 
#include <stdint.h> 
#include <pic32mx.h>

void u32init(void);
void display_init(void);
void display_string(int line, char *s);
void display_update(void);

void delay(int ms);
void print_binary(int bin);
void init(void);

int main() {
    init();

    initlcd();
    lcdprintstring("hello world");

    return 0;

    TRISECLR = pinsMode(3, 5);
    print_bin_row(0, pinRead(PORTE, 3));
    PORTE = pinsMode(3, 5);
    pinSetMode(&PORTE, 7, (1==1));
    int res = pinsRead(PORTE, 0, 7);
    print_bin_row(1, res);

    display_update();

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


