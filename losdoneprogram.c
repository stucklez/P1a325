#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX_LEN 50
#define AMOUNT_OF_POINTS 44
#define AMOUNT_OF_CONNECTIONS 55
#define AMOUNT_OF_CLUSTERS 3
#define POSTOFFICE_LOCATION 0
#define MAX_LEN 50
#define MAX_POOL_SIZE 20

struct order {
  int status;
  int day, month, year;
  char address[MAX_LEN];
  int streetNumber;
  int odrNumber;
  char firstName[MAX_LEN], lastName[MAX_LEN];
};
typedef struct order order;
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

void createClusters (points point[AMOUNT_OF_POINTS]); 
void scanMap(points point[AMOUNT_OF_POINTS]);
void scanConnections(connections connection[AMOUNT_OF_CONNECTIONS]);
void createRoute(points point[AMOUNT_OF_POINTS], connections connection[AMOUNT_OF_CONNECTIONS]);
double lenghtBetween(int x1, int y1, int x2, int y2);
int findLocation(points point[AMOUNT_OF_POINTS], char *name);
void readFile (order *pool, int *fileLoaded);
void userDialog (order *pool, points point[AMOUNT_OF_POINTS], connections connection[AMOUNT_OF_CONNECTIONS]);
void prntOrder(order *pool);
void manual_order(order *pool);

int main(void){
    order pool[MAX_POOL_SIZE];
    points point[AMOUNT_OF_POINTS];
    connections connection[AMOUNT_OF_CONNECTIONS];
	int fileLoaded = 0 ;
	readFile (pool, &fileLoaded);
    scanMap(point);
    scanConnections(connection);
    createClusters(point);
    createRoute(point, connection);
    //userDialog(pool, point, connection);
    
  return 0;

}
// Printer nuværende order pool.
void prntOrder(order *pool){
  int i;
  for (i = 0; i < MAX_POOL_SIZE; i++) {
    printf(" Status: %d\n Day: %d / %d - %d\n"
           " Adress: %s %d\n Odr number: %d\n"
           " Name: %s %s\n\n",
           pool[i].status, pool[i].day, pool[i].month, pool[i].year,
           pool[i].address, pool[i].streetNumber, pool[i].odrNumber,
           pool[i].firstName, pool[i].lastName);
  }
}

//Læser filen ind.
void readFile (order *pool, int *fileLoaded) {
  int i;
  FILE *orderlist;
  orderlist = fopen("orderlist.txt", "r");
  *fileLoaded = 1;

  if (orderlist != NULL){
    printf(" <File loaded>\n\n");
    for(i = 0; i != MAX_POOL_SIZE; i++){
      fscanf(orderlist,"%s %s - %s %d - %d",
             pool[i].firstName, pool[i].lastName, pool[i].address,
             &pool[i].streetNumber, &pool[i].odrNumber);
    }
    printf("\n");
    fclose(orderlist);
  } else {
    printf(" <Error loading file>\n\n");
  }
}

