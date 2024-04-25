#ifndef ADMIN_H_INCLUDED
#define ADMIN_H_INCLUDED
#include "common.h"
void createTable(char albumName[]);
void deleteTables(char tableID[]);
void CheckNewOrder();
void DisplayAllBill();
void updateFoods(float newPrice,int foodID);
#endif // ADMIN_H_INCLUDED

