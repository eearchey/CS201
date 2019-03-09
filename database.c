#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "database.h"


Food *addData(char line[]) {
    //reading from file

    //entering the food's ID number
    Food *new = NULL;
    new = (struct Food*)malloc(sizeof(struct Food));
    char* token = strtok(line, "~"); 
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
    token = strtok(NULL, "\n");
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

    char temp[1000];
    fgets(temp, 1000, foods);
    root = addData(temp);

    Food *cur = NULL;
    cur = (struct Food*)malloc(sizeof(struct Food));
    cur->leftChild = NULL;
    cur->rightChild = NULL;

    Food *checker = NULL;
    checker = (struct Food*)malloc(sizeof(struct Food));
    checker->leftChild = NULL;
    checker->rightChild = NULL;

    //while (fgets(temp, 1000, foods) != NULL) {
    for (int j = 0; j < 10; j++) { //delete later
        cur = addData(fgets(temp, 1000, foods));
        checker = root;
        while (1) {
            if (strcmp(cur->name, checker->name) == 0) {
                if (checker->leftChild == NULL) {
                    checker->leftChild = cur;
                    break;
                }
                else {
                    checker = checker->leftChild;
                }
            }
            else if (cur->name[0] < checker->name[0]) {
                if (checker->leftChild == NULL) {
                    checker->leftChild = cur;
                    break;
                }
                else {
                    checker = checker->leftChild;
                }
            } 
            else if (cur->name[0] > checker->name[0]) {
                if (checker->rightChild == NULL) {
                    checker->rightChild = cur;
                    break;
                }
                else {
                    checker = checker->rightChild;
                }
            }
            else {
                int i = 0;
                while (cur->name[i] == checker->name[i]) {
                    i++;
                }
                if (cur->name[i] < checker->name[i]) {
                    if (checker->leftChild == NULL) {
                        checker->leftChild = cur;
                        break;
                    }
                    else {
                        checker = checker->leftChild;
                    }
                }
                else if (cur->name[i] > checker->name[i]) {
                    if (checker->rightChild == NULL) {
                        checker->rightChild = cur;
                        break;
                    }
                    else {
                        checker = checker->rightChild;
                    }
                }
            } 
        }
    } 

    

    return;
}