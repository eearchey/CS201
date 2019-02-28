#ifndef DATABASE_H
#define DATABASE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct Food {
    char name[200];
    int NDB_number;
    char manufacturer[100];
    int householdServingSize;
    char servingSizeUnit[50];

    //nutrients
    int protein;
    int fat;
    int sugars;
    int calories;
    int sodium;

    struct Food *next;
} Food;

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


int main (int argc, char *argv[]) {
    /*if (argc != 4) {
        printf("Usage: productsfile nutrientsfile servingsizefile");
    }*/

    FILE *nutrients;
    FILE *products;
    FILE *serving_size;

    products = fopen(argv[1], "r");
    serving_size = fopen(argv[2], "r");
    Food *head = NULL;
    Food *cur = NULL;
    head = (struct Food*)malloc(sizeof(struct Food));
    cur = (struct Food*)malloc(sizeof(struct Food));
    head->next = NULL;

    //getting the titles of the columns out of the way
    //you can do products and serving sizes at the same time because they line up
    char temp[1000];
    char tempy[1000];
    fgets(temp, 200, products);
    fgets(tempy, 200, serving_size);

    fgets(temp, 1000, products);
    fgets(tempy, 1000, serving_size);
    head = addData(temp, tempy);

    //printf("%d number \n %s name \n %s manufacturer \n", head->NDB_number, head->name, head->manufacturer);
    //printf("%d serving size \n %s serving size unit \n", head->householdServingSize, head->servingSizeUnit);
    

    while (fgets(temp, 1000, products) != NULL) {
        fgets(tempy, 1000, serving_size);
        cur = addData(temp, tempy);
        cur->next = head;
        head = cur;
        //printf("%d number \n %s name \n %s manufacturer \n", head->NDB_number, head->name, head->manufacturer);
        //printf("%d serving size \n %s serving size unit \n", head->householdServingSize, head->servingSizeUnit);
    }

    fclose(products);
    fclose(serving_size);

    return 0;
}

#endif