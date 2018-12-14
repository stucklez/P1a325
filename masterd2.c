#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LEN 50
#define MAX_POOL_SIZE 20
#define AMOUNT_OF_POINTS 43
#define AMOUNT_OF_CONNECTIONS 55
#define AMOUNT_OF_CLUSTERS 1
#define POSTOFFICE_LOCATION 0
#define START_TIME 480

struct order {
  int status;
  char address[MAX_LEN];
  int odrNumber;
  char firstName[MAX_LEN], lastName[MAX_LEN];
  int time;
};
typedef struct order order;

struct points{
  char name[MAX_LEN];
  int x, y;
  double distantToEnd;
  int status;
  int associatedOrder;
};
typedef struct points points;

struct connections{
  char firstPoint[MAX_LEN];
  char secondPoint[MAX_LEN];
};
typedef struct connections connections;

void scanOrders(order *pool);
void adminDialog(order *pool);
void userDialog(order *pool);
void prntOrder(order *pool);
void scanMap(points point[AMOUNT_OF_POINTS]);
void scanConnections(connections connection[AMOUNT_OF_CONNECTIONS]);
void setPointStatus(points point[AMOUNT_OF_POINTS], order *pool);
void createRoute(points point[AMOUNT_OF_POINTS], connections connecton[AMOUNT_OF_CONNECTIONS], order *pool);
double lenghtBetween(int x1, int y1, int x2, int y2);
int findLocation(points point[AMOUNT_OF_POINTS], char *name);
int convertLenToMin(double length);

int main(void){
  order pool[MAX_POOL_SIZE];
  points point[AMOUNT_OF_POINTS];
  connections connection[AMOUNT_OF_CONNECTIONS];

	scanOrders(pool);
  adminDialog(pool);

  scanMap(point);
  scanConnections(connection);

  setPointStatus(point, pool);

  createRoute(point, connection, pool);

  userDialog(pool);


  return 0;
}

//Læser filen ind.
void scanOrders (order *pool) {
  int i;
  FILE *orderlist;
  orderlist = fopen("orderlist.txt", "r");

  if (orderlist != NULL){
    printf(" <File loaded>\n\n");
    for(i = 0; i != MAX_POOL_SIZE; i++){
      fscanf(orderlist,"%s %s - %s - %d",
             pool[i].firstName, pool[i].lastName, pool[i].address,
             &pool[i].odrNumber);
      pool[i].status = 1;
    }
    printf("\n");
    fclose(orderlist);
  } else {
    printf(" <Error loading file>\n\n");
  }
}

void adminDialog(order *pool){
  int i, j, r = 0, t = 0;
  int usrNumber = 0, firstInput = 0, adminInput = 0, userInput = 0,
      userOrderNr = 0, fileLoaded = 0;

      //Start af admin dialog.
        adminInput = 0;
        while(t!=1){
        printf(" Choose an option below: \n\n");
        printf(" [1] View Current pool\n [2] Continue to route generating \n [0] Exit program\n");
        printf(" Input: ");
        scanf(" %d", &adminInput);
          //exit the program
          if(adminInput == 0){
              printf(" Program closed\n\n");
              exit (0);
          }
          //View current pool
          if(adminInput == 1){
            printf(" View current pool here\n\n");
            prntOrder(pool);
          } else if(adminInput == 2){
            //her går vi til genering.
            printf("Hey her kommer ruten din nar\n\n");
            t=1;
            }
          if(adminInput!= 1 && adminInput!= 2 && adminInput!= 0){
              printf("Something went wrong please try again\n");
            }

          if(adminInput==0){
            printf("Exiting program\n\n");
            exit(0);
            }
          }
}
// Starten til vores brugerdialog.
void userDialog (order *pool) {
  int i, j, r = 0;
  int usrNumber = 0, firstInput = 0, adminInput = 0, userInput = 0,
      userOrderNr = 0, fileLoaded = 0;
  int hours1 = 0, mins1 = 0, hours2 = 0, mins2 = 0;

    printf("-----ReceiverDialog-----\n\nWrite your Ordernumber\n");
    scanf("%d", &userOrderNr);
    for (j = 0; j < MAX_POOL_SIZE; j++) {
      if (pool[j].odrNumber == userOrderNr) {
          usrNumber = j;
        }
      }

    hours1 = (pool[usrNumber].time-30) / 60;
    mins1 = (pool[usrNumber].time-30) % 60;

    hours2 = (pool[usrNumber].time+30) / 60;
    mins2 = (pool[usrNumber].time+30) % 60;

  /* Brugerens pakke blive vist med tilhørende leverings information (Estimeret tid) og brugeren bliver spurgt om de kan modtage pakken*/
    printf("%s %s, Your package will be delivered between \n\n %d:%d -- %d:%d \n\nCan you receive it at that time? If yes [1] --- If no [2]\n", pool[usrNumber].firstName, pool[usrNumber].lastName, hours1, mins1, hours2, mins2);
    scanf("%d", &userInput);
    if (userInput == 1) {
      pool[usrNumber].status = 1;
    }
    if (userInput == 2) {
      pool[userOrderNr].status = 2;
    }
    if (userInput != 1 && userInput != 2) {
      printf("Wrong number! Try again\n");
    }
}

