#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX_LEN 50
#define AMOUNT_OF_POINTS 43
#define AMOUNT_OF_CONNECTIONS 55
#define AMOUNT_OF_CLUSTERS 3
#define POSTOFFICE_LOCATION 0

struct points{
  char name[MAX_LEN];
  int x, y;
  double distantToEnd;
  int status;
};
typedef struct points points;

struct connections{
  char firstPoint[MAX_LEN];
  char secondPoint[MAX_LEN];
  double lenght;
};
typedef struct connections connections;

double lenghtBetween(int x1, int y1, int x2, int y2);

int main(void){
  points point[AMOUNT_OF_POINTS];
  connections connection[AMOUNT_OF_CONNECTIONS];
  int i = 0, cx = 0, cy = 0, min = 0, start = 0, end = 0, endpoints = 0, cluster = 1, current = 0;


  //Husk at have filen map.txt
  FILE *map;
  map = fopen("clusterlist.txt", "r");


  if (map != NULL) {
    printf("File loaded\n");

    for(i = 0; i<=AMOUNT_OF_POINTS-1; i++){
      fscanf(map,"%s (%d, %d) %d", point[i].name, &point[i].x, &point[i].y, &point[i].status);
    }
    fclose(map);
  } else {
    printf("Error\n");
  }

  map = fopen("connections.txt", "r");

  if (map != NULL) {
    printf("File2 loaded\n");

    for(i = 0; i <= AMOUNT_OF_CONNECTIONS-1; i++){
      fscanf(map,"%s -- %s", connection[i].firstPoint, connection[i].secondPoint);
    }

    fclose(map);
  } else {
    printf("Error2\n");
  }


  start = POSTOFFICE_LOCATION;

  //Finder det end point der er taettest på startpunktet.
  for(int j = 0; cluster <= AMOUNT_OF_CLUSTERS; j++){
    min = 999; endpoints = 0;
    for(i = 0; i <= AMOUNT_OF_POINTS-1; i++){
      if (point[i].status == cluster) {
        point[i].distantToEnd = lenghtBetween(point[start].x, point[start].y, point[i].x, point[i].y); //sqrt(pow(point[i].x - point[start].x , 2)+pow(point[i].y - point[start].y, 2));
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
        point[i].distantToEnd = lenghtBetween(point[end].x, point[i].x, point[end].y, point[i].y);


      current = start;
      for(i = 0; i<=AMOUNT_OF_CONNECTIONS-1; i++){
        if (strcmp(point[current].name, connection[i].firstPoint) == 0) {
          
          printf("%s -> (%s)\n", point[current].name, connection[i].secondPoint);
          }

        if (strcmp(point[current].name, connection[i].secondPoint) == 0) {
          printf("%s -> (%s)\n", point[current].name, connection[i].firstPoint);
        }
      }

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

double lenghtBetween(int x1, int y1, int x2, int y2) {
  return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}
