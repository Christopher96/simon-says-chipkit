#include "header.h"

#include <pic32mx.h>  

typedef enum { INSTR, DATA } Mode;

#define RS  14   // a5
#define RW  12   // a4
#define E   10   // a3 

char lcdread(Mode md) {
    char c;

    /* TRISE = 0xFFFF;                 // set port E 0-7 as input */
    TRISE = pinsMode(0, 7);                 // set pins 0-7 on E to input
    /* PORTCbits.RC14 = (md == DATA);  // set instruction to data mode */
    pinSetMode(&PORTB, RS, md == DATA);     // instruction or data mode?
    /* PORTCbits.RC13 = 1;             // read mode */
    PORTBSET = pinMode(RW);                 // read mode
    /* PORTCbits.RC15 = 1;             // pulse enable */
    PORTBSET = pinMode(RW);                 // enable on
    delaymicros(10);                        // wait for response
    /* c = PORTE & 0x00FF;             // read a byte from port E */
    c = pinsRead(PORTB, 0, 7);              // read byte from port B
    /* PORTCbits.RC15 = 0;             // turn off enable */
    PORTBCLR = pinMode(E);                  // enable off
    delaymicros(10);                        // wait for response
}

void lcdbusywait(void) {
    char state;
    do {
        state = lcdread(INSTR);     // read instruction
    } while(state & 0x80);          // repeat until busy flag is clear
}

char lcdwrite(char val, Mode md) {
    /* TRISE = 0xFF00;                 // set port E 0-7 as output */
    TRISECLR = pinsMode(0, 7);              // set pins 0-7 on E to output
    /* PORTCbits.RC14 = (md == DATA);  // set instruction to data mode */
    pinSetMode(&PORTB, RS, md == DATA);     // instruction or data mode?
    /* PORTCbits.RC13 = 0;             // write mode */
    PORTBCLR = pinMode(RW);                 // write mode
    PORTE = val;                            // write a character
    /* PORTCbits.RC15 = 1;             // pulse enable */
    PORTBSET = pinMode(E);                  // enable on
    delaymicros(10);                        // wait for response
    /* PORTCbits.RC15 = 0;             // turn off enable */
    PORTBCLR = pinMode(E);                  // enable off
    delaymicros(10);                        // wait for response
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
    delaymicros(1530);                      // wait for execution
}

void initlcd(void) {
    // set LCD control pins
    /* TRISC = 0x1FFF;                 // set port C 13-15 to output and other to input */
    TRISC = pinMode(0, 12);                 // set ports 0-12 to input
    /* PORTC = 0x0000;                 // turn off all controls */
    PORTB = 0x0;                            // turn off controls

    // send instructions to initialize the display
    delaymicros(15000);
    lcdwrite(0x30, INSTR);                  // 8 bit mode
    delaymicros(4100);
    lcdwrite(0x30, INSTR);                  // 8 bit mode
    delaymicros(100);
    lcdwrite(0x30, INSTR);                  // 8 bit mode
    lcdbusywait();
    lcdwrite(0x3C, INSTR);                  // set 2 lines, 5x8 font
    lcdbusywait();
    lcdwrite(0x08, INSTR);                  // turn off display
    lcdbusywait();
    lcdclear();
    lcdwrite(0x06, INSTR);                  // set entry mode to increment cursor
    lcdbusywait();
    lcdwrite(0x0C, INSTR);                  // turn display on with no cursor
    lcdbusywait();
}
