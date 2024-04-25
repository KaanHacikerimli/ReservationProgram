#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED
#include <stdbool.h>

typedef struct {
int id;
char name[50];
float fee;
}Food;

typedef struct{
Food f;
int quantity;
char tableId[250];
bool isConfirmed;
bool isActive;
}TakenOrder;

void displayMenu();
void displayOrderList(char tableId[]);
void writeToLogFile(char text[]);


#endif // COMMON_H_INCLUDED
