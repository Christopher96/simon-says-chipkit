// Helper functions
typedef enum { OUTPUT, INPUT } Pinmode;
typedef enum { LOW, HIGH } Signal;

// PORT             Bit     Pinout

// PORT F
#define BTN1        1

// PORT D

#define YELLOW      9       // 7
#define BLUE        2       // 6
#define RED         1       // 5
#define GREEN       0       // 3

#define PWM         3       // 9

#define E           5       // 34
#define RW          6       // 36
#define RS          7       // 37

// Change notification
#define CN_BLUE     14
#define CN_GREEN    15
#define CN_RED      16
#define CN_YELLOW   17

#define EEPROM_SLAVE_ADDR 0x50
#define SEED_ADDR 0x0FF
#define HIGHSCORES_ADDR 0x00
#define HIGHSCORE_SIZE 5
#define HIGHSCORES_MAX HIGHSCORE_SIZE * 100

#define EEPROM_WRITE 0
#define EEPROM_READ 1

