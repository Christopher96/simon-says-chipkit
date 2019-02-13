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

/* int getbtns(void); */
/* int getsw(void); */

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
    TRISD &= 0x7FF0;

    return;
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

/* This function is called repetitively from the main program */
void labwork( void )
{
    delay( 1000 );

    *porte = count;
    count++;

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

    time2string( textstring, mytime );
    display_string( 3, textstring );
    display_update();
    tick( &mytime );
    display_image(96, icon);
}
