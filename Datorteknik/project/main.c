
#include <stddef.h> 
#include <stdint.h> 
#include <pic32mx.h>

#include "header.h"

int main() {

    init();
    initlcd();
    initBuzzer();
    initEEprom();
    initGame();
    return 0;
    getHighscores();
    return 0;

    return 0;
    
    while(1) {
        printBinaryRow(0, IFS(1));
        printBinaryRow(1, IFS(0));
    }
    return 0;
}

void init() {
    u32init();
    display_init();
}


