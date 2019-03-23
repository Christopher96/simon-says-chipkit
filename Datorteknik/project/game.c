#include "header.h"

#include <stdio.h>
#include <stdlib.h>
#include <pic32mx.h>  

const int leds[] = { YELLOW, BLUE, RED, GREEN };
const int n_leds = sizeof(leds) / sizeof(leds[0]);

int led_pins;

typedef enum { EXIT, MAIN_MENU, PLAYER, LEVEL_UP, GAME_OVER, SIMON, READY, NEW_HIGHSCORE, HIGHSCORES, RESET} State;
State state;

int current_score = 0;
int count = 0;

int simon_toggle = 0;
int simon_picked = 0;
int simon_count = 0;
int simon_leds_size = 0;

#define MAX_LEDS 100
int simon_leds[MAX_LEDS];

int pressed = -1;

void initGame() {
    resetTimer(2);
    startTimer(2);

    // Create LED pin mask
    led_pins = pinModeAll(n_leds, BLUE, GREEN, RED, YELLOW);

    // Set BTN1 to input
    TRISFSET = pinMode(BTN1);

    switchState(MAIN_MENU);

    TRISFSET = pinMode(BTN1);

    while(state != EXIT)
        update();
}

void pickLed(int pin) {
    char* led_name;
    switch(pin) {
        case YELLOW:
            led_name = "YELLOW";
            playTone(10000, 200);
            break;
        case RED: 
            led_name = "RED";
            playTone(20000, 200);
            break;
        case GREEN:
            led_name = "GREEN";
            playTone(30000, 200);
            break;
        case BLUE: 
            led_name = "BLUE";
            playTone(40000, 200);
            break;
    }
}


void playerThink() {
    int count_to = 50;

    if(count >= count_to) {
        switchState(GAME_OVER);
    }
}

int player_count = 0;

void playerPick(int pin) {
    pickLed(pin);
    if(pin == simon_leds[player_count]) {
        count = 0;
        if(player_count == simon_leds_size-1) {
            switchState(LEVEL_UP);
        } else {
            player_count++;
        }
    } else {
        switchState(GAME_OVER);
    }
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
    pickLed(pin);
    simon_count++;
}

void simonPick() {
    int r = rand() % (n_leds);
    int pin = leds[r];
    turnOn(pin);
    pickLed(pin);

    simon_leds[simon_leds_size] = pin;
    simon_leds_size++;
    simon_picked = 1;
}

char highscores[HIGHSCORES_MAX];
int highscore_num = 0;

void orderHighscores() {
    for (int i = 0; i < HIGHSCORES_MAX; i+= HIGHSCORE_SIZE) {
        char highscore[HIGHSCORE_SIZE];

        for(int k = 0; k < HIGHSCORE_SIZE; k++) {
            highscore[k] = highscores[i+k]; 
        }

        int highest_index = i;
        int highest_score = highscore[4];

        for(int j = i+HIGHSCORE_SIZE; j < HIGHSCORES_MAX; j+= HIGHSCORE_SIZE) {
            if(highest_score < highscores[j+4]) {
                highest_index = j;
                highest_score = highscores[j+4];
            }
        }

        for(int k = 0; k < HIGHSCORE_SIZE; k++) {
            highscores[i+k] = highscores[highest_index+k];
        }
        for(int k = 0; k < HIGHSCORE_SIZE; k++) {
            highscores[highest_index+k] = highscore[k];
        }
    }
}

void printHighscore() {
    int i = highscore_num * HIGHSCORE_SIZE;

    if(highscores[i] > 0) {
        char name[4];
        for(int j = 0; j < 4; j++) {
            name[j] = highscores[i+j]; 
        }
        lcdclear();
        lcdprintstring(itoaconv(highscore_num + 1));
        lcdprintstring(". ");
        lcdprintarray(name);
        lcdprintchar(' ');
        lcdprintstring(itoaconv(highscores[i+4]));
    } else {
        switchState(MAIN_MENU);
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
        saveHighscore(current_score, name);
        switchState(MAIN_MENU);
    }
}

void turnOn(int pin) {
    PORTDSET = pinMode(pin);
}

void turnOffAll() {
    PORTDCLR = led_pins;
}

void newGame() {
    srand(rand());

    simon_leds_size = 0;
    for(int i = 0; i < MAX_LEDS; i++) {
        simon_leds[i] = 0;
    }
    current_score = 0;

    switchState(SIMON);
}