//Function that scans the cordinates and name for each point on the roadmap.
void scanMap(points point[AMOUNT_OF_POINTS]){

  FILE *map;
  map = fopen("clusterlist.txt", "r");

  if (map != NULL) {
    //printf("Map file loaded\n");

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
    //printf("Connection file loaded\n");
    for(int i = 0; i <= AMOUNT_OF_CONNECTIONS-1; i++){
      fscanf(map,"%s -- %s", connection[i].firstPoint, connection[i].secondPoint);
    }
    fclose(map);
  } else {
    printf("Error loading connections\n");
  }
}
// Function that creates the delivery clusters
void createClusters (points point[AMOUNT_OF_POINTS]) {
    int a, b, c, i; 
//This for loop finds the location of the Pickup points in the array
    for (i = 0; i < AMOUNT_OF_POINTS; i++) {
        if (strcmp(point[i].name, "Pickup1") == 0) {
            a = i; 
        }
        if (strcmp(point[i].name, "Pickup2") == 0) {
            b = i;
        }
        if (strcmp(point[i].name, "Pickup3") == 0) {
            c = i;
        }
        
    }
    printf("%d %d %d\n", a,b,c);
    
    //This for loop calculates the distance between the deliveryadresses and the pickup points. And assigns the adresses to the nearest pickup point
    // The pickup points is then the center of the clusters
    for (i = 0; i < AMOUNT_OF_POINTS; i++) {
      if (strlen(point[i].name) != 1 && strcmp(point[i].name, "PostOffice") != 0) {
        if (lenghtBetween(point[a].x, point[a].y, point[i].x, point[i].y) < lenghtBetween(point[b].x, point[b].y, point[i].x, point[i].y)
        && lenghtBetween(point[a].x, point[a].y, point[i].x, point[i].y) < lenghtBetween(point[c].x, point[c].y, point[i].x, point[i].y)) {
            point[i].status = 1;
        }
        if (lenghtBetween(point[b].x, point[b].y, point[i].x, point[i].y) < lenghtBetween(point[a].x, point[a].y, point[i].x, point[i].y)
        && lenghtBetween(point[b].x, point[b].y, point[i].x, point[i].y) < lenghtBetween(point[c].x, point[c].y, point[i].x, point[i].y)) {
            point[i].status = 3;
        }
        if (lenghtBetween(point[c].x, point[c].y, point[i].x, point[i].y) < lenghtBetween(point[a].x, point[a].y, point[i].x, point[i].y)
        && lenghtBetween(point[c].x, point[c].y, point[i].x, point[i].y) < lenghtBetween(point[b].x, point[b].y, point[i].x, point[i].y)) {
            point[i].status = 2; 
        }
    }
    //This is to make sure the pickuppoints have the same clusternumber as the adresse that is assigned to it 
    point[a].status = 1; 
    point[b].status = 3;
    point[c].status = 2; 
    printf("%s %d\n",point[i].name, point[i].status);
}
}

// Starten til vores brugerdialog.
void userDialog (order *pool, points point[AMOUNT_OF_POINTS], connections connection[AMOUNT_OF_CONNECTIONS]) {
  int i, j = 0, r = 0,o = 0,k=0,t=0;
  int usrNumber = 1, firstInput = 0, adminInput = 0, userInput = 0,
      userOrderNr = 0, fileLoaded = 0;

  /* Prompt for om der skal gives adgang til bruger eller admin dialog*/

  printf("Admin [1]\nBruger [2]\n");
  scanf("%d", &firstInput);
  /* Bruger dialog: Først bliver der promptet for ordrenumret, så ordren kan findes i den indlæste data.*/
  if (firstInput == 2) {
    printf("-----ReceiverDialog-----\n\n");
    while(0==k){  
      printf("Write your Ordernumber\n");
      scanf("%d", &userOrderNr);
      for(j=0;j<=MAX_POOL_SIZE;j++){
          if(userOrderNr==pool[j].odrNumber){
            usrNumber = j;
            k++;
          }
      }
      if(k!=1){
        printf("Please make sure the ordernumber is correct, and try again!\n");
      }
    }
  
  
  /* Brugerens pakke blive vist med tilhørende leverings information (Estimeret tid) og brugeren bliver spurgt om de kan modtage pakken*/
    printf("Your package will be delivered between \n\nbla to bla\n\nCan you receive it at that time? If yes [1] --- If no [2]\n");
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
  
  if(firstInput == 1){
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
          createRoute(point, connection);
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


//Starten på hvor vi kan sætte vores manualle ordre ind.
void manual_order(order *pool){

	printf("Enter manual order\n Firstname Lastname - Adress Streetnumber - Ordernumber\n");

	scanf("%s %s - %s %d - %d", pool[MAX_POOL_SIZE+1].firstName, pool[MAX_POOL_SIZE+1].lastName, pool[MAX_POOL_SIZE+1].address,
		              			&pool[MAX_POOL_SIZE+1].streetNumber, &pool[MAX_POOL_SIZE+1].odrNumber);


	printf("%s %s - %s %d - %d", pool[MAX_POOL_SIZE+1].firstName, pool[MAX_POOL_SIZE+1].lastName, pool[MAX_POOL_SIZE+1].address,
		              			 pool[MAX_POOL_SIZE+1].streetNumber, pool[MAX_POOL_SIZE+1].odrNumber);
}
