/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int mytime = 0x5957;

char textstring[] = "text, more text, and even more text!";

int count = 0;

/* Interrupt Service Routine */
void user_isr( void )
{
    // Check for timer flag
    if(IFS(0) & 0x00000100) {
        time2string(textstring, mytime);
        display_string(3, textstring);
        display_update();
        tick(&mytime);

        // Reset the interrupt flag
        IFSCLR(0) = 0x00000100;
    }

    if(IFS(0) & 0x80) {
        count++;
        PORTESET = count;

        IFSCLR(0) = 0x80;
    }

}

int prime = 1234567;

/* Lab-specific initialization goes here */
void labinit( void )
{
    /* Switch 1 - SW1 */

    // Set switch interrupt priority
    IPC(1) = (0x1F << 24);

    // Enable interrupt INT1IE bit 7
    IECSET(0) = 0x80;

    // Stop Timer 2 and reset control register
    T2CON = 0x0;
    
    // Clear the contents of Timer 2
    TMR2 = 0x0;

    // Set the bits to 111 for a 1:256 prescale value to fit the counter in a 16 bit value
    T2CON = 0x70;

    // The base clock rate is 80 MHz, we divide this by 256 (the prescale) to get a tick per second
    // We want a tick per 100 ms so we divide by 10
    PR2 = (80000000 / 256) / 10;

    IPCSET(2) = 0x0000000C; // Set priority level = 3
    IPCSET(2) = 0x00000001; // Set subpriority level = 1

    IEC(0) = 0x100;

    // Clear timer interrupt status flag
    IFSCLR(0) = 0x00000100;

    // Start Timer 2
    T2CONSET = 0x8000;

    // Enable timer interrupts
    enable_interrupt();

    // Set switches and buttons as inputs
    TRISDSET = 0x7FF0;

    // Set leds to outputs
    TRISESET = 0xFF00;

    // Reset leds
    PORTE = 0x0;
}


/* This function is called repetitively from the main program */
void labwork( void ) {
    prime = nextprime(prime);
    display_string(0, itoaconv(prime));
    display_update();
}
