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
    // make the int volatile to be able to read from it dynamically
    trise = (volatile int *) 0xbf886100;
    porte = (volatile int *) 0xbf886110;

    // Initialize port E bit 0-7 as output (set them to 0)
    *trise = *trise & 0xFF00;

    // Initialize port D bit 5-11 as input (switches and buttons)
    TRISD &= 0xFF00;

    return;
}

int btns = 0;
int switches = 0;
int count = 0;

/* This function is called repetitively from the main program */
void labwork( void )
{
    delay( 1000 );
    time2string( textstring, mytime );
    display_string( 3, textstring );
    display_update();
    tick( &mytime );
    display_image(96, icon);

    *porte = count;
    count++;

    btns = getbtns();
    switches = getswitches();

    if((btns >> 3) & 1) {
        // Change only 
        mytime &= (switches << 8) | ~0;
    } else if ((btns >> 2) & 1) {

    } else if (btns & 1) {

    }
}

