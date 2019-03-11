// LCD display
void initlcd(void);
char lcdprintstring(char *str);
void delaymicros(int micros);

// Helper functions
typedef enum { OUTPUT, INPUT } Pinmode;
typedef enum { LOW, HIGH } Signal;

// Game


// PORT             Bit     Pinout

// PORT F
#define BTN1        1

// PORT D

#define BLUE        0       // 3
#define GREEN       1       // 5
#define RED         2       // 6
#define YELLOW      9       // 7

#define RS          10      // 8
#define RW          3       // 9
#define E           4       // 10

// Change notification
#define CN_BLUE     14
#define CN_GREEN    15
#define CN_RED      16
#define CN_YELLOW   17
