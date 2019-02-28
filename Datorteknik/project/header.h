// LCD display
void initlcd(void);
char lcdprintstring(char *str);
void delaymicros(int micros);

// Helper functions
typedef enum { OUTPUT, INPUT } Pinmode;
typedef enum { LOW, HIGH } Signal;

// Game


// PORT         Bit     Pinout

// PORT F
#define RS      2       // 0
#define RW      3       // 1

// PORT D
#define E       8       // 2

#define BLUE    1       // 5
#define GREEN   2       // 6
#define RED     9       // 7
#define YELLOW  10      // 8

#define BTN2    5
#define BTN3    6
#define BTN4    7



