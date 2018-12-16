#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAX_LEN 50 //This is used for various arrays for uknown sizes under 50.
#define MAX_POOL_SIZE 20 //This indicates how many orders are being scanned into the array of orders.
#define AMOUNT_OF_POINTS 43
#define AMOUNT_OF_CONNECTIONS 55
#define POSTOFFICE_LOCATION 0
//START_TIME is the time of day the delivery route are set to start.
#define START_TIME 480 //480 minutes corrispond to 8:00

//Struct to store pending orders.
struct order {
  int status;
  char address[MAX_LEN];
  int odrNumber;
  char firstName[MAX_LEN], lastName[MAX_LEN];
  int time;
};
typedef struct order order;

//Struct to store point information for the road network.
struct points{
  char name[MAX_LEN];
  int x, y;
  double distantToEnd;
  int status;
  int associatedOrder;
};
typedef struct points points;

//Struct that stores the connections inbetween points.
struct connections{
  char firstPoint[MAX_LEN];
  char secondPoint[MAX_LEN];
};
typedef struct connections connections;

//Prototypes
void scanOrders(order *pool);
void adminDialog(order *pool, int progress);
void userDialog(order *pool);
void prntOrder(order *pool, int progress);
void scanMap(points point[AMOUNT_OF_POINTS]);
void scanConnections(connections connection[AMOUNT_OF_CONNECTIONS]);
void setPointStatus(points point[AMOUNT_OF_POINTS], order *pool);
void createRoute(points point[AMOUNT_OF_POINTS], connections connecton[AMOUNT_OF_CONNECTIONS], order *pool, int cluster);
double lenghtBetween(int x1, int y1, int x2, int y2);
int findLocation(points point[AMOUNT_OF_POINTS], char *name);
int convertLenToMin(double length);
void shuffleOrderStatus(order *pool);
void createClusters (points point[AMOUNT_OF_POINTS], order *pool);

//Main
int main(void){
  int input = 0, clusterinput = 0, clusteramount = 0, progress = 0;
  order pool[MAX_POOL_SIZE];
  points point[AMOUNT_OF_POINTS];
  connections connection[AMOUNT_OF_CONNECTIONS];

  //Starts out by scanning the orderlist.
	scanOrders(pool);
  //Then prompting the admin dialog.
  adminDialog(pool, progress);

  //Scans the road network.
  scanMap(point);
  scanConnections(connection);

  //Chose if you want to calculate clusters or only use one cluster.
  printf("Choose cluster setting:\n[1] Automatic cluster creation\n"
         "[2] Only one cluster\n");
  scanf("%d", &clusterinput);

  if (clusterinput == 1) {
    clusteramount = 3;
    createClusters(point, pool);
  } else if (clusterinput == 2) {
    clusteramount = 1;
    setPointStatus(point, pool);
  }

  //Creates route between order adresses by the determined clusters
  createRoute(point, connection, pool, clusteramount);
  progress++;

  //Admin dialog for next step after first route creation
  while(input != 4){
    printf("\n\nThe first times are estimated, what next?:\n[1] View orders. (With status and estimated time.) \n"
         "[2] Simulate user input for next route creation.\n"
         "[3] See example on the user interface from a specific order.\n[4] Calculate the final route.\n\n");

    scanf("%d", &input);

    if (input == 1) {
      //View orders. (With first time window and status.)
      prntOrder(pool, progress);
    } else if (input == 2) {
      //Shuffle order status to simulate user outcome.
      shuffleOrderStatus(pool);
    } else if (input == 3) {
      //User dialog for a specific order.
      userDialog(pool);
    } else if (input < 1 || input > 4){
      printf("Invalid input\n");
    }
  }

  //Uses same cluster method for second route.
  if (clusterinput == 1) {
    createClusters(point, pool);
  } else if (clusterinput == 2) {
    setPointStatus(point, pool);
  }

  //Second route creation.
  createRoute(point, connection, pool, clusteramount);
  progress++;

  //Final admin dialog
  input = 0;
  while(input != 2){
    printf("[1] View final order times. / [2] Exit program.\n");
    scanf("%d", &input);
    if (input == 1) {
      prntOrder(pool, progress);
    } else if (input == 2) {
      printf("Exiting program.\n");
    } else {
      printf("Wrong input\n");
    }
  }

  return 0;
}

//Function that scans the orderlist from the textfile "orderlist.txt".
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

//First admin dialog before route creation.
void adminDialog(order *pool, int progress){
  int i, j, r = 0, t = 0;
  int usrNumber = 0, firstInput = 0, adminInput = 0, userInput = 0,
      userOrderNr = 0, fileLoaded = 0;

      //Start of admin dialog.
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
            prntOrder(pool, progress);
          } else if(adminInput == 2){
            //Exitting function to continue to route creation

            t=1;
            }
          if(adminInput!= 1 && adminInput!= 2 && adminInput!= 0){
              printf("Something went wrong please try again\n");
            }
          }
}
// Function to view specific order from user perspective.
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

    // The user gets a time window where they can answer if they can recieve the package in the givin time window.
    // If no will they get to chose between new date or pickup point.
    printf("%s %s, Your package will be delivered between \n\n %d:%d -- %d:%d"
           "\n\nAre you able to recieve yout package in this time window?\n"
           "[1] Yes -- [2] No, I want a new time -- [3] No, I want it delivered to Pickup point.\n",
           pool[usrNumber].firstName, pool[usrNumber].lastName, hours1, mins1, hours2, mins2);
    scanf("%d", &userInput);
    if (userInput == 1) {
      // status 1 = mark address as end point for second route creation.
      pool[usrNumber].status = 1;
    }
    if (userInput == 2) {
      // status 0 = address gets ignored for second route creation.
      pool[usrNumber].status = 0;
    }
    if (userInput == 3) {
      // status 2 = pickuppoint nearest to the address is marked as active endpoint.
      pool[usrNumber].status = 2;
    }
    if (userInput < 1 && userInput > 3) {
      printf("Invalid input\n");
    }
}

