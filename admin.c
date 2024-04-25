#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdbool.h>
#include "admin.h"
extern char *foodsTxt;
extern char *ordersTxt ;
extern char *closedOrdersTxt ;
extern char *takenOrdersTxt;
extern char *logTtxt ;


void createTable(char tableID[]) {
    DIR *dir;
    if ((dir = opendir(tableID)) == NULL) {
        printf("%s is created successfully...\n", tableID);
        if (mkdir(tableID) == -1) {
         printf("this table could not created..");
        }
        char filePath[200];
        snprintf(filePath, sizeof(filePath), "%s//Orders.txt", tableID);
        FILE *file = fopen(filePath, "ab+");
        if (file == NULL) {
           printf("Orders file could not created..");
        }

        char text[200];
        snprintf(text, sizeof(text), "Table %s is created successfully....\n", tableID);
        writeToLogFile(text);
        fclose(file);
    } else {
        closedir(dir);
        printf("This table already exists.\n");
    }
}

void deleteTables(char tableID[]){

if(opendir(tableID)!=NULL){

    char temp[50];
    strcpy(temp,tableID);
    strcat(temp,"//");
    strcat(temp,"Orders.txt");
    remove(temp);
    rmdir(tableID);
    printf("%s TableId was deleted",tableID);
}else{
printf("there is no table with the given tableID");
}


    char text[200];
        sprintf(text,"%s TableId was deleted",tableID);
        writeToLogFile(text);

}
void CheckNewOrder() {
    FILE *file;
    file = fopen(takenOrdersTxt, "rb");

    if (file != NULL) {
        TakenOrder current;
        fread(&current, sizeof(TakenOrder), 1, file);
        printf("%s",current.f.name);
        if (current.isActive == true) {
            printf("Food ID: %d\n", current.f.id);
            printf("Food name: %s\n", current.f.name);
            printf("Food number : %d\n", current.quantity);
            printf("Order is active? : %s\n", current.isActive ? "Yes" : "No");
            printf("Order is confirmed? : %s\n", current.isConfirmed ? "Yes" : "No");
            printf("Confirm Order? \n Yes : 1 \n No: 0\n");
            printf("Unconfirmed orders will be canceled!!\n");
            printf("Approved orders are assigned to the relevant table!!\n");

            int isConfirm;
            scanf("%d", &isConfirm);

            if (isConfirm == 1) {
                current.isConfirmed = true;
                DIR *dir;
                char *tableID = current.tableId;

                if ((dir = opendir(tableID)) != NULL) {
                    char filePath[200];
                    sprintf(filePath, "%s/Orders.txt", tableID);
                    FILE *file = fopen(filePath, "ab+");
                    fwrite(&current, sizeof(TakenOrder), 1, file);
                    fclose(file);
                    closedir(dir);
                } else {
                    printf("Could not open directory!\n");
                }
            } else if (isConfirm == 0) {
                current.isConfirmed = false;
            } else {
                printf("Invalid input.\n");
            }

            fclose(file);
            file = fopen(takenOrdersTxt, "wb"); // Dosyayı sıfırla
            fclose(file);
        } else {
            printf("No New orders\n");
        }
    } else {
        printf("Error opening file!\n");
    }
}






void DisplayAllBill() {
    FILE *file;
    file = fopen(closedOrdersTxt, "r");

    if (file == NULL) {
        printf("Dosya açılamadı.\n");
        return;
    }

    float current;
    while (fscanf(file, "%f", &current) == 1) {
        printf("TOTAL FEE: %.2f\n", current);
    }

    fclose(file);

    char text[200];
        sprintf(text,"All bill Are displayed...");
        writeToLogFile(text);
}


void updateFoods(float newPrice,int foodID){

 FILE *file;
Food food;
Food food2;
int found = 0;
file = fopen(foodsTxt, "rb+");
if (file == NULL) {
    printf("Dosya Acilirken hata olustu \n");
    return;
}
while (fread(&food, sizeof(Food), 1, file) == 1) {
    printf("Okunan yemek ID: %d\n", food.id);
    if (food.id == foodID) {

        food2.fee=newPrice;
        strcpy(food2.name,food.name);
        food2.id=food.id;
        fseek(file, -sizeof(Food), SEEK_CUR);
        fwrite(&food2, sizeof(Food), 1, file);
        printf("Price has been updated succesfully.\n");
        found = 1;
        break;
    }
}
if (!found) {
    printf("ID'si %d olan yemek bulunamadi.\n", foodID);
}
fclose(file);

char text[200];
sprintf(text,"food has updated ");
writeToLogFile(text);

}
