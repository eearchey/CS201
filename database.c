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

Food *getDataFromPrevJournal(Food *cur, char name[], FILE *prevDiary) {
    char throwaway[50];
    strncpy(cur->name, name, strlen(name) - 1);

    fscanf(prevDiary, "%[^\n]%*c", cur->manufacturer);

    fscanf(prevDiary, "%s", throwaway);
    fscanf(prevDiary, "%f", &cur->calories);
    fscanf(prevDiary, "%s", throwaway);

    fscanf(prevDiary, "%s", throwaway);
    fscanf(prevDiary, "%f", &cur->carbs);
    fscanf(prevDiary, "%s", throwaway);

    fscanf(prevDiary, "%s", throwaway);
    fscanf(prevDiary, "%f", &cur->fat);
    fscanf(prevDiary, "%s", throwaway);

    fscanf(prevDiary, "%s", throwaway);
    fscanf(prevDiary, "%f", &cur->protein);
    fscanf(prevDiary, "%s", throwaway);

    fscanf(prevDiary, "%s", throwaway);
    fscanf(prevDiary, "%s", throwaway);
    fscanf(prevDiary, "%f%*c", &cur->serving_size);
    fscanf(prevDiary, "%s", throwaway);

    fscanf(prevDiary, "%s", throwaway);
    fscanf(prevDiary, "%s", throwaway);
    fscanf(prevDiary, "%s", throwaway);
    fscanf(prevDiary, "%f%*c", &cur->house_serving_size);
    fscanf(prevDiary, "%s%*c", cur->house_serving_size_unit);
    return cur;
}

Food *readFromPrevJournal(Food *first, char filename[]) {
    FILE *prevDiary = fopen(filename, "r");
    Food *cur = (struct Food*)malloc(sizeof(struct Food));
    char temp[1000];
    //getting breakfast header
    fgets(temp, 10000, prevDiary);

    //now reading data
    while (strcmp(fgets(temp, 10000, prevDiary), "\n") != 0) {
        cur = getDataFromPrevJournal(cur, temp, prevDiary);
        strcpy(cur->meal, "breakfast");
        cur->nextFood = first;
        first = cur;
        fgets(temp, 10000, prevDiary);
    }

    //lunch header
    fgets(temp, 1000, prevDiary);
    //getting lunch
    while (strcmp(fgets(temp, 10000, prevDiary), "\n") != 0) {
        cur = getDataFromPrevJournal(cur, temp, prevDiary);
        strcpy(cur->meal, "lunch");
        cur->nextFood = first;
        first = cur;
        fgets(temp, 10000, prevDiary);
    }

    //dinner header
    fgets(temp, 1000, prevDiary);
    //getting dinner
    while (strcmp(fgets(temp, 10000, prevDiary), "\n") != 0) {
        cur = getDataFromPrevJournal(cur, temp, prevDiary);
        strcpy(cur->meal, "dinner");
        cur->nextFood = first;
        first = cur;
        fgets(temp, 10000, prevDiary);
    }

    //snacks header
    fgets(temp, 1000, prevDiary);
    //getting snacks
    while (fgets(temp, 10000, prevDiary) != NULL) {
        cur = getDataFromPrevJournal(cur, temp, prevDiary);
        strcpy(cur->meal, "snack");
        cur->nextFood = first;
        first = cur;
        fgets(temp, 10000, prevDiary);
    }

    return first;
}


