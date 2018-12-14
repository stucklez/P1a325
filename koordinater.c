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

//Function that scans the cordinates and name for each point on the roadmap.
void scanMap(points point[AMOUNT_OF_POINTS]){

  FILE *map;
  map = fopen("clusterlist.txt", "r");

  if (map != NULL) {
    printf("Map file loaded\n");

    for(int i = 0; i<=AMOUNT_OF_POINTS-1; i++){
      fscanf(map,"%s (%d, %d) %d", point[i].name, &point[i].x, &point[i].y, &point[i].status);
    }
    fclose(map);
  } else {
    printf("Error loading map file \"clusterlist.txt\"\n");
  }
}

//Function that loads textfile with each connection between points.
void scanConnections(connections connection[AMOUNT_OF_CONNECTIONS]){
  FILE *map;
  map = fopen("connections.txt", "r");

  if (map != NULL) {
    printf("Connection file loaded\n");
    for(int i = 0; i <= AMOUNT_OF_CONNECTIONS-1; i++){
      fscanf(map,"%s -- %s", connection[i].firstPoint, connection[i].secondPoint);
    }
    fclose(map);
  } else {
    printf("Error loading connections\n");
  }
}

//Function that creates a route using A* algorithm with the data scanned in "scanMap" & "scanConnections".
void createRoute(points point[AMOUNT_OF_POINTS], connections connection[AMOUNT_OF_CONNECTIONS]){
  int i = 0, cx = 0, cy = 0, min = 0, start = 0, end = 0,
      endpoints = 0, cluster = 1, current = 0, location = 0,
      startscore = 0, duplicate = 0, prev = 0;
  //The different paths are stored in a 2 dimentional array called "paths[][]"
  int paths[MAX_LEN][MAX_LEN], path = 0, a = 0, p = 0, currentpath = 0;


  start = POSTOFFICE_LOCATION;

  //For loop that first finds the closest endpoint to the current point.
  //Then it finds the shortest path from start to end by using A*.
  //The end point then becomes the new start point and the process repeats
  //Until there are no more endpoints.
  for(int j = 0; cluster <= AMOUNT_OF_CLUSTERS; j++){
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
    //If there is no more endpoints left, it will set the last endpoint to the Post office.
    if (cluster == AMOUNT_OF_CLUSTERS && endpoints == 0) {
      end = POSTOFFICE_LOCATION; endpoints++; cluster++;
    }

    if (endpoints == 0) {
      cluster++;
    } else {
      //Location to the start and endpoint is put into the variables start and end.
      printf("#%-2d Cluster: %-2d Start: %-15s End: %-15s Lenght: %lf\n", j+1, cluster, point[start].name, point[end].name, point[end].distantToEnd);

      //distance to end point from all other points are calculated and stored in the respected
      //point struct to be used in the score when deciding which path to further develop.
      for(i=0; i <= AMOUNT_OF_POINTS-1; i++)
        point[i].distantToEnd = lenghtBetween(point[i].x, point[i].y, point[end].x, point[end].y);

      //The 2d array "paths" gets cleared after recieving new start and end point.
      current = start;
      currentpath = 0;
      for(i = 0; i <= path+1; i++){
        paths[i][MAX_LEN-1] = 0;
        paths[i][MAX_LEN-2] = 0;
      }
      path = 0; a = -1;
      startscore = 0;

      //While loop that continues until the current point is equal to the end point.
      while(current != end){
        p = 0;

        //Looks at possible connections to go to from current point.
        for(i = 0; i <= AMOUNT_OF_CONNECTIONS-1; i++){
          if (strcmp(point[current].name, connection[i].firstPoint) == 0
              || strcmp(point[current].name, connection[i].secondPoint) == 0) {
            if (strcmp(point[current].name, connection[i].firstPoint) == 0) {
              location = findLocation(point, connection[i].secondPoint);
            } else if (strcmp(point[current].name, connection[i].secondPoint) == 0) {
              location = findLocation(point, connection[i].firstPoint);
            }

            //For loop that detects if the current point already is on the current path.
            for(int k = 0; k <= a+1; k++){
              if (location == paths[currentpath][k] || location == start) {
                duplicate = 1;
              }
            }

            //New points are now placed in the 2d array. duplicates are skipped.
            //First connection gets put into the currentpath, all other connections
            //gets put on a new path with all previous points on current path copied
            //to the new path.
            if (duplicate != 1) {
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
              //The path will be giving a score based on how long the path is combined with the lenght to the end point.
              //The program can then chose to further develop on the path with the lowest score
              //thereby in the end finding the optimal route from start to end. 
              paths[currentpath][MAX_LEN-1] += lenghtBetween(point[current].x, point[current].y, point[location].x, point[location].y)
                                             + point[location].distantToEnd;

            }
            duplicate = 0;
          }
        }
        //Amount of points in the currentpath get placed on a fixed location in the array.
        paths[currentpath][MAX_LEN-2] = a;

        //Finds the path with the lowest score.
        min = 999;
        for (i = 0; i <= path; i++){
          if (paths[i][MAX_LEN-1] <= min) {
            min = paths[i][MAX_LEN-1];
            location = i;
          }
        }

        //Choses the path with the lowest score as the currentpath.
        //And selects the last point in the new currentpath as current.
        currentpath = location;
        startscore = paths[currentpath][MAX_LEN-1];
        if (paths[currentpath][MAX_LEN-2] != a) {
          a = paths[currentpath][MAX_LEN-2];
        }

        //Selects the right amount of points to further develop on.
        prev = current;
        for(i=0; i<= paths[currentpath][MAX_LEN-2]; i++){
          current = paths[currentpath][i];
        }

        //Gives 100 extra points if it have chosen the same path,
        //To prevent it from getting stuck.
        if(prev == current){
          paths[currentpath][MAX_LEN-1] += 100;
        }
      }

      //Prints out the final path.
      for(int k = 0; k <= paths[currentpath][MAX_LEN-2]; k++)
        printf("-> %s", point[paths[currentpath][k]].name);

      printf("\n");

      //After finding an optimal route from start to finnish will the end point
      //be set as the new start point and the precedure repeats itself.
      point[end].status = 0;
      start = end;
    }
  }
}

//Function that calculates the lenght between two points by using Pythagoras.
double lenghtBetween(int x1, int y1, int x2, int y2) {
  return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

//Function that finds the location in the point array from name of the point.
int findLocation(points point[AMOUNT_OF_POINTS], char *name){
  for(int i = 0; i <= AMOUNT_OF_POINTS; i++){
    if (strcmp(name, point[i].name) == 0) {
      return i;
    }
  }
}
