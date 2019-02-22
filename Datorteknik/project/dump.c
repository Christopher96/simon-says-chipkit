
void delaymicros(int micros) {
    if(micros > 1000) {             // avoid timer overflow
        delaymicros(1000);
        delaymicros(micros-1000);
    } else if (micros > 6) {
        TMR1 = 0;                   // reset timer to 0
        T1CONbits.ON = 1;           // turn timer on
        PR1 = (micros-6)*20;        // 20 clocks per microsecond. Overhead ~6 us
        IFS0bits.T1IF = 0;          // clear overflow flag
        while(!IFS0bits.T1IF);      // wait until overflag is set
    }
}