// Prints order and current time window, if calculated.
void prntOrder(order *pool, int progress){
  int i, hours1 = 0, hours2 = 0, mins1 = 0, mins2 = 0;
  for (i = 0; i < MAX_POOL_SIZE; i++) {
    printf(" Status: %d", pool[i].status);

    //Printing the actual time window.
    if (progress == 0) {
      printf(" Time: N/A\n");
    } else if (pool[i].status == 1 && progress == 1) {
      hours1 = (pool[i].time-90) / 60;
      mins1 = (pool[i].time-90) % 60;

      hours2 = (pool[i].time+90) / 60;
      mins2 = (pool[i].time+90) % 60;
      printf(" Time: %d:%d -- %d:%d\n", hours1, mins1, hours2, mins2);
    } else if (pool[i].status == 1 && progress == 2) {
      hours1 = (pool[i].time-15) / 60;
      mins1 = (pool[i].time-15) % 60;

      hours2 = (pool[i].time+15) / 60;
      mins2 = (pool[i].time+15) % 60;
      printf(" Time: %d:%d -- %d:%d\n", hours1, mins1, hours2, mins2);
    } else if (pool[i].status == 0) {
      printf(" (The package is schedueled to another day.)\n");
    } else if (pool[i].status == 2) {
      printf(" (The package is being delivered to the nearest pickup point.)\n");
    }

    printf(" Adress: %s\n Order number: %d\n"
           " Name: %s %s\n\n",
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

    for(int i = 0; i<=AMOUNT_OF_POINTS; i++){
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

// Function that sets all active orders to one cluster.
// Gets used if the one cluster option chosen in admin dialog.
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
void createRoute(points point[AMOUNT_OF_POINTS], connections connection[AMOUNT_OF_CONNECTIONS], order *pool, int clusteramount){
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
  for(int j = 0; cluster <= clusteramount; j++){
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
    if (cluster == clusteramount && endpoints == 0) {
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

// Function that converts lenght (from cordinates) into time/minutes.
int convertLenToMin(double length){
  return ((length * 100) / 333) + 2;
}

//Choosing random order status to simulate user outcome for second route creation.
void shuffleOrderStatus(order *pool){
  srand(time(NULL));
  for(int i = 0; i <= MAX_POOL_SIZE; i++){
    pool[i].status = rand() % 3;
  }
  printf("\nThe order statuses are now shuffled.\n\n");
}

// Function that creates the delivery clusters
void createClusters (points point[AMOUNT_OF_POINTS], order *pool) {
    int a = 0, b = 0, c = 0, i, j;
    //This for loop finds the location of the Pickup points in the array
    for (i = 0; i <= AMOUNT_OF_POINTS; i++) {
      //printf("%d %s\n", i, point[i].name);
        if (strcmp(point[i].name, "Pickup1") == 0) {
            a = i;
            point[i].status = 0;
        }
        if (strcmp(point[i].name, "Pickup2") == 0) {
            b = i;
            point[i].status = 0;
        }
        if (strcmp(point[i].name, "Pickup3") == 0) {
            c = i;
        }
      }


    //This for loop calculates the distance between the deliveryadresses and the pickup points. And assigns the adresses to the nearest pickup point
    // The pickup points is then the center of the clusters
    for (i = 0; i <= MAX_POOL_SIZE; i++) {
      for (j = 0; j <= AMOUNT_OF_POINTS; j++) {

      if (strcmp(pool[i].address, point[j].name) == 0 && (pool[i].status == 1 || pool[i].status == 2)) {
        if (lenghtBetween(point[a].x, point[a].y, point[i].x, point[i].y) < lenghtBetween(point[b].x, point[b].y, point[i].x, point[i].y)
        && lenghtBetween(point[a].x, point[a].y, point[i].x, point[i].y) < lenghtBetween(point[c].x, point[c].y, point[i].x, point[i].y)) {
            if (pool[i].status == 1) {
              point[j].status = 1;
              point[j].associatedOrder = i;
            }else if (pool[i].status == 2) {
              point[a].status = 1;
            }
        }
        if (lenghtBetween(point[b].x, point[b].y, point[i].x, point[i].y) < lenghtBetween(point[a].x, point[a].y, point[i].x, point[i].y)
        && lenghtBetween(point[b].x, point[b].y, point[i].x, point[i].y) < lenghtBetween(point[c].x, point[c].y, point[i].x, point[i].y)) {
          if (pool[i].status == 1) {
            point[j].status = 3;
            point[j].associatedOrder = i;
          }else if (pool[i].status == 2) {
            point[a].status = 3;
          }
        }
        if (lenghtBetween(point[c].x, point[c].y, point[i].x, point[i].y) < lenghtBetween(point[a].x, point[a].y, point[i].x, point[i].y)
        && lenghtBetween(point[c].x, point[c].y, point[i].x, point[i].y) < lenghtBetween(point[b].x, point[b].y, point[i].x, point[i].y)) {
          if (pool[i].status == 1) {
            point[j].status = 2;
            point[j].associatedOrder = i;
          }else if (pool[i].status == 2) {
            point[a].status = 2;
          }
        }
      }
    }
  }
}
