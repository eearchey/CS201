#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "database.h"


Food *addData(char temp[]) {
    //reading from file

    //entering the food's ID number
    Food *new = NULL;
    new = (struct Food*)malloc(sizeof(struct Food));
    char* token = strtok(temp, "~"); 
    new->ID = atoi(token);

    //entering the food's name
    token = strtok(NULL, "~");
    strcpy(new->name, token);

    //entering the manufacturer's name
    token = strtok(NULL, "~");
    strcpy(new->manufacturer, token);

    //entering the amount of calories
    token = strtok(NULL, "~");
    new->calories = atof(token);

    //entering the amount of carbs
    token = strtok(NULL, "~");
    new->carbs = atof(token);

    //entering the amount of fat
    token = strtok(NULL, "~");
    new->fat = atof(token);

    //entering the amount of protein
    token = strtok(NULL, "~");
    new->protein = atof(token);

    //entering the serving size (g or mL)
    token = strtok(NULL, "~");
    new->serving_size = atof(token);

    //checking if serving size is in g or mL
    token = strtok(NULL, "~");
    if (strcmp(token, "g") == 0) {
        new->isGrams = true;
    }
    else {
        new->isGrams = false;
    }

    //entering the household serving size
    token = strtok(NULL, "~");
    new->house_serving_size = atof(token);

     //entering the household serving size's unit
    token = strtok(NULL, "~");
    strcpy(new->house_serving_size_unit, token);

    return new;
}


void createTree(char file[]) {
    FILE *foods;

    foods = fopen(file, "r");
    Food *root = NULL;
    root = (struct Food*)malloc(sizeof(struct Food));
    root->leftChild = NULL;
    root->rightChild = NULL;

    

    return;
}