// Printer nuværende order pool.
void prntOrder(order *pool){
  int i;
  for (i = 0; i < MAX_POOL_SIZE; i++) {
    printf(" Status: %d\n"
           " Adress: %s\n Order number: %d\n"
           " Name: %s %s\n\n",
           pool[i].status,
           pool[i].address, pool[i].odrNumber,
           pool[i].firstName, pool[i].lastName);
  }
}

//Function that scans the cordinates and name for each point on the roadmap.
void scanMap(points point[AMOUNT_OF_POINTS]){

  FILE *map;
  map = fopen("clusterlist.txt", "r");

  if (map != NULL) {
    printf("Map file loaded\n");

    for(int i = 0; i<=AMOUNT_OF_POINTS-1; i++){
      fscanf(map,"%s (%d, %d) %d", point[i].name, &point[i].x, &point[i].y, &point[i].status);
      point[i].associatedOrder = -1;
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

void setPointStatus(points point[AMOUNT_OF_POINTS], order *pool){
  for(int i = 0; i <= MAX_POOL_SIZE; i++){
    for(int j = 0; j <= AMOUNT_OF_POINTS; j++){
      if (strcmp(pool[i].address, point[j].name) == 0 && pool[i].status == 1) {
        point[j].status = 1;
        point[j].associatedOrder = i;
      }
    }
  }
}

//Function that creates a route using A* algorithm with the data scanned in "scanMap" & "scanConnections".
void createRoute(points point[AMOUNT_OF_POINTS], connections connection[AMOUNT_OF_CONNECTIONS], order *pool){
  int i = 0, cx = 0, cy = 0, min = 0, start = 0, end = 0,
      endpoints = 0, cluster = 1, current = 0, location = 0,
      startscore = 0, duplicate = 0, prev = 0, combinedtime = START_TIME;
  //The different paths are stored in a 2 dimentional array called "paths[][]"
  int paths[MAX_LEN][MAX_LEN], path = 0, a = 0, p = 0, currentpath = 0;
  double combinedlenght = 0.0;



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
      combinedlenght = 0;

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

      combinedlenght = lenghtBetween(point[start].x, point[start].y, point[paths[currentpath][0]].x, point[paths[currentpath][0]].y);
      if (paths[currentpath][MAX_LEN-2] >= 1) {
        for(int k = 0; k <= paths[currentpath][MAX_LEN-2]-1; k++)
          combinedlenght += lenghtBetween(point[paths[currentpath][k]].x, point[paths[currentpath][k]].y, point[paths[currentpath][k+1]].x, point[paths[currentpath][k+1]].y);
      }

      combinedtime += convertLenToMin(combinedlenght);
      pool[point[end].associatedOrder].time = combinedtime;
      printf("\n Path Lenght %lf Time: %d\n", combinedlenght, combinedtime);

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

int convertLenToMin(double length){
  return ((length * 100) / 333) + 2;
}