#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void readFile (order *pool, int *fileLoaded);
void userDialog (order *pool);
void prntOrder(order *pool);
void manual_order(order *pool);

int main(void){
    order pool[MAX_POOL_SIZE];
	int fileLoaded = 0 ;
	readFile (pool, &fileLoaded);
  userDialog(pool);


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

// Starten til vores brugerdialog.
void userDialog (order *pool) {
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
      printf(" Choose an option below: \n\n");
      printf(" [1] View Current pool\n [2] Continue to route generating \n [0] Exit program\n");
      printf(" Input: ");
      scanf(" %d", &adminInput);
      while(t!=1){
        //exit the program
        if(adminInput == 0){
            printf(" Program closed\n\n");
            exit (0);
        }
        //View current pool
        if(adminInput == 1){
          printf(" View current pool here\n\n");
          prntOrder(pool);
          t = 1;
        } else if(adminInput == 2){
          //her går vi til genering.
          printf("Hey her kommer ruten din nar\n\n");
          t=1;
          }
        if(adminInput!= 1 && adminInput!= 2 && adminInput!= 0){
            printf("Something went wrong please try again\n");
            printf(" [1] View Current pool\n [2] Continue to route generating \n [0] Exit program\n");
            printf(" Input: ");
            scanf(" %d", &adminInput);
          }
        
        if(adminInput==0){
          printf("Exiting program\n\n");
          exit(0);
          }
      }
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
//Starten på hvor vi kan sætte vores manualle ordre ind.
void manual_order(order *pool){

	printf("Enter manual order\n Firstname Lastname - Adress Streetnumber - Ordernumber\n");

	scanf("%s %s - %s %d - %d", pool[MAX_POOL_SIZE+1].firstName, pool[MAX_POOL_SIZE+1].lastName, pool[MAX_POOL_SIZE+1].address,
		              			&pool[MAX_POOL_SIZE+1].streetNumber, &pool[MAX_POOL_SIZE+1].odrNumber);


	printf("%s %s - %s %d - %d", pool[MAX_POOL_SIZE+1].firstName, pool[MAX_POOL_SIZE+1].lastName, pool[MAX_POOL_SIZE+1].address,
		              			 pool[MAX_POOL_SIZE+1].streetNumber, pool[MAX_POOL_SIZE+1].odrNumber);
}