void writeToLog(char filename[], Food* head) {
    FILE *journal = fopen(filename, "w+");
    Food *current;
    current = (struct Food*)malloc(sizeof(struct Food));
    current = head;

    fprintf(journal, "Breakfast:\n");
    while (current != NULL) {
        if (strcmp(current->meal, "breakfast") == 0) {
            fprintf(journal, "%s\n", current->name);
            fprintf(journal, "%s\n", current->manufacturer);
            fprintf(journal, "Calories: %g kcal\n", current->calories);
            fprintf(journal, "Carbs: %g grams\n", current->carbs);
            fprintf(journal, "Fat: %g grams\n", current->fat);
            fprintf(journal, "Protein: %g grams\n", current->protein);
            if (current->isGrams == true) {
                fprintf(journal, "Serving Size: %g grams\n", current->serving_size);
            }
            else {
                fprintf(journal, "Serving Size: %g mL\n", current->serving_size);
            }
            fprintf(journal, "Household Serving Size: %g %s\n", current->house_serving_size, current->house_serving_size_unit);
        }
        current = current->nextFood;
    }

    current = head;
    fprintf(journal, "\nLunch:\n");
    while(current != NULL) {
        if (strcmp(current->meal, "lunch") == 0) {
            fprintf(journal, "%s\n", current->name);
            fprintf(journal, "%s\n", current->manufacturer);
            fprintf(journal, "Calories: %g kcal\n", current->calories);
            fprintf(journal, "Carbs: %g grams\n", current->carbs);
            fprintf(journal, "Fat: %g grams\n", current->fat);
            fprintf(journal, "Protein: %g grams\n", current->protein);
            if (current->isGrams == true) {
                fprintf(journal, "Serving Size: %g grams\n", current->serving_size);
            }
            else {
                fprintf(journal, "Serving Size: %g mL\n", current->serving_size);
            }
            fprintf(journal, "Household Serving Size: %g %s\n", current->house_serving_size, current->house_serving_size_unit);
        }
        current = current->nextFood;
    }

    current = head;
    fprintf(journal, "\nDinner:\n");
    while(current != NULL) {
        if (strcmp(current->meal, "dinner") == 0) {
            fprintf(journal, "%s\n", current->name);
            fprintf(journal, "%s\n", current->manufacturer);
            fprintf(journal, "Calories: %g kcal\n", current->calories);
            fprintf(journal, "Carbs: %g grams\n", current->carbs);
            fprintf(journal, "Fat: %g grams\n", current->fat);
            fprintf(journal, "Protein: %g grams\n", current->protein);
            if (current->isGrams == true) {
                fprintf(journal, "Serving Size: %g grams\n", current->serving_size);
            }
            else {
                fprintf(journal, "Serving Size: %g mL\n", current->serving_size);
            }
            fprintf(journal, "Household Serving Size: %g %s\n", current->house_serving_size, current->house_serving_size_unit);
        }
        current = current->nextFood;
    }

    current = head;
    fprintf(journal, "\nSnacks:\n");
    while(current != NULL) {
        if (strcmp(current->meal, "snack") == 0) {
            fprintf(journal, "%s\n", current->name);
            fprintf(journal, "%s\n", current->manufacturer);
            fprintf(journal, "Calories: %g kcal\n", current->calories);
            fprintf(journal, "Carbs: %g grams\n", current->carbs);
            fprintf(journal, "Fat: %g grams\n", current->fat);
            fprintf(journal, "Protein: %g grams\n", current->protein);
            if (current->isGrams == true) {
                fprintf(journal, "Serving Size: %g grams\n", current->serving_size);
            }
            else {
                fprintf(journal, "Serving Size: %g mL\n", current->serving_size);
            }
            fprintf(journal, "Household Serving Size: %g %s\n", current->house_serving_size, current->house_serving_size_unit);
        }
        current = current->nextFood;
    }
    fclose(journal);
    return;
}

Food *addEntry(Food *cur, char food[], char brand[]) {
    if (cur == NULL) {
        printf("Food not found\n");
        return NULL;
    }

    else if ((strcasestr(cur->name, food) != NULL) && (strcasestr(cur->manufacturer, brand) != NULL)) {
        return cur;
    }

    if (strcasecmp(food, cur->name) == 0) {
        cur = addEntry(cur->leftChild, food, brand);
    }

    else if (isupper(cur->name[0])) {
        int i = 0;
        if (toupper(food[0]) == cur->name[0]) {
            while (i < strlen(cur->name) && i < strlen(food)) {
                    if (cur->name[i] != toupper(food[i])) {
                        break;
                    }
                    i++;
            }
        }
        if (toupper(food[i]) < cur->name[i]) {
            cur = addEntry(cur->leftChild, food, brand);
        }
        else {
            cur = addEntry(cur->rightChild, food, brand);
        }
    }
    else {
        int j = 0;
        if ((food[0]) == cur->name[0]) {
            while (j < strlen(cur->name) && j < strlen(food)) {
                    if (cur->name[j] != food[j]) {
                        break;
                    }
                    j++;
                }
        }
        if ((food[j]) < cur->name[j]) {
            cur = addEntry(cur->leftChild, food, brand);
        }
        else {
            cur = addEntry(cur->rightChild, food, brand);
        }
    }
    return cur;
}

void search(Food *cur, char food[], char brand[]) {
    if (cur == NULL) {
        return;
    }
    else if (strcmp(brand, "unknown") == 0) {
            if (strcasestr(cur->name, food) != NULL) {
            printf("Food: %s, Brand: %s\n", cur->name, cur->manufacturer);
            }
    }
    
    else if ((strcasestr(cur->name, food) != NULL) && (strcasestr(cur->manufacturer, brand) != NULL)) {
        printf("Food: %s, Brand: %s\n", cur->name, cur->manufacturer);
    }
    search(cur->leftChild, food, brand);
    search(cur->rightChild, food, brand);
}

