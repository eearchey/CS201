#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "database.h"


Food *addData(char temp[], char tempy[]) {
    //from product file
    //we enter the first food's number
    Food *new = NULL;
    new = (struct Food*)malloc(sizeof(struct Food));
    char* token = strtok(temp, "\t"); 
    new->NDB_number = atoi(token);

    //entering the food's name
    token = strtok(NULL, "\t");
    strcpy(new->name, token);

    //skipping unnecessary data
    token = strtok(NULL, "\t");
    token = strtok(NULL, "\t");

    //entering the manufacturer's name
    token = strtok(NULL, "\t");
    strcpy(new->manufacturer, token);

    //from serving size file
    token = strtok(tempy, "\t");

    //skipping technical serving sizes
    token = strtok(NULL, "\t");
    token = strtok(NULL, "\t");

    //enter household serving sizes
    token = strtok(NULL, "\t");
    new->householdServingSize = atoi(token);
    
    //skip last column
    token = strtok(NULL, "\t");
    strcpy(new->servingSizeUnit, token);

    return new;
}


void createList(char productsFile[], char serving_sizeFile[], char nutrientsFile[]) {
    FILE *food_database;

    foods = fopen(food_database, "r");
    Food *root = NULL;
    Food *cur = NULL;
    root = (struct Food*)malloc(sizeof(struct Food));
    head->next = NULL;

    

    return;
}