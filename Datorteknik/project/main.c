
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

    initlcd();
    char* msg = "floss swag";
    lcdprintstring(msg);
    return 0;
<<<<<<< HEAD

    int count = 0;
    while(1) {
        print(0, itoaconv(count));
        count++;
    }

    return 0;
    initGame();

    while(1) {
        printBinaryRow(0, IFS(1));
        printBinaryRow(1, IFS(0));
    }
    return 0;
=======
    initlcd();
    char* msg = "swag";
    lcdprintstring(msg);
    return 0;
>>>>>>> da7ab58dfbecf5d8faa8d668ad12f84311d93f92
}

void init() {
    u32init();
    display_init();
}


