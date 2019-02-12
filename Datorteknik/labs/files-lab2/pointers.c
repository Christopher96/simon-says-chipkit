#include <stdio.h>

char* text1 = "This is a string.";
char* text2 = "Yet another thing.";
int count = 0;

char list1[80];
char list2[80];

void copycodes(char* text, int* list, int* count) {
    while(1) {
        char text_char = *(text); // get the character from where text is pointing to
        if(text_char == 0) // if there are no characters left break the loop
            return;
        *list = text_char; // assign the character to the current list address

        text++; // increments to the next byte
        list++; // increments to the next word
        (*count)++; // derefrencing the count variable then increment it
    }
}

void work() {
   copycodes(text1, (int*)list1, &count);
   copycodes(text2, (int*)list2, &count);
}

void printlist(const int* lst){
  printf("ASCII codes and corresponding characters.\n");
  while(*lst != 0){
    printf("0x%03X '%c' ", *lst, (char)*lst);
    lst++;
  }
  printf("\n");
}

void endian_proof(const char* c){
  printf("\nEndian experiment: 0x%02x,0x%02x,0x%02x,0x%02x\n", 
         (int)*c,(int)*(c+1), (int)*(c+2), (int)*(c+3));
  // little endian since LSB is to the right and MSB to the left
  
}

int main(void){
  work();

  printf("\nlist1: ");
  printlist(list1);
  printf("\nlist2: ");
  printlist(list2);
  printf("\nCount = %d\n", count);

  endian_proof((char*) &count);
}
