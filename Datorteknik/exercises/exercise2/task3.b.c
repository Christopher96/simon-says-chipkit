#include <pic32mc.h>

int main() {
    TRISE = TRISE & ~(7 << 1);
    TRISECLR = (7 << 1);        // Same as above
}
