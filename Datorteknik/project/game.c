#include "header.h"

#include <stdio.h>
#include <stdlib.h>
#include <pic32mx.h>  

const int leds[] = { BLUE, GREEN, RED, YELLOW };
const int n_leds = sizeof(leds) / sizeof(leds[0]);

char* current_led;

int led_pins;

typedef enum { EXIT, IDLE, PLAYER, LEVEL_UP, GAME_OVER, SIMON, READY } State;
State state;

int game_level = 1;
int count = 0;

int simon_toggle = 0;
int simon_picked = 0;
int simon_count = 0;
int simon_leds_size = 0;

#define MAX_LEDS 100
int simon_leds[MAX_LEDS];

int state_toggle = 0;
int pressed = 0;

void initGame() {
    int r = rand();
    srand(r);

    // Create LED pin mask
    led_pins = pinModeAll(n_leds, BLUE, GREEN, RED, YELLOW);

    // Set BTN1 to input
    TRISFSET = pinMode(BTN1);

    switchState(IDLE);

    while(state != EXIT)
        update();
}

void newGame() {
    game_level = 1;
}

char* ledName(int pin) {
    switch(pin) {
        case YELLOW: return "yellow";
        case RED: return "red";
        case GREEN: return "green";
        case BLUE: return "blue";
    }
}

int readyCount = 5;

void getReady() {
    if(count >= readyCount) {
        switchState(PLAYER);
    }
}

void playerThink() {
    int countTo = 50;

    if(count >= countTo) {
        switchState(GAME_OVER);
    }
}

char* simon_led;
void playerPick(int pin) {
    current_led = ledName(pin);
    simon_led = ledName(simon_leds[simon_leds_size]);
    if(pin == simon_leds[simon_count]) {
        count = 0;
        if(simon_count == simon_leds_size-1) {
            switchState(LEVEL_UP);
        } else {
            simon_count++;
        }
    } else {
        switchState(GAME_OVER);
    }
    pressed = 1;
}

void simonThink() {
    int countTo = (simon_toggle) ? 5 : 1;

    if(count >= countTo) {
        count = 0;
        simon_toggle = (simon_toggle) ? 0 : 1;

        if(!simon_toggle) {
            turnOffAll();
            return;
        }

        if(simon_picked) {
            switchState(READY);
            return;
        }

        if(simon_count < simon_leds_size) {
            simonSays();
            return;
        }

        if(!simon_picked) {
            simonPick();
            return;
        }
    }
}
void simonSays() {
    int pin = simon_leds[simon_count];
    turnOn(pin);
    simon_count++;
}

void simonPick() {
    int r = rand() % (n_leds);
    int pin = leds[r];
    turnOn(pin);

    simon_leds[simon_leds_size] = pin;
    simon_leds_size++;
    simon_picked = 1;
}

void turnOn(int pin) {
    PORTDSET = pinMode(pin);
    current_led = ledName(pin);
}

void turnOffAll() {
    PORTDCLR = led_pins;
    current_led = "";
}

void switchState(State new_state) {
    if(state != PLAYER)
        // Set LED pins to output
        TRISDCLR = led_pins;
    else {
        // Change LED pins to input if players turn
        TRISDSET = led_pins;
    }

    turnOffAll();
    count = 0;

    simon_count = 0;
    simon_toggle = 0;
    simon_picked = 0;

    state = new_state;
}

void update() {
    if(pressed == -1) {
        if(pinRead(PORTF, BTN1)) {
            switchState(state_toggle ? SIMON : IDLE);
            state_toggle = (state_toggle) ? 0 : 1;
            pressed = 1;
        }
    }

    char* state_txt;
    char* state_info = "";

    switch(state) {
        case IDLE:
            turnOffAll();
            state_txt = "main menu";
            break;
        case SIMON:
            state_txt = "simon says...";
            state_info = current_led;
            break;
        case READY:
            state_txt = "get ready!";
            state_info = itoaconv(readyCount-count);
            break;
        case PLAYER:
            state_txt = "player says...";
            state_info = current_led;

            if(pressed > -1) break;
            
            if(pinRead(PORTD, BLUE)) {
                playerPick(BLUE);
            } else if(pinRead(PORTD, GREEN)){
                playerPick(GREEN);
            } else if(pinRead(PORTD, RED)) {
                playerPick(RED);
            } else if(pinRead(PORTD, YELLOW)) {
                playerPick(YELLOW);
            } else {
                current_led = "";
            }

            break;
        case LEVEL_UP:
            state_txt = "you leveled up!";
            state_info = "level: " + itoaconv(game_level);
            break;
        case GAME_OVER:
            state_txt = "game over!";
            break;
    }

    print(0, state_txt);
    print(1, state_info);
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
        case READY:
            getReady();
            break;
        case LEVEL_UP:
            if(count >= 5)
                switchState(SIMON);
            break;
    }

    if(pressed > -1)
        pressed--;
}

