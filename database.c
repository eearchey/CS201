#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "database.h"


Food *addData(Food *new, char line[]) {
    bool noServingSize;
    if (strstr(line, "~~") != NULL) {
        noServingSize = true;
    }
    //entering the food's ID number
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

    //seeing if there are two tildes
    if (noServingSize == true) {
        new->house_serving_size = 0;
        strcpy(new->house_serving_size_unit, "none");
        return new;
    }

    //entering the household serving size
    token = strtok(NULL, "~");
    new->house_serving_size = atof(token);

     //entering the household serving size's unit
    token = strtok(NULL, "~");
    strcpy(new->house_serving_size_unit, token);

    return new;
}


//this function creates the binary search tree that holds the information for each food item
void createTree(char file[], Food *root) {
    //starts reading from food_database.csv
    FILE *foods;
    foods = fopen(file, "r");

    //creates the root
    char temp[10000];
    fgets(temp, 10000, foods);
    addData(root, temp);

    //this creates the checker node, which is used to see where in the tree the item should be placed, as the tree is sorted by alphabetical order
    Food *checker = NULL;
    checker = (struct Food*)malloc(sizeof(struct Food));
    checker->leftChild = checker->rightChild = NULL;

    //adds nodes to the tree
    while (fgets(temp, 10000, foods) != NULL) {
        fgets(temp, 10000, foods);
        Food *cur = NULL;
        cur = (struct Food*)malloc(sizeof(struct Food));
        cur->leftChild = cur->rightChild = NULL;
        addData(cur, temp);
        checker = root;
        while (1) {
            if ((strcmp(cur->name, checker->name) == 0) || (cur->name[0] < checker->name[0])) {
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
                while (i < strlen(cur->name)) {
                    if (cur->name[i] != checker->name[i]) {
                        break;
                    }
                    i++;
                }
                if (i == strlen(cur->name) || cur->name[i] < checker->name[i]) {
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

    //closing files
    free(checker);
    fclose(foods);
    return;
}

Food *addEntry(Food *newEntry, char food[1000]) {
    return newEntry;
}

void search(Food *cur, char food[]) {
    if (cur == NULL) {
        return;
    }
    else if (strcasestr(food, cur->name) != NULL) {
        printf("Food: %s, Brand: %s\n", cur->name, cur->manufacturer);
    }
    search(cur->leftChild, food);
    search(cur->rightChild, food);
}

//this function does the actual editing of the journal for each person
void editJournal(char name[], Food* root) {

    //creating and opening "name.log" file, which contains the journal for each individual person
    FILE* journal;
    char filename[50];
    strcpy(filename, name);
    strcat(filename, ".log");
    journal = fopen(filename, "w");

    printf("\nHi %s!\n", name);
    printf("Here are your options:\nView diary\nSearch for food\nAdd entry\nUpdate entry\nDelete entry\nQuit\n");
    printf("Enter your choice: ");
    char choice[50];
    scanf("%s", choice);
    scanf("%[^\n]s", choice);

    while (1) {
        if (strcasestr(choice, "view") != NULL) {
            fprintf(journal, "You've chosen to view!\n");
        }
        else if (strcasestr(choice, "search") != NULL) {
            char food[1000];
            printf("What did you eat?\n");
            scanf("%s", food);
            scanf("%[^\n]s", food);
            printf("\n");
            search(root, food);
        }
        else if (strcasestr(choice, "add") != NULL) {
            fprintf(journal, "You've chosen to add!\n");
        }
        else if (strcasestr(choice, "update") != NULL) {
            fprintf(journal, "You've chosen to update!\n");
        }
        else if (strcasestr(choice, "delete") != NULL) {
            fprintf(journal, "You've chosen to delete!\n");
        }
        else if (strcasestr(choice, "quit") != NULL) {
            break;
        }
        else {
            printf("Sorry, that is not an available option. Please choose to view, search, update, delete, or quit.\n");
        }

        printf("\nHere are your options:\nView diary\nSearch for food\nUpdate entry\nDelete entry\nQuit\n");
        printf("Enter your choice: ");
        scanf("%s", choice);
    }

    return;
}