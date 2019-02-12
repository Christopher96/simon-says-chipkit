void makelist(int start, int length) {
    for(int i=0; i<length; i++) {
        factlist[i] = fact(start);
        start++;
    }
}
