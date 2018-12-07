#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define AMOUNT_OF_POINTS 43
#define AMOUNT_OF_CLUSTERS 3
#define POSTOFFICE_LOCATION 0

struct points{
  char name[50];
  int x, y;
  double distantToEnd;
  int status;
};
typedef struct points points;


int main(void){
  points point[AMOUNT_OF_POINTS];
  int i, cx = 0, cy = 0, min = 0, start = 0, end = 0, endpoints = 0, cluster = 1;


  //Husk at have filen map.txt
  FILE *map;
  map = fopen("clusterlist.txt", "r");


  if (map != NULL) {
    printf("File loaded\n");

    for(i = 0; i<=AMOUNT_OF_POINTS-1; i++){
      fscanf(map,"%s (%d, %d) %d", point[i].name, &point[i].x, &point[i].y, &point[i].status);
    }
  } else {
    printf("Error\n");
  }

  start = POSTOFFICE_LOCATION;

  //Finder det end point der er taettest på startpunktet.
  for(int j = 0; cluster <= AMOUNT_OF_CLUSTERS; j++){
    min = 999; endpoints = 0;
    for(i = 0; i <= AMOUNT_OF_POINTS-1; i++){
      if (point[i].status == cluster) {
        point[i].distantToEnd = sqrt(pow(point[i].x - point[start].x , 2)+pow(point[i].y - point[start].y, 2));
        endpoints++;
        if (point[i].distantToEnd <= min) {
          min = point[i].distantToEnd; end = i;
        }
      }
    }
    //Hvis der ikke er flere endpoints gaar den tilbage til postoffice.
    if (cluster == AMOUNT_OF_CLUSTERS && endpoints == 0) {
      end = POSTOFFICE_LOCATION; endpoints++; cluster++;
    }

    if (endpoints == 0) {
      cluster++;
    } else {
      //location til start og slut punkt er lagt i variablerne start og end.
      printf("#%-2d Cluster: %-2d Start: %-15s End: %-15s Lenght: %lf\n", j+1, cluster, point[start].name, point[end].name, point[end].distantToEnd);

      //Alle punkter faar nu en afstand til det endpoint vi lige har fundet.
      for(i=0; i <= AMOUNT_OF_POINTS-1; i++)
        point[i].distantToEnd = sqrt(pow(point[i].x - point[end].x , 2) + pow(point[i].y - point[end].y, 2));

      //Vi har nu et start punkt og et slutpunkt samt laenge til slutpunkt på alle punkter.
      //
      //A* skal implementeres her.
      //

      point[end].status = 0;
      start = end;
    }
  }
  return 0;
}
