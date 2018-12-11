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
/*order makeOrder(int status, int day, int month, 
                 int year, char *address, int streetNumber, 
				 int odrNumber, char *firstName, char *lastName);*/

int main(void){
	order pool[MAX_POOL_SIZE];
	userDialog(pool);
	
	
	
	return 0;

}

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

/*order makeOrder(int status, int day, int month, 
                 int year, char *address, int streetNumber, 
				 int odrNumber, char *firstName, char *lastName){
  order result;
  result.status = status;
  result.day = day; result.month = month; result.year = year;
  strcpy(result.address, address); result.streetNumber = streetNumber;
  result.odrNumber = odrNumber;
  strcpy(result.firstName, firstName); strcpy(result.lastName, lastName);
  return result;
}
*/

void userDialog (order *pool) {
	int i, j, r = 0; 
		/*tmpDay, tmpMonth, tmpYear, tmpStreetNumber = 0, odrNumber = 0;
	    char tmpFirstName[MAX_LEN], tmpLastName[MAX_LEN], tmpAddress[MAX_LEN];*/
		int usrNumber = 0; 
		int firstInput = 0;
		int adminInput = 0;
		int userInput = 0;
		int userOrderNr = 0;
		int fileLoaded = 0; 

		
		/* Prompt for om der skal gives adgang til bruger eller admin dialog*/

		printf("Admin [1]\nBruger [2]\n");
		scanf("%d", &firstInput);
		/* Bruger dialog: Først bliver der promptet for ordrenumret, så ordren kan findes i den indlæste data.*/
		if (firstInput == 2) {
			printf("-----BRUGERDIALOG-----\n\nIndtast Ordrenummer\n");
			scanf("%d", &userOrderNr);
			for (j = 0; j < MAX_POOL_SIZE; j++) {
				if (pool[j].odrNumber == userOrderNr) {
					usrNumber = j;
				}
			}
		/* Brugerens pakke blive vist med tilhørende leverings information (Estimeret tid) og brugeren bliver spurgt om de kan modtage pakken*/
			printf("Din pakke bliver leveret i tidsrummet\n\nbla til bla\n\nKan du modtage den der? Hvis JA [1] --- Hvis NEJ [2]\n");
			scanf("%d", &userInput);
			if (userInput == 1) {
				pool[usrNumber].status = 1;
			}
			if (userInput == 2) {
				pool[userOrderNr].status = 2;
			}
			if (userInput != 1 || userInput != 2) {
				printf("Fuck dig prøv igen\n");
			}


		}
		else {
		while(adminInput != 4){
			adminInput = 0;
			printf(" Choose an option below: \n\n");
			printf(" [1] View Current pool\n [2] Create manual orders\n [3] See content from external file\n [0] Exit program\n");
			if(fileLoaded){
			printf(" [4] Continue\n\n");
		}
			printf(" Input: ");
			scanf(" %d", &adminInput);
		
			//exit the program
			if(adminInput == 0){
			system("clear");
			printf(" Program closed\n\n");
			exit (0);
		}
		
			else if(adminInput < 0 || adminInput > 4){
			system("clear");
			printf(" Invalid input\n\n");
			
		}
				//View current pool
				if(adminInput == 1){

				//system("cls") eller system("clear"). hvad der virker
				system("clear");
				printf(" View current pool here\n\n");
				prntOrder(pool);
				}
			
			//Manual orders
			else if(adminInput == 2){
				system("clear");
				printf(" Create manual orders\n\n");
				printf(" [1] Create new list\n [2] Add to existing list\n\n");
				scanf(" %d", &adminInput);
				switch(adminInput){
					case 1: printf(" FUNCTION HERE\n\n"); fileLoaded = 1; break;
					case 2: printf(" FUNCTION HERE\n\n"); fileLoaded = 1; break;
					default: printf(" Error\n\n"); fileLoaded = 1; break;
				}
			} //load from external file
			else if(adminInput == 3){
				readFile(pool, fileLoaded);
				
			}//continue to route generator
			else if(adminInput == 4){
				break;
			}
		}
		
		
		system("clear");
		printf(" <<<<<<FUNCTION: ROUTE GENERATING>>>>>>\n\n");
}
}

void readFile (order *pool, int *fileLoaded) {
	int i;
			system("clear");
			printf(" Load from external file\n\n");
			FILE *orderlist;
			orderlist = fopen("orderlist.txt", "r");
			fileLoaded = 1;
			
			if (orderlist != NULL){
				printf(" <File loaded>\n\n");
			}	
			else{
				printf(" <Error loading file>\n\n");
			}
			printf("         Name                        Address        Order\n");
			printf(" __________________________________________________________\n");
			
			for(i = 0; i != MAX_POOL_SIZE; i++){
				fscanf(orderlist,"%s %s - %s %d - %d", 
				   pool[i].firstName, pool[i].lastName, pool[i].address, 
	              &pool[i].streetNumber, &pool[i].odrNumber);
									   
			printf(" %-12s %-12s | %-15s %-5d | %-5d|\n", 
			       pool[i].firstName, pool[i].lastName, pool[i].address,
            	   pool[i].streetNumber, pool[i].odrNumber);
			}
			printf("\n");
			fclose(orderlist);
			}