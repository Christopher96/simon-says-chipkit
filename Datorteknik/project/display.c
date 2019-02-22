#include "header.h"

#include <stddef.h>   /* Declarations of integer sizes and the like, part 1 */
#include <stdint.h>   /* Declarations of integer sizes and the like, part 2 */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */

typedef enum { INSTR, DATA } Mode;

#define RS  14   // a5
#define RW  12   // a4
#define E   10   // a3 

char lcdread(mode md) {
    char c;

    /* TRISE = 0xFFFF;                 // set port E 0-7 as input */
    TRISE = pinsMode(0, 7);                 
    /* PORTCbits.RC14 = (md == DATA);  // set instruction to data mode */
    pinSetMode(&PORTB, RS, md == DATA);
    /* PORTCbits.RC13 = 1;             // read mode */
    PORTBSET = pinMode(RW);
    /* PORTCbits.RC15 = 1;             // pulse enable */
    PORTBSET = pinMode(RW);
    delaymicros(10);                // wait for LCD to respond
    /* c = PORTE & 0x00FF;             // read a byte from port E */
    c = pinsRead(PORTB, 0, 7);             
    /* PORTCbits.RC15 = 0;             // turn off enable */
    PORTBCLR = pinMode(E);
    delaymicros(10);                // wait for LCD to respond
}

void lcdbusywait(void) {
    char state;
    do {
        state = lcdread(INSTR);     // read instruction
    } while(state & 0x80);          // repeat until busy flag is clear
}

char lcdwrite(char val, mode md) {
    /* TRISE = 0xFF00;                 // set port E 0-7 as output */
    TRISECLR = pinsMode(0, 7);
    /* PORTCbits.RC14 = (md == DATA);  // set instruction to data mode */
    pinSetMode(&PORTB, RS, md == DATA);
    /* PORTCbits.RC13 = 0;             // write mode */
    PORTBCLR = pinMode(RW);
    PORTE = val;                    // value to write
    /* PORTCbits.RC15 = 1;             // pulse enable */
    PORTBSET = pinMode(E);
    delaymicros(10);                // wait for LCD to respond
    /* PORTCbits.RC15 = 0;             // turn off enable */
    PORTBCLR = pinMode(E);
    delaymicros(10);                // wait for LCD to respond
}

char lcdprintstring(char *str) {
    while(*str != 0) {              // loop until null terminator
        lcdwrite(*str.DATA);        // print this character
        lcdbusywait();
        str++;                      // advance pointer to next char
    }
}

void lcdclear(void) {
    lcdwrite(0x01, INSTR);          // clear display
    delaymicros(1530);              // wait for execution
}

void initlcd(void) {
    // set LCD control pins
    /* TRISC = 0x1FFF;                 // set port C 13-15 to output and other to input */
    TRISC = pinMode(0, 12);
    /* PORTC = 0x0000;                 // turn off all controls */
    PORTC = 0;

    // send instructions to initialize the display
    delaymicros(15000);
    lcdwrite(0x30, INSTR);          // 8 bit mode
    delaymicros(4100);
    lcdwrite(0x30, INSTR);          // 8 bit mode
    delaymicros(100);
    lcdwrite(0x30, INSTR);          // 8 bit mode
    lcdbusywait();
    lcdwrite(0x3C, INSTR);          // set 2 lines, 5x8 font
    lcdbusywait();
    lcdwrite(0x08, INSTR);          // turn off display
    lcdbusywait();
    lcdclear();
    lcdwrite(0x06, INSTR);          // set entry mode to increment cursor
    lcdbusywait();
    lcdwrite(0x0C, INSTR);          // turn display on with no cursor
    lcdbusywait();
}
