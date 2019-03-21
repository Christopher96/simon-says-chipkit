#include "header.h"

#include <pic32mx.h>  

void playBuzzer() {
    OC4R = 4096;                                // Load initial value into duty cycle register
    OC4RS = 4096;                               // When a period finishes the contents of OC1RS is loaded to OC1R;
    OC4CONSET = pinMode(15);
}

void pauseBuzzer() {
    OC4R = 0;                                
    OC4RS = 0;                              
    OC4CONCLR = pinMode(15);
}

char notes[] = "ccggaagffeeddc "; // a space represents a rest
/* char notes[] = "deeeddddffffeeedeeeddddffffeeedeee"; // a space represents a rest */
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300;
int length = sizeof(notes) / sizeof(notes[0]); // the number of notes

void playTone(int tone, int duration) {
    for (long i = 0; i < duration * 1000L; i += tone * 2) {
        playBuzzer();
        delaymicros(tone);
        pauseBuzzer();
        delaymicros(tone);
    }
}

void playNote(char note, int duration) {
    char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
    int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };

    // play the tone corresponding to the note name
    for (int i = 0; i < 8; i++) {
        if (names[i] == note) {
            playTone(tones[i], duration);
        }
    }
}

void loop() {
    for (int i = 0; i < length; i++) {
        if (notes[i] == ' ') {
            delay(beats[i] * tempo); // rest
        } else {
            playNote(notes[i], beats[i] * tempo);
        }

        // pause between notes
        delay(tempo / 2); 
    }
}
void initBuzzer() {
    resetTimer(3);
    OC4CON = 0xE;                                       // Set Timer3 as source | enable pwm mode without fault protection
    startTimer(3);
    while(1) {
        loop();
    }
}
