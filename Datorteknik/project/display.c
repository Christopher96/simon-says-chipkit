#include "header.h"

#include <pic32mx.h>  

typedef enum { INSTR, DATA } Mode;

char lcdread(Mode md) {
    char c;

    /* TRISE = 0xFFFF;                 // set port E 0-7 as input */
    TRISESET = pinModeFT(0, 7);                 // set pins 0-7 on E to input
    /* PORTCbits.RC14 = (md == DATA);  // set instruction to data mode */
    pinSetMode(&PORTD, RS, md == DATA);     // instruction or data mode?
    /* PORTCbits.RC13 = 1;             // read mode */
    PORTDSET = pinMode(RW);                 // read mode
    /* PORTCbits.RC15 = 1;             // pulse enable */
    PORTDSET = pinMode(E);                 // enable on
    delaymicros(1000);                        // wait for response
    /* c = PORTE & 0x00FF;             // read a byte from port E */
    c = pinReadFT(PORTE, 0, 7);              // read byte from port E
    /* PORTCbits.RC15 = 0;             // turn off enable */
    PORTDCLR = pinMode(E);                  // enable off
    delaymicros(1000);                        // wait for response
}

void lcdbusywait(void) {
    char state;
    do {
        state = lcdread(INSTR);     // read instruction
        printBinaryRow(1, state);
    } while(state & 0x80);          // repeat until busy flag is clear
}

char lcdwrite(char val, Mode md) {
    /* TRISE = 0xFF00;                 // set port E 0-7 as output */
    TRISECLR = pinModeFT(0, 7);              // set pins 0-7 on E to output
    /* PORTCbits.RC14 = (md == DATA);  // set instruction to data mode */
    pinSetMode(&PORTD, RS, md == DATA);     // instruction or data mode?
    PORTDCLR = pinMode(RW);                 // write mode
    PORTE = val;                            // write a character
    /* PORTCbits.RC15 = 1;             // pulse enable */
    PORTDSET = pinMode(E);                  // enable on
    delaymicros(1000);                        // wait for response
    /* PORTCbits.RC15 = 0;             // turn off enable */
    PORTDCLR = pinMode(E);                  // enable off
    delaymicros(1000);                        // wait for response
}

char lcdprintstring(char *str) {
    while(*str != 0) {                      // iterate until null character is found
        lcdwrite(*str, DATA);               // print character
        lcdbusywait();
        str++;                              // next character
    }
}

void lcdclear(void) {
    lcdwrite(0x01, INSTR);                  // clear display
    delaymicros(15300);                      // wait for execution
}

void initlcd(void) {
    // set LCD control pins
    TRISDCLR = pinModeAll(3, RS, RW, E);    // set controls to output
    PORTDCLR = pinModeAll(3, RS, RW, E);    // turn off controls

    print(3, "sending");
    // send instructions to initialize the display
    delaymicros(150000);
    lcdwrite(0x30, INSTR);                  // 8 bit mode
    delaymicros(41000);
    lcdwrite(0x30, INSTR);                  // 8 bit mode
    delaymicros(1000);
    lcdwrite(0x30, INSTR);                  // 8 bit mode
    print(3, "waiting");
    lcdbusywait();
    print(3, "sending");
    lcdwrite(0x3C, INSTR);                  // set 2 lines, 5x8 font
    print(3, "waiting");
    lcdbusywait();
    lcdwrite(0x08, INSTR);                  // turn off display
    lcdbusywait();
    print(3, "clearing screen");
    lcdclear();
    lcdwrite(0x06, INSTR);                  // set entry mode to increment cursor
    print(3, "waiting");
    lcdbusywait();
    lcdwrite(0x0C, INSTR);                  // turn display on with no cursor
    lcdbusywait();
    print(4, "done");
}
