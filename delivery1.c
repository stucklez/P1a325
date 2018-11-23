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
    make_order(1, 23, 11, 2018, "Kildevaeldet", 3, 2211, "Simon", "Mikkelsen");

  pool[2] =
    make_order(1, 24, 11, 2018, "Ogade", 5, 3122, "Morten", "Morten");

  pool[3] =
    make_order(1, 24, 11, 2018, "Nyhavnsgade", 3, 4321, "Casper", "Munk");

  for(i = 1; i <=3; i++)
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
  return result;
}

void prnt_order(order o){
  printf("Status: %d\nDay: %d / %d - %d\n"
         "Adress: %s %d\nOdr number: %d\n"
         "Name: %s %s\n\n",
         o.status, o.day, o.month, o.year,
         o.adresse, o.street_number, o.odr_number,
         o.first_name, o.last_name);
}
