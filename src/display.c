#include "header.h"

#include <pic32mx.h>  

typedef enum { INSTR, DATA } Mode;

char lcdread(Mode md) {
    char c;

    TRISESET = pinModeFT(0, 7);                 // set pins 0-7 on E to input
    pinSetMode(&PORTD, RS, md == DATA);         // instruction or data mode?
    PORTDSET = pinMode(RW);                     // read mode
    PORTDSET = pinMode(E);                      // enable on
    delaymicros(10);                            // wait for response
    c = pinReadFT(PORTE, 0, 7);                 // read byte from port E
    PORTDCLR = pinMode(E);                      // enable off
    delaymicros(10);                            // wait for response
}

void lcdbusywait(void) {
    char state;
    do {
        state = lcdread(INSTR);                 // read instruction
    } while(state & 0x80);                      // repeat until busy flag is clear
}

char lcdwrite(char val, Mode md) {
    TRISECLR = pinModeFT(0, 7);                 // set pins 0-7 on E to output
    pinSetMode(&PORTD, RS, md == DATA);         // instruction or data mode?
    PORTDCLR = pinMode(RW);                     // write mode
    PORTE = val;                                // write a character
    PORTDSET = pinMode(E);                      // enable on
    delaymicros(1000);                            // wait for response
    PORTDCLR = pinMode(E);                      // enable off
    delaymicros(1000);                            // wait for response
}

void lcdprintchar(char c) {
    lcdwrite(c, DATA);
    lcdbusywait();
}

void lcdprintarray(char str[]) {
    int size = sizeof(str) / sizeof(str[0]);
    for(int i = 0; i < size; i++) {
        lcdwrite(str[i], DATA);                   
        lcdbusywait();
    }
}

void lcdprintstring(char *str) {
    while(*str != 0) {                          // iterate until null character is found
        lcdwrite(*str, DATA);                   // print character
        lcdbusywait();
        str++;                                  // next character
    }
}

void printSpaces() {
    for (int i = 0; i < 16; i++) {
        lcdprintchar(' ');
    }
}

void lcdprintstop(int row, char* str) {
    lcdhome();
    if(row == 1) lcdnext();
    lcdprintstring(str);
}

void lcdprint(int row, char* str) {
    lcdprintstop(row, str);
    printSpaces();
}

void lcdclear(void) {
    lcdwrite(0x01, INSTR);                     
    delaymicros(1000);
}

void lcdinstr(char val) {
    lcdwrite(val, INSTR);
    lcdbusywait();
}

void lcdnext(void) {
    lcdinstr(0xC0);
}

void lcdhome(void) {
    lcdinstr(0x02);
}

void lcdcursorleft(void) {
    lcdinstr(0x10);
}

void lcdshowcursor(void) {
    lcdinstr(0x0E);
}

void lcdhidecursor(void) {
    lcdinstr(0x0C);
}

void initlcd(void) {
    // set LCD control pins
    TRISDCLR = pinModeAll(3, RS, RW, E);        // set controls to output
    PORTDCLR = pinModeAll(3, RS, RW, E);        // turn off controls

    print(3, "sending");
    delaymicros(45000);
    lcdwrite(0x30, INSTR);                      // 8 bit mode
    delaymicros(4100);
    lcdwrite(0x30, INSTR);                      // 8 bit mode
    delaymicros(100);
    lcdwrite(0x30, INSTR);                      // 8 bit mode
    print(3, "waiting");
    lcdbusywait();
    print(3, "sending");
    lcdwrite(0x3C, INSTR);                      // set 2 lines, 5x8 font
    print(3, "waiting");
    lcdbusywait();
    print(3, "display off");
    lcdwrite(0x08, INSTR);                      // turn off display
    lcdbusywait();
    print(3, "clearing screen");
    lcdclear();
    print(3, "setting entry mode");
    lcdwrite(0x06, INSTR);                      // set entry mode to increment cursor
    lcdbusywait();
    print(3, "display on");
    lcdwrite(0x0C, INSTR);                      // turn display on with no cursor
    lcdbusywait();
    print(4, "done");

}
