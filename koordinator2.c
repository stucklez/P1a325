#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAPSIZE 31

struct streets{
  char streetName[50];
  int x1, y1;
  int x2, y2;
  int streetLenght;
};
typedef struct streets streets;


int main(void){
  int i = 0, j = 0, cx = 0, cy = 0, min = 999, location =0;
  streets street[MAPSIZE];

  FILE *map;
  map = fopen("map2.txt", "r");


  if (map != NULL) {
    printf("File loaded\n");

    for(i = 0; i<=MAPSIZE; i++){
      fscanf(map,"%s start (%d,%d) slut (%d,%d) : %d", street[i].streetName, &street[i].x1, &street[i].y1, &street[i].x2, &street[i].y2, &street[i].streetLenght);
      //printf("%-30s: X1:%-3d Y1:%-3d X2:%-3d Y2:%-3d Lenght: %d\n", street[i].streetName, street[i].x1, street[i].y1, street[i].x2, street[i].y2, street[i].streetLenght);
    }

  } else {
    printf("Error\n");
  }
  min = 999;
  cx = 2; cy = 14;
  for(i = 0; i<=MAPSIZE; i++){
    if ((cx == street[i].x1 && cy == street[i].y1) || (cx == street[i].x2 && cy == street[i].y2)) {
      printf("%s %d\n", street[i].streetName, street[i].streetLenght);
      if (street[i].streetLenght <= min) {
        min = street[i].streetLenght;
        location = i;
      }
    }
  }

printf("Shortest street from current point is %s with a lenght of %d.\n", street[location].streetName, street[location].streetLenght);


  return 0;
}
