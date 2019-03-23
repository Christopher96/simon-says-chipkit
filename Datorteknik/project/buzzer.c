#include "header.h"

#include <pic32mx.h>  

void playBuzzer() {
    OC4R = 2048;                                // Load initial value into duty cycle register
    OC4RS = 2048;                               // When a period finishes the contents of OC1RS is loaded to OC1R;
    OC4CONSET = pinMode(15);
}

void pauseBuzzer() {
    OC4R = 0;                                
    OC4RS = 0;                              
    OC4CONCLR = pinMode(15);
}

void playTone(int freq, int duration) {
    resetTimer(3);
    PR3 = freq;
    startTimer(3);
    playBuzzer();
    delay(duration);
    pauseBuzzer();
}

void initBuzzer() {
    OC4CON = 0xE;                                       // Set Timer3 as source | enable pwm mode without fault protection
}
