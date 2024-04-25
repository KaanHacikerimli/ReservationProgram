#ifndef CUSTOMER_H_INCLUDED
#define CUSTOMER_H_INCLUDED
#include "common.h"

void newOrder(char tableId[],int foodId,int orderNumber);
void UpdateQuantity(char tableId[],int foodId,int newAmount);
void CancelOrder(char tableId[],int foodId);
void PayTheBill(char tableId[]);
#endif // CUSTOMER_H_INCLUDED
