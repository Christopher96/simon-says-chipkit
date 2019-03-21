#include "header.h"

#include <stdio.h>
#include <stdlib.h>
#include <pic32mx.h>  

const int leds[] = { BLUE, GREEN, RED, YELLOW };
const int n_leds = sizeof(leds) / sizeof(leds[0]);

char* current_led;

int led_pins;

typedef enum { EXIT, MAIN_MENU, PLAYER, LEVEL_UP, GAME_OVER, SIMON, READY, NEW_HIGHSCORE, NEW_GAME } State;
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
    resetTimer(2);
    startTimer(2);

    int r = rand();
    srand(r);

    // Create LED pin mask
    led_pins = pinModeAll(n_leds, BLUE, GREEN, RED, YELLOW);

    // Set BTN1 to input
    TRISFSET = pinMode(BTN1);

    switchState(MAIN_MENU);

    TRISFSET = pinMode(BTN1);

    while(state != EXIT)
        update();
}

void mainMenu() {

}
void newGame() {

}

char* ledName(int pin) {
    switch(pin) {
        case YELLOW: return "yellow";
        case RED: return "red";
        case GREEN: return "green";
        case BLUE: return "blue";
    }
}

int ready_count = 5;

void getReady() {
    if(count >= ready_count) {
        switchState(PLAYER);
    }
}

void playerThink() {
    int count_to = 50;

    if(count >= count_to) {
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
    int count_to = (simon_toggle) ? 5 : 1;

    if(count >= count_to) {
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

int score_index = -1;
int check_score = 0;

void gameOver() {
    if(check_score == 0) {
        score_index = checkHighscore(game_level);
        check_score = 1;
    } else if(score_index != -1){
        switchState(NEW_HIGHSCORE);
    } else {
        switchState(NEW_GAME);
    }
}

char name[4];
int new_char = 0;
int name_index = 0;
char selected_char = 'A';

void newHighscore() {
    if(name_index < 4) {
        lcdprintchar(selected_char);
        if(!new_char) {
            lcdcursorleft();
        } else {
            new_char = 0;
            selected_char = 'A';
        }
    } else {
        saveHighscore(score_index, game_level, name);
        switchState(NEW_GAME);
    }
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
    switch(state) {
        case PLAYER:
        case NEW_HIGHSCORE:
            // Change LED pins to input
            TRISDSET = led_pins;
            break;
        default:
            // Set LED pins to output
            TRISDCLR = led_pins;
            break;
    }

    switch(state) {
        case NEW_GAME:
            score_index = 0;
            check_score = 0;
            game_level = 1;
            break;
        case SIMON:
            simon_count = 0;
            simon_toggle = 0;
            simon_picked = 0;
            break;
    }

    turnOffAll();
    count = 0;

    state = new_state;
}

int buttonPress(int color) {
    if(pinRead(PORTD, color) && pressed == -1) {
        pressed = 1;
        return 1;
    }
    turnOffAll();
    return 0;
}


char* state_txt;
char* state_info = "";

void update() {
    if(pressed == -1) {
        if(pinRead(PORTF, BTN1)) {
            switchState(state_toggle ? SIMON : MAIN_MENU);
            state_toggle = (state_toggle) ? 0 : 1;
            pressed = 1;
        }
    }
    switch(state) {
        case MAIN_MENU:
            turnOffAll();
            state_txt = "Main menu";
            mainMenu();
            break;
        case SIMON:
            state_txt = "Simon says...";
            state_info = current_led;
            break;
        case READY:
            state_txt = "Get ready!";
            state_info = itoaconv(ready_count-count);
            break;
        case PLAYER:
            state_txt = "Player says...";
            state_info = current_led;

            if(buttonPress(BLUE)) {
                playerPick(BLUE);
            } else if(buttonPress(GREEN)){
                playerPick(GREEN);
            } else if(buttonPress(RED)) {
                playerPick(RED);
            } else if(buttonPress(YELLOW)) {
                playerPick(YELLOW);
            } else {
                current_led = "";
            }

            break;
        case LEVEL_UP:
            state_txt = "You leveled up!";
            state_info = "Level: " + itoaconv(game_level);
            break;
        case GAME_OVER:
            state_txt = "Game over!";
            gameOver();
            break;
        case NEW_HIGHSCORE:
            
            if(buttonPress(YELLOW)) {
                selected_char++;
            } else if(buttonPress(RED)) {
                selected_char--;
            } else if(buttonPress(GREEN)) {
                name[name_index] = selected_char;
                name_index++;
                new_char = 1;
            } else if(buttonPress(BLUE)) {
                selected_char = 'A';
            } 

            if(selected_char > 'Z') {
                selected_char = 'A';
            } else if(selected_char < 'A') {
                selected_char = 'Z';
            }

            newHighscore();
            break;
        case NEW_GAME:
            newGame();
            break;
    }

    print(0, state_txt);
    print(1, state_info);
    print(2, itoaconv(count));
}


void printSpaces() {
    for (int i = 0; i < 16; i++) {
        lcdprintchar(' ');
    }
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
    /*  */
    /* lcdhome(); */
    /* lcdprintstring(state_txt); */
    /* printSpaces(); */
    /* lcdnext(); */
    /* lcdprintstring(state_info); */
    /* printSpaces(); */
    /*  */
    if(pressed > -1)
        pressed--;
}

