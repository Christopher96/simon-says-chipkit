int main() {
    // Telling the c compiler that it is indeed an address and not an integer by doing a type cast
    volatile int *inport = (volatile int *) 0x8000abc0;
    volatile int *outport = (volatile int *) 0x80007bc0;

    // * Here
    while(1) {
        // The compiler would move this * (unless prohibited by volatile)
        int inputdata = *inport;
        inputdata = inputdata & (1 << 3);
        if(inputdata) {
            *outport = 0x3f;
        } else {
            *outport = 0;
        }
    }
}
