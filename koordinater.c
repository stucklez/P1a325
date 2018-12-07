#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define AMOUNT_OF_POINTS 43

struct points{
  char point_name[50];
  int x, y;
  double distantToEnd;
  int status;
};
typedef struct points points;


int main(void){
  points point[AMOUNT_OF_POINTS];
  int i, cx = 0, cy = 0, min = 0, start = 0, end = 0, endpoints = 1;


  //Husk at have filen map.txt
  FILE *map;
  map = fopen("map.txt", "r");


  if (map != NULL) {
    printf("File loaded\n");

    for(i = 0; i<=AMOUNT_OF_POINTS-1; i++){
      fscanf(map,"%s (%d, %d) %d", point[i].point_name, &point[i].x, &point[i].y, &point[i].status);
    }
  } else {
    printf("Error\n");
  }

  start = 0;

  //Finder det end point der er taettest på startpunktet.
  for(int j = 0; endpoints != 0; j++){
    min = 999; endpoints = 0;
    for(i = 0; i <= AMOUNT_OF_POINTS-1; i++){
      if (point[i].status == 1) {
        point[i].distantToEnd = sqrt(pow(point[i].x - point[start].x , 2)+pow(point[i].y - point[start].y, 2));
        endpoints++;
        if (point[i].distantToEnd <= min) {
          min = point[i].distantToEnd; end = i;
        }
      }
    }

    //location til start og slut punkt er lagt i variablerne start og end.
    printf("#%-2d Start: %-25s End: %-25s Lenght: %lf\n", j, point[start].point_name, point[end].point_name, point[end].distantToEnd);

    //
    //A* skal implementeres her.
    //

    //Saetter end point som det nye start punkt og markere det som done. repeat.
    point[end].status = 0;
    start = end;
  }

  return 0;
}
