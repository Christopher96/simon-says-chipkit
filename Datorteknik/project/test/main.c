/* main.c

   This file written 2015 by F Lundevall and David Broman

   Latest update 2015-09-15 by David Broman

   For copyright and licensing, see file COPYING */

#include <stddef.h>   /* Declarations of integer sizes and the like, part 1 */
#include <stdint.h>   /* Declarations of integer sizes and the like, part 2 */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */

void u32init(void);
void display_init(void);
void display_string(int line, char *s);
void display_update(void);

void delay(int ms);
void print_binary(int bin);
void init(void);

int main() {
    init();

    char* text;
    int switches;

    TRISD = 0xFF00; // set RD[7:0] to output and RD[11:8] to input

    while(1) {
        switches = (PORTD >> 8) && 0xF;  // Read and mask switches from RD[11:8]
        PORTD = switches;                // display on the LEDs
        
        print_binary(PORTD);

        delay(10);
    }
    return 0;
}

void init() {
    u32init();
    display_init();
}

void delay(int ms) {
    int c = 4711;
    for(int i = 0; i < ms; i++)
        for(int j = c; j > 0; j--)
            ;
}

void print_binary(int bin) {
    int max = 32;
    char str[max];
    int i = 0;

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

    display_string(0, str);
    display_update();
}
