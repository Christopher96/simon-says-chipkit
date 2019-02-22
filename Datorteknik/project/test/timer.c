#include <pic32mx.h>  

#include "header.h"

void clearTimer(int timer) {
    IFSCLR(0) = pinMode(timer);
}

int readTimer(int timer) {
    return pinRead(IFS(0), timer);
}

void resetTimer(int timer)  {
    switch(timer) {
        case 1:
            T1CON = 0x0;        // Reset control register timer 1
            TMR1 = 0x0;         // Clear contents of timer 1
        break;
        case 2:
            T2CON = 0x0;        
            TMR2 = 0x0;
        break;
    }
}

void startTimer(int timer) {
    int pin = pinMode(13);

    switch(timer) {
        case 1:
            T1CONSET = pin;
        break;
        case 2:
            T2CONSET = pin;
        break; 

    }
}

void delaymicros(int micros) {
    if(micros > 1000) {                     // avoid timer overflow
        delaymicros(1000);
        delaymicros(micros-1000);
    } else if (micros > 6) {
        /* TMR1 = 0;                           // reset timer */
        /* T1CONbits.ON = 1;           // turn timer on */
        resetTimer(1);                      // reset timer
        clearTimer(1);                      // clear timer flag
        PR1 = (micros-6)*20;                // 20 clocks per microsecond. Overhead ~6 us
        startTimer(1);                      // start timer
        /* IFS0bits.T1IF = 0;          // clear overflow flag */
        /* while(!IFS0bits.T1IF);           // wait until overflag is set */
        while(!readTimer(1));               // check if timer flag has been set
    }
}
