
#include <stdio.h> 
#include <math.h>
#include <stdlib.h>

int lengthtotime (int yeet); 

int main(void) {
    int yeet = 30; 
    printf("%d", lengthtotime(yeet));

    return 0; 
}

int lengthtotime (int yeet) {
    int boi = yeet;
    return (boi * 100) / 333;     
}