//this function does the actual editing of the journal for each person
void editJournal(char name[], Food* root) {

    //creating the name of the .log file
    FILE* journal;
    char filename[50];
    strcpy(filename, name);
    strcat(filename, ".log");
    char yn[10];
    char temp[1000];
    Food *head = NULL;
    head = (struct Food*)malloc(sizeof(struct Food));
    head->leftChild = head->rightChild = NULL;
    Food *cur = NULL;
    cur = (struct Food*)malloc(sizeof(struct Food));
    cur->leftChild = cur->rightChild = NULL;
    
    journal = fopen(filename, "r");
    if (journal != NULL) {
        fclose(journal);
        printf("Is this your diary?\n");
        printf("%s\n", filename);
        scanf("%[^\n]%*c", yn);
        if (strcasecmp(yn, "yes") == 0) {
            head = readFromPrevJournal(head, filename);
        }
    }
    
    //opening choice menu
    printf("\nHi %s!\n", name);
    printf("Here are your options:\nView diary\nSearch for food\nAdd entry\nUpdate entry\nDelete entry\nQuit\n");
    printf("Enter your choice: ");
    char choice[50];
    scanf("%[^\n]%*c", choice);

    while (1) {
        //view diary
        if (strcasestr(choice, "view") != NULL) {
            journal = fopen(filename, "r");
            if (journal == NULL) {
                printf("\nDiary is empty.\n");
                printf("\nHere are your options:\nView diary\nSearch for food\nUpdate entry\nDelete entry\nQuit\n");
                printf("Enter your choice: ");
                scanf("%s%*c", choice);
                continue;
            }
            else {
                printf("\n");
                while (fgets(temp, 10000, journal) != NULL) {
                    printf("%s", temp);
                }
            }
        }

        //search foods
        else if (strcasestr(choice, "search") != NULL) {
            char food[1000];
            char brand[1000];
            printf("Do you know the brand?\n");
            scanf("%s%*c", yn);
            if (strcasecmp(yn, "yes") == 0) {
                printf("What is the brand name?\n");
                scanf("%[^\n]%*c", brand);
                printf("What did you eat?\n");
                scanf("%[^\n]%*c", food);
                printf("\n");
                search(root, food, brand);
            }
            else {
                printf("What did you eat?\n");
                scanf("%[^\n]%*c", food);
                printf("\n");
                search(root, food, "unknown");
            }
        }

        //add entry to diary
        else if (strcasestr(choice, "add") != NULL) {
            printf("Enter the exact name of the food and the brand (if you don't know them, you can use the search function).\n");
            printf("Do you know the name and brand of the food? Enter yes or no (no will return you to the choice selection).\n");
            char understood[10];
            scanf("%s%*c", understood);

            if (strcasecmp(understood, "yes") != 0) {
                printf("\nHere are your options:\nView diary\nSearch for food\nUpdate entry\nDelete entry\nQuit\n");
                printf("Enter your choice: ");
                scanf("%s%*c", choice);
                continue;
            }
        
            printf("Food name: ");
            char foodName[1000];
            scanf("%[^\n]%*c", foodName);
            printf("Brand name: ");
            char brandName[1000];
            scanf("%[^\n]%*c", brandName);

            Food *newEntry = NULL;
            newEntry = (struct Food*)malloc(sizeof(struct Food));
            newEntry->nextFood = NULL;
            cur = addEntry(root, foodName, brandName);

            newEntry->ID = cur->ID;
            strcpy(newEntry->name, cur->name);
            strcpy(newEntry->manufacturer, cur->manufacturer);
            newEntry->calories = cur->calories;
            newEntry->carbs = cur->carbs;
            newEntry->fat = cur->fat;
            newEntry->protein = cur->protein;
            newEntry->serving_size = cur->serving_size;
            newEntry->isGrams = cur->isGrams;
            newEntry->house_serving_size = cur->house_serving_size;
            strcpy(newEntry->house_serving_size_unit, cur->house_serving_size_unit);
            newEntry->leftChild = newEntry->rightChild = NULL;
            
            if (newEntry == NULL) {
                printf("\nHere are your options:\nView diary\nSearch for food\nUpdate entry\nDelete entry\nQuit\n");
                printf("Enter your choice: ");
                scanf("%s", choice);
                continue;
            }

            newEntry->nextFood = head;
            head = newEntry;

            printf("Did you eat this for breakfast, lunch, dinner, or a snack?\n");
            char meal[40];
            scanf("%s%*c", meal);
            if (strcasecmp(meal, "breakfast") == 0) {
                strcpy(head->meal, "breakfast");
            }

            else if (strcasecmp(meal, "lunch") == 0) {
                strcpy(head->meal, "lunch");
            }
            else if (strcasecmp(meal, "dinner") == 0) {
                strcpy(head->meal, "dinner");
            }
            else {
                strcpy(head->meal, "snack");
            }

            writeToLog(filename, head);

        }

        //update previous entry
        else if (strcasestr(choice, "update") != NULL) {
            fprintf(journal, "You've chosen to update!\n");
        }

        //delete entry
        else if (strcasestr(choice, "delete") != NULL) {
            fprintf(journal, "You've chosen to delete!\n");
        }

        //quit
        else if (strcasestr(choice, "quit") != NULL) {
            //free(head);
            break;
        }

        //if they entered an invalid option
        else {
            printf("Sorry, that is not an available option. Please choose to view, search, update, delete, or quit.\n");
        }

        printf("\nHere are your options:\nView diary\nSearch for food\nUpdate entry\nDelete entry\nQuit\n");
        printf("Enter your choice: ");
        scanf("%s", choice);
    }

    return;
}