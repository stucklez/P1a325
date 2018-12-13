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
};
typedef struct connections connections;

void scanMap(points point[AMOUNT_OF_POINTS]);
void scanConnections(connections connection[AMOUNT_OF_CONNECTIONS]);
void createRoute(points point[AMOUNT_OF_POINTS], connections connection[AMOUNT_OF_CONNECTIONS]);
double lenghtBetween(int x1, int y1, int x2, int y2);
int findLocation(points point[AMOUNT_OF_POINTS], char *name);

int main(void){
  points point[AMOUNT_OF_POINTS];
  connections connection[AMOUNT_OF_CONNECTIONS];

  scanMap(point);
  scanConnections(connection);

  createRoute(point, connection);

  return 0;
}

//Funktion der skanner roadmap fra tekstfil, det vil sige punkter, status og clusters.
void scanMap(points point[AMOUNT_OF_POINTS]){

  FILE *map;
  map = fopen("clusterlist.txt", "r");

  if (map != NULL) {
    printf("File loaded\n");

    for(int i = 0; i<=AMOUNT_OF_POINTS-1; i++){
      fscanf(map,"%s (%d, %d) %d", point[i].name, &point[i].x, &point[i].y, &point[i].status);
    }
    fclose(map);
  } else {
    printf("Error\n");
  }
}

//Function that loads textfile with each connection between points.
void scanConnections(connections connection[AMOUNT_OF_CONNECTIONS]){
  FILE *map;
  map = fopen("connections.txt", "r");

  if (map != NULL) {
    printf("File2 loaded\n");
    for(int i = 0; i <= AMOUNT_OF_CONNECTIONS-1; i++){
      fscanf(map,"%s -- %s", connection[i].firstPoint, connection[i].secondPoint);
    }
    fclose(map);
  } else {
    printf("Error2\n");
  }
}

//Function that creates a route using A* algorithm with the data scanned in "scanMap" & "scanConnections".
void createRoute(points point[AMOUNT_OF_POINTS], connections connection[AMOUNT_OF_CONNECTIONS]){
  int i = 0, cx = 0, cy = 0, min = 0, start = 0, end = 0,
      endpoints = 0, cluster = 1, current = 0, location = 0;
  int paths[MAX_LEN][MAX_LEN], path = 0, a = 0, p = 0, currentpath = 0;
  int score[MAX_LEN], startscore =0;

  start = POSTOFFICE_LOCATION;

  //Finder det end point der er taettest på startpunktet.
  for(int j = 0; cluster <= AMOUNT_OF_CLUSTERS; j++){
    int j = 0;
    min = 999; endpoints = 0;
    for(i = 0; i <= AMOUNT_OF_POINTS-1; i++){
      if (point[i].status == cluster) {
        point[i].distantToEnd = lenghtBetween(point[start].x, point[start].y, point[i].x, point[i].y);
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
      currentpath = 0;
      for(i = 0; i <= path; i++){
        paths[currentpath][MAX_LEN-1] = 0;
        paths[currentpath][MAX_LEN-2] = 0;
      }
      path = 0; a = -1;

      while(current != end){

      p = 0;

      //Leder efter connections der indeholder current point og tildeler Location det punkt current haenger sammen med.
      for(i = 0; i <= AMOUNT_OF_CONNECTIONS-1; i++){
        if (strcmp(point[current].name, connection[i].firstPoint) == 0
            || strcmp(point[current].name, connection[i].secondPoint) == 0) {
          if (strcmp(point[current].name, connection[i].firstPoint) == 0) {
            location = findLocation(point, connection[i].secondPoint);
          } else if (strcmp(point[current].name, connection[i].secondPoint) == 0) {
            location = findLocation(point, connection[i].firstPoint);
          }

          if (p > 0) {
            paths[currentpath][MAX_LEN-2] = a;
            path++;
            for(int o = 0; o<=a; o++)
              paths[path][o] = paths[currentpath][o];
            paths[path][MAX_LEN-1] = startscore;
            currentpath = path;

          } else {
            a++;
          }
          p++;
          paths[currentpath][a] = location;
          paths[currentpath][MAX_LEN-1] += lenghtBetween(point[current].x, point[current].y, point[location].x, point[location].y) + point[location].distantToEnd;
          printf("%d %d %s -> %s %d\n", currentpath, a, point[current].name, point[location].name, paths[currentpath][MAX_LEN-1]);

        }
      }

      //Printer array Paths
      for(i = 0; i <= currentpath; i++){
        for(int o = 0; o <= paths[i][MAX_LEN-2]; o++){
          printf("%d %d %s %d\n", i, o, point[paths[i][o]].name, paths[i][MAX_LEN-1]);
        }
      }
      min = 999;
      for (i = 0; i <= currentpath; i++){
        if (paths[i][MAX_LEN-1] <= min) {
          min = paths[i][MAX_LEN-1];
          location = i;
        }
      }

      printf("The best choice: %s\n", point[paths[location][0]].name);
      currentpath = location;

      startscore = paths[currentpath][MAX_LEN-1];
      //This needs fixing

      printf("Points in current path: %d\n", paths[currentpath][MAX_LEN-2]);
      //current = paths[currentpath][MAX_LEN-2];
      for(i=0; i<= paths[currentpath][MAX_LEN-2]; i++){
        current = paths[currentpath][i];
      }

      printf("New current path: %d\nNew current point: %s\n", currentpath, point[current].name);

    }
      //-----------------------------

      //Efter at have fundet en rute fra start til slut aendre den end til start og repeater
      point[end].status = 0;
      start = end;
    }
  }
}

//Funktion der beregner laengde mellem to punkter
double lenghtBetween(int x1, int y1, int x2, int y2) {
  return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

//Funktion der finder en lokation til et punkt ud fra punktets navn.
int findLocation(points point[AMOUNT_OF_POINTS], char *name){
  for(int i = 0; i <= AMOUNT_OF_POINTS; i++){
    if (strcmp(name, point[i].name) == 0) {
      return i;
    }
  }
}
