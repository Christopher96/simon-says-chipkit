#include "header.h"

#include <stdio.h>
#include <stdlib.h>
#include <pic32mx.h>  

const int leds[] = { YELLOW, RED, GREEN, BLUE };
const int n_leds = sizeof(leds) / sizeof(leds[0]);

char* current_led;

int led_pins;

int n_btns = 3;

typedef enum { EXIT, IDLE, PLAYER, PLAYER_DONE, SIMON, SIMON_DONE } State;
State state;

int game_level = 1;
int count = 0;

int simon_toggle = 0;
int simon_count = 0;
int simon_leds_size = 0;

#define MAX_LEDS 100
int simon_leds[MAX_LEDS];

void initGame() {
    // Set LEDs to output
    led_pins = pinModeAll(n_leds, YELLOW, RED, GREEN, BLUE);
    TRISDCLR = led_pins;

    // Set buttons to input
    TRISDSET = pinModeAll(n_btns, BTN2, BTN3, BTN4);

    switchState(IDLE);

    while(state != EXIT)
        update();
}

void newGame() {
    game_level = 1;
}

void getReady() {
    int countTo = 10;

    if(count >= countTo) {
        switchState(PLAYER);
    }
}

void playerThink() {
    int countTo = 5;

    if(count >= countTo) {
        count = 0;
    }
}

void simonThink() {
    int countTo = (simon_toggle) ? 5 : 1;

    if(count >= countTo) {
        count = 0;
        simon_toggle = (simon_toggle) ? 0 : 1;

        if(!simon_toggle) {
            PORTDCLR = led_pins;
            return;
        }

        if(simon_count < simon_leds_size) {
            simonSays();
            return;
        }

        simonPick();
    }
}

char* ledName(int pin) {
    switch(pin) {
        case YELLOW: return "yellow";
        case RED: return "red";
        case GREEN: return "green";
        case BLUE: return "blue";
    }
}

void simonSays() {
    int pin = simon_leds[simon_count];
    PORTDSET = pinMode(pin);
    current_led = ledName(pin);
    simon_count++;
}

void simonPick() {
    int r = rand() % (n_leds);
    int pin = leds[r];
    current_led = ledName(pin);
    PORTDSET = pinMode(pin);

    simon_leds[simon_leds_size++] = pin;

    switchState(SIMON_DONE);
}

void switchState(State new_state) {
    count = 0;
    switch(new_state) {
        case SIMON:
            simon_count = 0;
            simon_toggle = 0;
            break;
    }
    state = new_state;
}

void update() {
    if(pinRead(PORTD, BTN4)) {
        switchState(SIMON);
    } else if(pinRead(PORTD, BTN3)) {
        switchState(IDLE);
    }

    char* state_txt;
    switch(state) {
        case IDLE:
            PORTDCLR = led_pins;
            state_txt = "main menu";
            break;
        case SIMON:
            state_txt = "simon says...";
            print(1, current_led);
            break;
        case SIMON_DONE:
            state_txt = "get ready";
            print(1, itoaconv(count));
            break;
        case PLAYER:
            state_txt = "player says...";
            break;
    }

    print(0, state_txt);
}


void fixedUpdate() {
    count++;

    switch(state) {
        case SIMON:
            simonThink();
            break;
        case PLAYER:
            playerThink();
            break;
        case SIMON_DONE:
            getReady();
            break;
    }
}

