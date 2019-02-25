#include "header.h"

#include <pic32mx.h>  

int level = 1;
int round = 1;

enum { EXIT, IDLE, PLAYER, SIMON } state;

void initGame() {
    while(state != EXIT)
        update();
}

void newGame() {
    level = 1;
    round = 1;

}

void update() {

} 
