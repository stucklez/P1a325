#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGHT 50
#define MAX_POOL_SIZE 10

struct order {
  int status;
  int day, month, year;
  char adresse[MAX_LENGHT];
  int street_number;
  int odr_number;
  char first_name[MAX_LENGHT], last_name[MAX_LENGHT];
};
typedef struct order order;

order make_order(int status, int day, int month, int year, char *adresse, int street_number,
      int odr_number, char *first_name, char *last_name);
void prnt_order(order o);

int main(void){
  order pool[MAX_POOL_SIZE];
  int i;

  pool[1] =
    make_order(1, 22, 11, 1996, "kildevældet", 3, 2211, "Simon", "Mikkelsen");


  for(i = 0; i <=1; i++)
      prnt_order(pool[i]);


  return 0;
}

order make_order(int status, int day, int month, int year, char *adresse, int street_number,
      int odr_number, char *first_name, char *last_name){
  order result;
  result.status = status;
  result.day = day; result.month = month; result.year = year;
  strcpy(result.adresse, adresse); result.street_number = street_number;
  result.odr_number = odr_number;
  strcpy(result.first_name, first_name); strcpy(result.last_name, last_name);
}

void prnt_order(order o){
  printf("Status: %1i\n"
         "Day: %2i / %2i - %4i\n"
         "Adresse: %s %i\n"
         "Name: %s %s\n",
         o.status, o.day, o.month, o.year,
         o.adresse, o.street_number, o.odr_number,
         o.first_name, o.last_name);
}