void switchState(State new_state) {
    char* state_txt = "";
    char* state_info = "";

    switch(new_state) {
        case PLAYER:
        case HIGHSCORES:
        case NEW_HIGHSCORE:
        case MAIN_MENU:
            // Change LED pins to input
            TRISDSET = led_pins;
            break;
        default:
            // Set LED pins to output
            TRISDCLR = led_pins;
            break;
    }

    switch(new_state) {
        case RESET: 
            lcdprint(0, "Reset highscores?");
            lcdprint(1, "(R) No (G) Yes");
            break;
        case MAIN_MENU:
            lcdhidecursor();
            lcdprint(0, "Main menu");
            lcdprint(1, "");
            break;
        case READY:
            lcdprint(0, "Get ready...");
            lcdprint(1, "");
            break;
        case PLAYER:
            player_count = 0;
            lcdprint(0, "Player says...");
            lcdprint(1, "");
            break;
        case LEVEL_UP:
            current_score++;
            lcdprint(0, "Correct!");
            lcdprintstop(1, "Score: ");
            lcdprintstring(itoaconv(current_score));
            break;
        case GAME_OVER:
            lcdprint(0, "Game over!");
            lcdprintstop(1, "Score: ");
            lcdprintstring(itoaconv(current_score));
            break;
        case SIMON:
            simon_count = 0;
            simon_toggle = 0;
            simon_picked = 0;
            lcdprint(0, "Simon says...");
            lcdprint(1, "");
            break;
        case HIGHSCORES:
            highscore_num = 0;
            readEEpromBytes(HIGHSCORES_ADDR, highscores, HIGHSCORES_MAX);
            orderHighscores();
            printHighscore();
            break;
        case NEW_HIGHSCORE:
            name_index = 0;
            new_char = 0;
            for (int i = 0; i < 4; ++i) {
                name[i] = 0;
            }
            selected_char = 'A';

            lcdshowcursor();
            lcdprint(0, "New highscore!");
            lcdprint(1, "");
            lcdprintstop(1, "");
            break;
    }

    print(0, state_txt);
    print(1, state_info);
    print(2, itoaconv(count));

    turnOffAll();
    count = 0;

    state = new_state;

}

int buttonPress(int color) {
    if(pinRead(PORTD, color) && pressed == -1) {
        pressed = 1;
        return 1;
    }
    return 0;
}

void update() {
    int print_char = 1;
    int print_highscore = 1;

    switch(state) {
        case RESET:
            if(buttonPress(RED)) {
                switchState(MAIN_MENU);
            } else if(buttonPress(GREEN)) {
                resetHighscores();
                switchState(MAIN_MENU);
            }
            break;
        case MAIN_MENU:
            if(buttonPress(GREEN)) {
                newGame();
            } else if(buttonPress(RED)) {
                switchState(HIGHSCORES);
            } else if(buttonPress(BLUE)) {
                switchState(RESET);
            }
            break;
        case PLAYER:
            if(buttonPress(BLUE)) {
                playerPick(BLUE);
            } else if(buttonPress(GREEN)){
                playerPick(GREEN);
            } else if(buttonPress(RED)) {
                playerPick(RED);
            } else if(buttonPress(YELLOW)) {
                playerPick(YELLOW);
            } 

            break;
        case GAME_OVER:
            if(buttonPress(GREEN)) {
                switchState(NEW_HIGHSCORE);
            }
            break;
        case HIGHSCORES:
            if(buttonPress(YELLOW)) {
                highscore_num--;
            } else if(buttonPress(BLUE)) {
                highscore_num++;
            } else if(buttonPress(RED)) {
                switchState(MAIN_MENU);
                print_highscore = 0;
            } else {
                print_highscore = 0;
            }

            if(print_highscore) {
                printHighscore();
            }
            break;
        case NEW_HIGHSCORE:

            if(buttonPress(YELLOW)) {
                selected_char--;
            } else if(buttonPress(BLUE)) {
                selected_char++;
            } else if(buttonPress(RED) && name_index > 0) {
                lcdprintchar(' ');
                lcdcursorleft();
                lcdcursorleft();
                name_index--;
            } else if(buttonPress(GREEN)) {
                name[name_index] = selected_char;
                name_index++;
                new_char = 1;
            } else {
                print_char = 0;
            }


            if(print_char) {
                if(selected_char > 'Z') {
                    selected_char = 'A';
                } else if(selected_char < 'A') {
                    selected_char = 'Z';
                }
                newHighscore();
            }
            break;
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
            if(count >= 5) 
                switchState(PLAYER);
            break;
        case LEVEL_UP:
            if(count >= 5)
                switchState(SIMON);
            break;
    }

    if(pressed > -1)
        pressed--;
}

