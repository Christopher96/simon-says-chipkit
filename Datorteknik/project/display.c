#include "header.h"

#include <pic32mx.h>  

typedef enum { INSTR, DATA } Mode;



// PORT F
#define RS  2 // 0
#define RW  3 // 1

// PORT D
#define E   8 // 2

char lcdread(Mode md) {
    char c;

    /* TRISE = 0xFFFF;                 // set port E 0-7 as input */
    TRISE = pinModeFT(0, 7);                 // set pins 0-7 on E to input
    /* PORTCbits.RC14 = (md == DATA);  // set instruction to data mode */
    pinSetMode(&PORTF, RS, md == DATA);     // instruction or data mode?
    /* PORTCbits.RC13 = 1;             // read mode */
    PORTFSET = pinMode(RW);                 // read mode
    /* PORTCbits.RC15 = 1;             // pulse enable */
    PORTDSET = pinMode(E);                 // enable on
    delaymicros(10);                        // wait for response
    /* c = PORTE & 0x00FF;             // read a byte from port E */
    print_binary_row(0, PORTE);
    c = pinReadFT(PORTE, 0, 7);              // read byte from port E
    print(0, c);
    /* PORTCbits.RC15 = 0;             // turn off enable */
    PORTDCLR = pinMode(E);                  // enable off
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
    TRISECLR = pinModeFT(0, 7);              // set pins 0-7 on E to output
    /* PORTCbits.RC14 = (md == DATA);  // set instruction to data mode */
    pinSetMode(&PORTF, RS, md == DATA);     // instruction or data mode?
    /* PORTCbits.RC13 = 0;             // write mode */
    PORTFCLR = pinMode(RW);                 // write mode
    PORTE = val;                            // write a character
    /* PORTCbits.RC15 = 1;             // pulse enable */
    PORTDSET = pinMode(E);                  // enable on
    delaymicros(10);                        // wait for response
    /* PORTCbits.RC15 = 0;             // turn off enable */
    PORTDCLR = pinMode(E);                  // enable off
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
    TRISFCLR = pinModeFT(RS, RW);           // set RS and RW to output
    TRISDCLR = pinMode(E);                  // and E as well
    /* PORTC = 0x0000;                 // turn off all controls */
    PORTF = 0x0;                            // turn off controls
    PORTD = 0x0;

    print(3, "sending");
    // send instructions to initialize the display
    delaymicros(15000);
    lcdwrite(0x30, INSTR);                  // 8 bit mode
    delaymicros(4100);
    lcdwrite(0x30, INSTR);                  // 8 bit mode
    delaymicros(100);
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
