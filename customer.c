#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdbool.h>
#include "customer.h"
extern char *foodsTxt;
extern char *ordersTxt;
extern char *closedOrdersTxt;
extern char *takenOrdersTxt;
extern char *logTtxt;


///tableID'ye göre masanýn olup olmadýðýný kontrol eder
///girilen foodID ile eþleþen yemek var mý diye bakar
///onaylanmamýþ sipariþ olup olmadýðýna bakr ve bu üç duruma göre yeni sipariþi TakenOrders.txt'ye yazar

void newOrder(char tableId[], int foodId, int orderNumber) {
    bool hasFood = false;
    bool hasTable = false;
    bool hasTakenOrder = false;
    char text[200];

    // Check food ID
    FILE *file = fopen("foods.txt", "rb+");
    if (file == NULL) {
        printf("There is no foods.txt file..\n");

    }
    Food currentFood;
    while (fread(&currentFood, sizeof(currentFood), 1, file) == 1) {
        if (currentFood.id == foodId) {
            hasFood = true;
            break;
        }
    }

    fclose(file);

    // Check table
    DIR *dir = opendir(tableId);
    if (dir != NULL) {
        hasTable = true;
        closedir(dir);
    }

//check takenOrder.txt
     FILE *file1 = fopen("takenOrders.txt", "rb+");
     TakenOrder currentOrder;
    int itemsRead;

  while ((itemsRead = fread(&currentOrder, sizeof(currentOrder), 1, file1)) > 0) {
    hasTakenOrder=true;
  }

fclose(file1);
    if (hasFood && hasTable) {

        if (!hasTakenOrder) {
           TakenOrder newOrder ;
            newOrder.f.fee=currentFood.fee;
            newOrder.f.id=foodId;
            sprintf(newOrder.f.name, "%s", currentFood.name);
            sprintf(newOrder.tableId,"%s", tableId);
            newOrder.isActive=true;
            newOrder.isConfirmed=false;
            newOrder.quantity=orderNumber;
            FILE *file2 = fopen("takenOrders.txt", "ab+");
            if (file2 != NULL) {
                fwrite(&newOrder, sizeof(TakenOrder), 1, file2);
                fclose(file2);
                sprintf(text,"\n%s table's new order is created successfully.", tableId);
                writeToLogFile(text);
                printf("New order is created successfully.\n");
            } else {
                printf("Could not create a table.\n");
                sprintf(text,"\n%s table's new order could not be created...", tableId);
                writeToLogFile(text);
            }
        }else{
        printf("We are so busy right now, please try ordering again soon.\n");
        }

}
 else {
        printf("There is no food or table with the given id.\n");
    }

}




///tableID'ye göre masanýn olup olmadýðýný kontrol eder
///girilen foodID ile eþleþen yemek var mý diye bakar
///okunan current TakenOrder objesinin quantity'sine girilen newAmount eþitlenir
///fseek metodu ile cursoru en son okunan takenOrders objesine getirir ve oranýn üzerine current objesini yazar

UpdateQuantity(char tableId[],int foodId,int newAmount){
        DIR *dir;
    char temp[50];
    char text[200];
    bool isCancel = false;

    if ((dir = opendir(tableId)) != NULL) {
        strcpy(temp, tableId);
        strcat(temp, "//");
        strcat(temp, "Orders.txt");

        FILE *file = fopen(temp, "rb+");
        if (file == NULL) {
            printf("Orders.txt file could not open..\n");

        }

        TakenOrder current;
        while (fread(&current, sizeof(current), 1, file) == 1) {
            if (current.f.id == foodId && current.isActive) {
                    printf("%d",newAmount);
                current.quantity=newAmount;
                fseek(file, -sizeof(current), SEEK_CUR);
                fwrite(&current, sizeof(current), 1, file);


                sprintf(text,"\n%s table's amount is updated successfully.",tableId);

                break;
            }
        }
        fclose(file);
        closedir(dir);



        writeToLogFile(text);

    }
}




///tableID'ye göre masanýn olup olmadýðýný kontrol eder
///girilen foodID ile eþleþen yemek var mý diye bakar
///okunan current TakenOrder objesi aktif deðilse bu objenin isActive deðerini false yapar ve
/// fseek metoduyla bulunulan yerin üzerine yazar
void CancelOrder(char tableId[], int foodId) {
    DIR *dir;
    char temp[50];

    char text[200];

    bool isCancel = false;

    if ((dir = opendir(tableId)) != NULL) {
        strcpy(temp, tableId);
        strcat(temp, "//");
        strcat(temp, "Orders.txt");

        FILE *file = fopen(temp, "rb+");
        if (file == NULL) {
            printf("Orders.txt file could not open..\n");

        }

        TakenOrder current;
        fread(&current, sizeof(current), 1, file);
        while (!feof(file)) {
            if (current.f.id == foodId && current.isActive) {
                current.isActive = false;
                fseek(file, -sizeof(current), SEEK_CUR);
                fwrite(&current, sizeof(current), 1, file);
                isCancel = true;
                sprintf(text,"Order is canceled for %s table.\n",tableId);
                writeToLogFile(text);
                break;
            }
            fread(&current, sizeof(current), 1, file);
        }

        fclose(file);
        closedir(dir);

        if (!isCancel) {
            printf("Order not found or already canceled.\n");
        }
    } else {
        printf("Directory could not open.\n");
    }
}
///tableID'ye göre masanýn olup olmadýðýný kontrol eder
///bulunan masanýn Orders.txt dosyasýný açýp içerisindeki tüm aktif sipariþlerin ücretlerini toplayýp ClosedOrders.txt'sine yazar
///ve masanýn orders.txt'sini boþaltýr
void PayTheBill(char tableId[]) {
    DIR *dir ;
    char temp[50];
    char text[200];
    float total = 0.0;

    if((dir = opendir(tableId)) != NULL) {

        strcpy(temp, tableId);
        strcat(temp, "//");
        strcat(temp, "Orders.txt");

        FILE *file = fopen(temp, "rb+");
        if (file == NULL) {
            printf("Cannot open Orders.txt file.\n");
        }

        TakenOrder order;
        fread(&order, sizeof(TakenOrder), 1, file);
        total += order.f.fee;

        while(!feof(file)) {
            fread(&order, sizeof(TakenOrder), 1, file);
            total += order.f.fee;
        }
        fclose(file);
        closedir(dir);

        FILE *closedOrder = fopen("closedOrders.txt", "a+");
        if (closedOrder == NULL) {
            printf("Cannot open closedOrders.txt file.\n");
        }
        fprintf( closedOrder,"%f\n",total);
        fclose(closedOrder);


        FILE *Order=fopen(temp,"w+");
        if(Order!=NULL){
            fwrite("",sizeof(""),1,Order);
        }
        sprintf(text,"The bill is paid for %s table.\n",tableId);
        writeToLogFile(text);
        fclose(Order);
    } else {
        printf("There is no table with the given id.\n");
    }
}
