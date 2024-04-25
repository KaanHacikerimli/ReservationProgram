#include <stdio.h>
#include <stdlib.h>
#include "admin.h"
int main()
{
    int selection, amount, foodId;
    char tableId[250];
    float fee;
    do{
        printf("Display food menu (1)\n");
        printf("Create table (2)\n");
        printf("Delete table (3)\n");
        printf("Check new order (4)\n");
        printf("Show order list of table (5)\n");
        printf("Show all invoices (6)\n");
        printf("Update food (7)\n");
        printf("Exit (8)\n");
        printf("Enter your selection : \n");
        scanf("%d",&selection);
        switch(selection){
case 1:
    displayMenu();
    break;
case 2:
   printf("Enter TableID for create : ");
   scanf("%s",&tableId);
    createTable(tableId);
    break;
case 3:
    printf("Enter TableID for delete : ");
   scanf("%s",&tableId);
deleteTables(tableId);
    break;
case 4:
 CheckNewOrder();
    break;
case 5:
printf("Enter table id\n");
scanf("%s",&tableId);
displayOrderList(tableId);
    break;
    case 6:
DisplayAllBill();
    break;
    case 7:
        printf("Enter new Price :\n");
        float newPrice;
        scanf("%f",&newPrice);
        printf("Enter foodID \n");
        int foodId;
        scanf("%d",&foodId);
        updateFoods(newPrice,foodId);
    break;
        }
    }while(selection !=8);
    return 0;
}


