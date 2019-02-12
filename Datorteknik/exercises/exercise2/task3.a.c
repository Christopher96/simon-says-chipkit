int main() {
    volatile int* trise = 0xBF886100;
    *trise = *trise & ~(7 << 1);   // 0xFFF...F1

}
