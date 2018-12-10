#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>
#define AMOUNT_OF_POINTS 43
#define AMOUNT_OF_CLUSTERS 3
#define POSTOFFICE_LOCATION 0
#define MAX_LEN 50
#define MAX_POOL_SIZE 20

struct order
{
    int status;
    int day, month, year;
    char address[MAX_LEN];
    int streetNumber;
    int odrNumber;
    char firstName[MAX_LEN], lastName[MAX_LEN];
};
typedef struct order order;

struct points
{
    char name[50];
    int x, y;
    double distantToEnd;
    int status;
};
typedef struct points points;

/*void prntOrder(order o);
order makeOrder(int status, int day, int month, 
                 int year, char *address, int streetNumber, 
				 int odrNumber, char *firstName, char *lastName);*/

int main(void)
{
    order pool[MAX_POOL_SIZE];
    int i, r = 0;
    /*tmpDay, tmpMonth, tmpYear, tmpStreetNumber = 0, odrNumber = 0;
    char tmpFirstName[MAX_LEN], tmpLastName[MAX_LEN], tmpAddress[MAX_LEN];*/

    int adminInput = 0;
    int fileLoaded = 0;

    while (adminInput != 4)
    {
        adminInput = 0;
        printf(" Choose an option below: \n\n");
        printf(" [1] View Current pool\n [2] Create manual orders\n [3] Load from external file\n [0] Exit program\n");
        if (fileLoaded)
        {
            printf(" [4] Continue\n\n");
        }
        printf(" Input: ");
        scanf(" %d", &adminInput);

        //exit the program
        if (adminInput == 0)
        {
            system("cls");
            printf(" Program closed\n\n");
            exit(0);
        }

        else if (adminInput < 0 || adminInput > 4)
        {
            system("cls");
            printf(" Invalid input\n\n");
        }
        //View current pool
        if (adminInput == 1)
        {
            system("cls");
            printf(" view current pool here\n\n");
        }

        //Manual orders
        else if (adminInput == 2)
        {
            system("cls");
            printf(" Create manual orders\n\n");
            printf(" [1] Create new list\n [2] Add to existing list\n\n");
            scanf(" %d", &adminInput);
            switch (adminInput)
            {
            case 1:
                printf(" FUNCTION HERE\n\n");
                fileLoaded = 1;
                break;
            case 2:
                printf(" FUNCTION HERE\n\n");
                fileLoaded = 1;
                break;
            default:
                printf(" Error\n\n");
                fileLoaded = 1;
                break;
            }
        } //load from external file
        else if (adminInput == 3)
        {
            system("cls");
            printf(" Load from external file\n\n");
            FILE *orderlist;
            orderlist = fopen("orderlist.txt", "r");
            fileLoaded = 1;

            if (orderlist != NULL)
            {
                printf(" <File loaded>\n\n");
            }
            else
            {
                printf(" <Error loading file>\n\n");
            }
            printf("         Name                        Address        Order\n");
            printf(" __________________________________________________________\n");

            for (i = 0; i != MAX_POOL_SIZE; i++)
            {
                fscanf(orderlist, "%s %s - %s %d - %d",
                       pool[i].firstName, pool[i].lastName, pool[i].address,
                       &pool[i].streetNumber, &pool[i].odrNumber);

                printf(" %-12s %-12s | %-15s %-5d | %-5d|\n",
                       pool[i].firstName, pool[i].lastName, pool[i].address,
                       pool[i].streetNumber, pool[i].odrNumber);
            }
            printf("\n");
            fclose(orderlist);

        } //continue to route generator
        else if (adminInput == 4)
        {
            break;
        }
    }

    system("cls");
    printf(" <<<<<<FUNCTION: ROUTE GENERATING>>>>>>\n\n");

    return 0;
}
/*
void prntOrder(order o){
  printf(" Status: %d\n Day: %d / %d - %d\n"
         " Adress: %s %d\n Odr number: %d\n"
         " Name: %s %s\n\n",
         o.status, o.day, o.month, o.year,
         o.address, o.streetNumber, o.odrNumber,
         o.firstName, o.lastName);*/
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

int main(void)
{
    points point[AMOUNT_OF_POINTS];
    int i, cx = 0, cy = 0, min = 0, start = 0, end = 0, endpoints = 0, cluster = 1;

    //Husk at have filen map.txt
    FILE *map;
    map = fopen("clusterlist.txt", "r");

    if (map != NULL)
    {
        printf("File loaded\n");

        for (i = 0; i <= AMOUNT_OF_POINTS - 1; i++)
        {
            fscanf(map, "%s (%d, %d) %d", point[i].name, &point[i].x, &point[i].y, &point[i].status);
        }
    }
    else
    {
        printf("Error\n");
    }

    start = POSTOFFICE_LOCATION;

    //Finder det end point der er taettest på startpunktet.
    for (int j = 0; cluster <= AMOUNT_OF_CLUSTERS; j++)
    {
        min = 999;
        endpoints = 0;
        for (i = 0; i <= AMOUNT_OF_POINTS - 1; i++)
        {
            if (point[i].status == cluster)
            {
                point[i].distantToEnd = sqrt(pow(point[i].x - point[start].x, 2) + pow(point[i].y - point[start].y, 2));
                endpoints++;
                if (point[i].distantToEnd <= min)
                {
                    min = point[i].distantToEnd;
                    end = i;
                }
            }
        }
        //Hvis der ikke er flere endpoints gaar den tilbage til postoffice.
        if (cluster == AMOUNT_OF_CLUSTERS && endpoints == 0)
        {
            end = POSTOFFICE_LOCATION;
            endpoints++;
            cluster++;
        }

        if (endpoints == 0)
        {
            cluster++;
        }
        else
        {
            //location til start og slut punkt er lagt i variablerne start og end.
            printf("#%-2d Cluster: %-2d Start: %-15s End: %-15s Lenght: %lf\n", j + 1, cluster, point[start].name, point[end].name, point[end].distantToEnd);

            //Alle punkter faar nu en afstand til det endpoint vi lige har fundet.
            for (i = 0; i <= AMOUNT_OF_POINTS - 1; i++)
                point[i].distantToEnd = sqrt(pow(point[i].x - point[end].x, 2) + pow(point[i].y - point[end].y, 2));

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
