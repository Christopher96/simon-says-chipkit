
#include <stddef.h> 
#include <stdint.h> 
#include <pic32mx.h>

#include "header.h"

void u32init(void);
void display_init(void);
void display_string(int line, char *s);
void display_update(void);

void print_binary(int bin);
void init(void);

int main() {

    init();

    initEEprom();

    resetHighscores();
    getHighscores();

    return 0;
    initBuzzer();
    initGame();
    
    initlcd();
    char* msg = "fuck you cunt";
    lcdprintstring(msg);
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


