#include <pic32mx.h>  

#include "header.h"

#define TMR2PERIOD ((80000000 / 256) / 10)  // 80 MHz / 256 / 100 for 1/100 sec
#if TMR2PERIOD > 0xFFFF
#error "Timer period is to big"
#endif

void clearTimer(int timer) {
    int pin = (timer*4);
    IFSCLR(0) = pinMode(pin);
}

int readTimer(int timer) {
    int pin = (timer*4);
    return IFS(0) & pinMode(pin);
}

void resetTimer(int timer)  {
    switch(timer) {
        case 1:
            TMR1 = 0x0;                     // Clear contents of timer 1
            T1CON = 0x0;                    // Reset control register timer 1
            break;
        case 2:
            T2CON = 0x0;        
            TMR2 = 0x0;
            T2CON = 0x70;                   // 1:256 prescale
            PR2 = TMR2PERIOD;               // See constant
            IPCSET(2) = 0x0000000C;         // Set priority level = 3
            IPCSET(2) = 0x00000001;         // Set subpriority level = 1
            IECSET(0) = 0x100;              // Enable timer interrupts
            enable_interrupts();
            break;
    }
    clearTimer(timer); 
}

void startTimer(int timer) {
    int pin = 0x8000;

    switch(timer) {
        case 1:
            T1CONSET = pin;
            break;
        case 2:
            T2CONSET = pin;
            break; 

    }
}

void pauseTimer(int timer) {
    switch(timer) {
        case 1:
            T1CON = 0;
            break;
        case 2:
            T2CON = 0;
            break; 

    }
}

void user_isr( void ) {
    if(readTimer(2)) {
        clearTimer(2);
        fixedUpdate();
    }
}

void delaymicros(int micros) {
    if(micros > 1000) {                     // avoid timer overflow
        delaymicros(1000);
        delaymicros(micros-1000);
    } else if (micros > 6) {

        TMR1 = 0;                           // reset timer
        T1CON = 0xFFFF;
        PR1 = (micros-6)*100;                // 20 clocks per microsecond. Overhead ~6 us
        IFSCLR(0) = 0x10;
        /* clearTimer(1); */
        /* T1CONbits.ON = 1;           // turn timer on */
        /* resetTimer(1);                      // reset timer */
        /* startTimer(1);                      // start timer */
        /* IFS0bits.T1IF = 0;          // clear overflow flag */
        /* while(!IFS0bits.T1IF);           // wait until overflag is set */
        while(!(IFS(0) & 0x10));               // check if timer flag has been set
    }
}
