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

/* Interrupt Service Routine */
void user_isr( void )
{
    return;
}

volatile int * trise;
volatile int * porte;

/* Lab-specific initialization goes here */
void labinit( void )
{
    // Stop Timer 2 and reset control register
    T2CON = 0x0;
    
    // Clear the contents of Timer 2
    TMR2 = 0x0;

    // Set the bits to 111 for a 1:256 prescale value to fit the counter in a 16 bit value
    T2CON = 0x70;

    // The base clock rate is 80 MHz, we divide this by 256 (the prescale) to get a tick per second
    // We want a tick per 100 ms so we divide by 10
    PR2 = (80000000 / 256) / 10;

    // Clear timer interrupt status flag
    IFSCLR(0) = 0x00000100;

    // Start Timer 2
    T2CONSET = 0x8000;

    // Make the int volatile to be able to read from it dynamically
    trise = (volatile int *) 0xbf886100;
    porte = (volatile int *) 0xbf886110;

    // Initialize port E bit 0-7 as output (set them to 0)
    *trise = *trise & 0xFF00;

    // Initialize port D bit 5-11 as input (switches and buttons)
    TRISD &= 0x7FF0;

    return;
}

int mask = 0;
int btns = 0;
int switches = 0;
int count = 0;

void update_time(int byte, int val) {
    // Set mask to to the correct byte
    mask = (0xF << byte * 4);
    // Rotate the switch values to the correct place
    val <<= byte * 4;
    // Mask the byte that should be changed to all 0, then add the switch values to that specific byte
    mytime = (mytime & ~mask) | val;
}

void update() {
    *porte = count;
    count++;

    // Reset the interrupt flag
    IFSCLR(0) = 0x00000100;

    if(count % 10 != 0)
        return;
    
    time2string( textstring, mytime );
    display_string( 3, textstring );
    display_update();
    display_image(96, icon);
    tick( &mytime );
}

/* This function is called repetitively from the main program */
void labwork( void ) {

    btns = getbtns();
    switches = getsw();

    if((btns >> 2) & 1) {
        update_time(3, switches);
    } 

    if ((btns >> 1) & 1) {
        update_time(2, switches);
    }

    if (btns & 1) {
        update_time(1, switches);
    }

    if(IFS(0) & 0x00000100) {
        display_debug(IFS(0));
        update();
    }
}
