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

#define RS          2       // 0
#define RW          3       // 1

// PORT D
#define E           8       // 2

#define BLUE        1       // 5
#define GREEN       2       // 6
#define RED         9       // 7
#define YELLOW      10      // 8

#define B_BLUE      5       // 34
#define B_GREEN     11      // 35
#define B_RED       6       // 36
#define B_YELLOW    7       // 37

// Change notification
#define CN_BLUE     14
#define CN_GREEN    15
#define CN_RED      16
#define CN_YELLOW   17
