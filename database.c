#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "database.h"

//this function gets the data from food_database.csv and creates a node with all of the information, then returns it
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
    strncpy(new->name, token, 500);   

    //entering the manufacturer's name
    token = strtok(NULL, "~");
    strncpy(new->manufacturer, token, 500);

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
    strncpy(new->house_serving_size_unit, token, 1000);
    new->house_serving_size_unit[strlen(new->house_serving_size_unit) - 1] = 0;

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
            //the different if/else statements determine where the node should go in the binary search tree, based on alphabetical order of the first word
            //if the names are exactly the same or if the first letter of the new node is before that of the first letter of the current node, go down to the left child or insert the new node as the left child
            if ((strcasecmp(cur->name, checker->name) == 0) || toupper(cur->name[0]) < toupper(checker->name[0])) {
                if (checker->leftChild == NULL) {
                    checker->leftChild = cur;
                    break;
                }
                else {
                    checker = checker->leftChild;
                }
            } 
            //if the first letter of the new node is after that of the first letter of the current node, go down to the right child or insert the new node as the right child
            else if (toupper(cur->name[0]) > toupper(checker->name[0])) {
                if (checker->rightChild == NULL) {
                    checker->rightChild = cur;
                    break;
                }
                else {
                    checker = checker->rightChild;
                }
            }
            //if the first or first few letters are the same but the whole word is different, go down the word until you get to a letter that differs, then add in the same manner as above
            else {
                int i = 0;
                if (strlen(cur->name) <= strlen(checker->name)) {
                    while (i < strlen(cur->name)) {
                        if (toupper(cur->name[i]) != toupper(checker->name[i])) {
                            break;
                        }
                        i++;
                    }
                    if (i == strlen(cur->name) || toupper(cur->name[i]) < toupper(checker->name[i])) {
                        if (checker->leftChild == NULL) {
                            checker->leftChild = cur;
                            break;
                        }
                        else {
                            checker = checker->leftChild;
                        }
                    }
                    else if (toupper(cur->name[i]) > toupper(checker->name[i])) {
                        if (checker->rightChild == NULL) {
                            checker->rightChild = cur;
                            break;
                        }
                        else {
                            checker = checker->rightChild;
                        }
                    }
                }
                else {
                    while (i < strlen(checker->name)) {
                        if (toupper(cur->name[i]) != toupper(checker->name[i])) {
                            break;
                        }
                        i++;
                    }
                    if (i == strlen(checker->name) || toupper(cur->name[i]) < toupper(checker->name[i])) {
                        if (checker->leftChild == NULL) {
                            checker->leftChild = cur;
                            break;
                        }
                        else {
                            checker = checker->leftChild;
                        }
                    }
                    else if (toupper(cur->name[i]) > toupper(checker->name[i])) {
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
    }

    //closing files and freeing the checker node
    free(checker);
    fclose(foods);
    return;
}

//this function reads in the data from a previous journal, throws away words that aren't required, and adds them to a new node that it returns
Food *getDataFromPrevJournal(Food *cur, char name[], FILE *prevDiary) {
    char throwaway[50];
    //getting the name
    strncpy(cur->name, name, strlen(name) - 1);

    //getting manufacturer
    fscanf(prevDiary, "%[^\n]%*c", cur->manufacturer);

    //getting the calories
    fscanf(prevDiary, "%s", throwaway);
    fscanf(prevDiary, "%f", &cur->calories);
    fscanf(prevDiary, "%s", throwaway);

    //getting the carbs
    fscanf(prevDiary, "%s", throwaway);
    fscanf(prevDiary, "%f", &cur->carbs);
    fscanf(prevDiary, "%s", throwaway);

    //getting the fat
    fscanf(prevDiary, "%s", throwaway);
    fscanf(prevDiary, "%f", &cur->fat);
    fscanf(prevDiary, "%s", throwaway);

    //getting the protein
    fscanf(prevDiary, "%s", throwaway);
    fscanf(prevDiary, "%f", &cur->protein);
    fscanf(prevDiary, "%s", throwaway);

    fscanf(prevDiary, "%s", throwaway);
    fscanf(prevDiary, "%s", throwaway);
    fscanf(prevDiary, "%f%*c", &cur->serving_size);
    fscanf(prevDiary, "%s", throwaway);

    //getting the house serving size value and unit
    fscanf(prevDiary, "%s", throwaway);
    fscanf(prevDiary, "%s", throwaway);
    fscanf(prevDiary, "%s", throwaway);
    char temp[100];
    fscanf(prevDiary, "%s%*c", temp);

    if (strcmp(temp, "N/A") == 0) {
        cur->house_serving_size = 0;
        strcpy(cur->house_serving_size_unit, "none");
    }

    else {
        cur->house_serving_size = atof(temp);
        fscanf(prevDiary, "%s%*c", cur->house_serving_size_unit);
    }

    //getting the amount of servings
    fscanf(prevDiary, "%s", throwaway);
    fscanf(prevDiary, "%s", throwaway);
    fscanf(prevDiary, "%s", throwaway);
    fscanf(prevDiary, "%f", &cur->servings);
    return cur;
}

//this function goes through the previous journal file, throws away the breakfast, lunch, dinner, and snacks headers, and then goes to the getDataFromPrevJournal function to actually get the nodes for diaryArray
int readFromPrevJournal(Food *diaryArray[], char filename[]) {
    int tracker = 0;
    diaryArray[tracker] = NULL;
    FILE *prevDiary = fopen(filename, "r");
    Food *cur = (struct Food*)malloc(sizeof(struct Food));
    char temp[1000];

    //getting breakfast header
    fgets(temp, 10000, prevDiary);

    //now reading data
    while (strcmp(fgets(temp, 10000, prevDiary), "\n") != 0) {
        cur = getDataFromPrevJournal(cur, temp, prevDiary);
        strcpy(cur->meal, "breakfast");
        diaryArray[tracker] = cur;
        tracker++;
        fgets(temp, 10000, prevDiary);
    }

    //lunch header
    fgets(temp, 1000, prevDiary);
    //getting lunch
    while (strcmp(fgets(temp, 10000, prevDiary), "\n") != 0) {
        cur = getDataFromPrevJournal(cur, temp, prevDiary);
        strcpy(cur->meal, "lunch");
        diaryArray[tracker] = cur;
        tracker++;
        fgets(temp, 10000, prevDiary);
    }

    //dinner header
    fgets(temp, 1000, prevDiary);
    //getting dinner
    while (strcmp(fgets(temp, 10000, prevDiary), "\n") != 0) {
        cur = getDataFromPrevJournal(cur, temp, prevDiary);
        strcpy(cur->meal, "dinner");
        diaryArray[tracker] = cur;
        tracker++;
        fgets(temp, 10000, prevDiary);
    }

    //snacks header
    fgets(temp, 1000, prevDiary);
    //getting snacks
    while (fgets(temp, 10000, prevDiary) != NULL) {
        cur = getDataFromPrevJournal(cur, temp, prevDiary);
        strcpy(cur->meal, "snack");
        diaryArray[tracker] = cur;
        tracker++;
        fgets(temp, 10000, prevDiary);
    }

    return tracker;
}

//this function creates the actual name.log file, using the diaryArray array of nodes that contain the desired entries
void writeToLog(char filename[], Food *diaryArray[]) {
    //creates a new file
    int tracker = 0;
    float totalCals = 0;
    FILE *journal = fopen(filename, "w+");

    //entering the breakfast entries
    fprintf(journal, "Breakfast:\n");
    int k = 0;
    while (diaryArray[tracker]->ID != -1) {
        if (diaryArray[tracker]->ID == -5) {
            tracker++;
            continue;
        }
        else if (strcmp(diaryArray[tracker]->meal, "breakfast") == 0) {
            fprintf(journal, "%s\n", diaryArray[tracker]->name);
            fprintf(journal, "%s\n", diaryArray[tracker]->manufacturer);
            fprintf(journal, "Calories: %g kcal\n", diaryArray[tracker]->calories);
            fprintf(journal, "Carbs: %g grams\n", diaryArray[tracker]->carbs);
            fprintf(journal, "Fat: %g grams\n", diaryArray[tracker]->fat);
            fprintf(journal, "Protein: %g grams\n", diaryArray[tracker]->protein);
            if (diaryArray[tracker]->isGrams == true) {
                fprintf(journal, "Serving Size: %g grams\n", diaryArray[tracker]->serving_size);
            }
            else {
                fprintf(journal, "Serving Size: %g mL\n", diaryArray[tracker]->serving_size);
            }

            if (strcmp(diaryArray[tracker]->house_serving_size_unit, "none") != 0) {
                fprintf(journal, "Household Serving Size: %g %s\n", diaryArray[tracker]->house_serving_size, diaryArray[tracker]->house_serving_size_unit);
            }
            else {
                fprintf(journal, "Household Serving Size: N/A\n");
            }

            fprintf(journal, "Number of servings: %g\n", diaryArray[tracker]->servings);
            totalCals += diaryArray[tracker]->calories;
        }
        tracker++;
        k++;
    }

    //entering the lunch entries
    tracker = 0;
    fprintf(journal, "\nLunch:\n");
    while (diaryArray[tracker]->ID != -1) {
        if (diaryArray[tracker]->ID == -5) {
            tracker++;
            continue;
        }
        else if (strcmp(diaryArray[tracker]->meal, "lunch") == 0) {
            fprintf(journal, "%s\n", diaryArray[tracker]->name);
            fprintf(journal, "%s\n", diaryArray[tracker]->manufacturer);
            fprintf(journal, "Calories: %g kcal\n", diaryArray[tracker]->calories);
            fprintf(journal, "Carbs: %g grams\n", diaryArray[tracker]->carbs);
            fprintf(journal, "Fat: %g grams\n", diaryArray[tracker]->fat);
            fprintf(journal, "Protein: %g grams\n", diaryArray[tracker]->protein);
            if (diaryArray[tracker]->isGrams == true) {
                fprintf(journal, "Serving Size: %g grams\n", diaryArray[tracker]->serving_size);
            }
            else {
                fprintf(journal, "Serving Size: %g mL\n", diaryArray[tracker]->serving_size);
            }

            if (strcmp(diaryArray[tracker]->house_serving_size_unit, "none") != 0) {
                fprintf(journal, "Household Serving Size: %g %s\n", diaryArray[tracker]->house_serving_size, diaryArray[tracker]->house_serving_size_unit);
            }
            else {
                fprintf(journal, "Household Serving Size: N/A\n");
            }
            fprintf(journal, "Number of servings: %g\n", diaryArray[tracker]->servings);
            totalCals += diaryArray[tracker]->calories;
        }
        tracker++;
    }

    //entering the dinner entries
    tracker = 0;
    fprintf(journal, "\nDinner:\n");
    while (diaryArray[tracker]->ID != -1) {
        if (diaryArray[tracker]->ID == -5) {
            tracker++;
            continue;
        }
        else if (strcmp(diaryArray[tracker]->meal, "dinner") == 0) {
            fprintf(journal, "%s\n", diaryArray[tracker]->name);
            fprintf(journal, "%s\n", diaryArray[tracker]->manufacturer);
            fprintf(journal, "Calories: %g kcal\n", diaryArray[tracker]->calories);
            fprintf(journal, "Carbs: %g grams\n", diaryArray[tracker]->carbs);
            fprintf(journal, "Fat: %g grams\n", diaryArray[tracker]->fat);
            fprintf(journal, "Protein: %g grams\n", diaryArray[tracker]->protein);
            if (diaryArray[tracker]->isGrams == true) {
                fprintf(journal, "Serving Size: %g grams\n", diaryArray[tracker]->serving_size);
            }
            else {
                fprintf(journal, "Serving Size: %g mL\n", diaryArray[tracker]->serving_size);
            }
            
            if (strcmp(diaryArray[tracker]->house_serving_size_unit, "none") != 0) {
                fprintf(journal, "Household Serving Size: %g %s\n", diaryArray[tracker]->house_serving_size, diaryArray[tracker]->house_serving_size_unit);
            }
            else {
                fprintf(journal, "Household Serving Size: N/A\n");
            }
            fprintf(journal, "Number of servings: %g\n", diaryArray[tracker]->servings);
            totalCals += diaryArray[tracker]->calories;
        }
        tracker++;
    }

    //entering the snack entries
    tracker = 0;
    fprintf(journal, "\nSnacks:\n");
    while (diaryArray[tracker]->ID != -1) {
        if (diaryArray[tracker]->ID == -5) {
            tracker++;
            continue;
        }
        else if (strcmp(diaryArray[tracker]->meal, "snack") == 0) {
            fprintf(journal, "%s\n", diaryArray[tracker]->name);
            fprintf(journal, "%s\n", diaryArray[tracker]->manufacturer);
            fprintf(journal, "Calories: %g kcal\n", diaryArray[tracker]->calories);
            fprintf(journal, "Carbs: %g grams\n", diaryArray[tracker]->carbs);
            fprintf(journal, "Fat: %g grams\n", diaryArray[tracker]->fat);
            fprintf(journal, "Protein: %g grams\n", diaryArray[tracker]->protein);
            if (diaryArray[tracker]->isGrams == true) {
                fprintf(journal, "Serving Size: %g grams\n", diaryArray[tracker]->serving_size);
            }
            else {
                fprintf(journal, "Serving Size: %g mL\n", diaryArray[tracker]->serving_size);
            }
            
            if (strcmp(diaryArray[tracker]->house_serving_size_unit, "none") != 0) {
                fprintf(journal, "Household Serving Size: %g %s\n", diaryArray[tracker]->house_serving_size, diaryArray[tracker]->house_serving_size_unit);
            }
            else {
                fprintf(journal, "Household Serving Size: N/A\n");
            }
            fprintf(journal, "Number of servings: %g\n", diaryArray[tracker]->servings);
            totalCals += diaryArray[tracker]->calories;
        }
        tracker++;
    }

    fprintf(journal, "\nTotal calories: %g kcals\n", totalCals);
    
    //closing the file
    fclose(journal);
    return;
}

//this is a non-linear search and add function that edits an array of nodes that match the search criteria
void addEntry(Food *cur, char food[], Food *searchResults[], int * index) {
    //if the entire tree has been recursed down and the node does not exist, this returns
    if (cur == NULL || (*index) == -10) {
        return;
    }

    if ((*index) == 1000) {
        (*index) = -10;
        return;
    }
     
    //if the node is found, it adds to the array
    else if (strncasecmp(food, cur->name, strlen(food)) == 0) {
        searchResults[(*index)++] = cur;
        addEntry(cur->leftChild, food, searchResults, (index));
        addEntry(cur->rightChild, food, searchResults, (index));
    }

    else if (strncasecmp(food, cur->name, strlen(cur->name)) == 0) {
        addEntry(cur->leftChild, food, searchResults, index); 
    }

    else {
        //checks for alphabetical order (recursing down)
        if (strlen(food) < strlen(cur->name)) {
            if (strncasecmp(food, cur->name, strlen(food)) < 0) {
                addEntry(cur->leftChild, food, searchResults, index);
            }
            else {
                addEntry(cur->rightChild, food, searchResults, index);
            }
        }
        else {
            if (strncasecmp(food, cur->name, strlen(cur->name)) < 0) {
                addEntry(cur->leftChild, food, searchResults, index);
            }
            else {
                addEntry(cur->rightChild, food, searchResults, index);
            }
        }
    }
    return;
}

void getArray(Food *root, Food *searchResults[], char foodName[]) {
    int i = 0;
    addEntry(root, foodName, searchResults, &i);
    return;
}

//this is a linear search function that goes through the entire tree, printing the values that match the keyword for the name and/or brand. Only 20 are printed at a time, after which the user is asked if they want to continue or not. If they do not, the function stops
void search(Food *cur, char food[], char brand[], int * number) {
    //returns when a leaf is reached
    if (cur == NULL || (*number) == -10) {
        return;
    }

    else {
        for (int i = 0; i < strlen(cur->name); i++) {
            (cur->name[i]) = toupper(cur->name[i]);
        }

        for (int i = 0; i < strlen(food); i++) {
            food[i] = toupper(food[i]);
        }

        if (strcmp(brand, "UNKNOWN") != 0) {
            for (int i = 0; i < strlen(brand); i++) {
                brand[i] = toupper(brand[i]);
            }
        }

        for (int i = 0; i < strlen(cur->manufacturer); i++) {
            (cur->manufacturer[i]) = toupper(cur->manufacturer[i]);
        }

        if ((*number) == 20) {
            char yn[15];
            printf("Would you like to view more results? Enter yes or no, then hit enter.\n");
            scanf("%15[^\n]%*c", yn);

            while (strcasecmp(yn, "yes") != 0 && strcasecmp(yn, "no") != 0) {
                printf("Sorry, that isn't a valid response. Enter yes or no, then hit enter.\n");
                scanf("%10[^\n]%*c", yn);
            }

            if (strcasecmp(yn, "no") == 0) {
                (*number) = -10;
                return;
            }
            (*number) = 0;
        }

        //if the user didn't know the brand when prompted
        else if (strcmp(brand, "UNKNOWN") == 0) {
            //prints when a match is found
            if (strstr(cur->name, food) != NULL) {
                if ((*number) == 0) {
                    printf("Results: \n");
                }  
                printf("Food: %s, Brand: %s, Calories: %g kcal\n", cur->name, cur->manufacturer, cur->calories);
                (*number)++;
            }
        }

        //prints when a match is found
        else if ((strstr(cur->name, food) != NULL) && (strstr(cur->manufacturer, brand) != NULL)) {
            if ((*number) == 0) {
                printf("Results: \n");
            }
            printf("Food: %s, Brand: %s Calories: %g kcal\n", cur->name, cur->manufacturer, cur->calories);
            (*number)++;
        }

        //recurses down the tree
        search(cur->leftChild, food, brand, number);
        search(cur->rightChild, food, brand, number);
    }
}

//this function initializes structs to have all integer/float values equal to -1 and all string values equal to "empty"
Food *initialize() {
    Food *cur = (struct Food*)malloc(sizeof(struct Food));
    cur->ID = -1;
    strcpy(cur->name, "empty");
    strcpy(cur->manufacturer, "empty");
    cur->calories = -1;
    cur->carbs = -1;
    cur->fat = -1;
    cur->protein = -1;
    cur->serving_size = -1;
    cur->isGrams = true;
    cur->house_serving_size = -1;
    strcpy(cur->house_serving_size_unit, "empty");
    cur->leftChild = NULL;
    cur->rightChild = NULL;
    return cur;
}

//this function does the actual editing of the journal for each person
void editJournal(char name[], Food* root) {
    //creating the name of the .log file
    FILE* journal;
    char filename[50];
    strncpy(filename, name, 50);
    strcat(filename, ".log");

    //this is a yes/no string that I use multiple times across view, add etc., so I declare it up top
    char yn[10];
    //templine used for fgets mainly
    char temp[1000];
    //seeing if there are already files with certain names, and if there are i append numbers to the end
    int numJournals = 1;
    char numJournalsString[5] = "1";

    //creating the array of nodes which is used to store the entires for each diary
    Food *diaryArray[1000];
    for (int k = 0; k < 1000; k++) {
        diaryArray[k] = (struct Food*)malloc(sizeof(struct Food));
        diaryArray[k] = initialize();
    }
    int diaryTracker = 0;

    //creating a temp node to be used
    Food *newEntry = initialize();
    newEntry = (struct Food*)malloc(sizeof(struct Food));

    //checks if the diary already exists
    journal = fopen(filename, "r");
    if (journal != NULL) {
        fclose(journal);
        //if the diary exists, the user is asked if it belongs to them
        printf("Is this your diary? Enter yes or no, then hit enter.\n");
        printf("%s\n", filename);
        scanf("%10[^\n]%*c", yn);
        while (strcasecmp(yn, "yes") != 0 && strcasecmp(yn, "no") != 0) {
            printf("Sorry, that isn't a valid response. Enter yes or no, then hit enter.\n");
            scanf("%10[^\n]%*c", yn);
        }
        //if the diary is theirs, the data is read in and the new entries (or updates, deletions, etc.) are added to that previously existing list
        if (strcasecmp(yn, "yes") == 0) {
            diaryTracker = readFromPrevJournal(diaryArray, filename);
        }
        else {
            filename[strlen(filename) - 4] = '\0';
            strcat(filename, numJournalsString);
            strcat(filename, ".log");
            journal = fopen(filename, "r");
            while (journal != NULL && strcasecmp(yn, "no") == 0) {
                printf("Is this your diary? Enter yes or no, then hit enter.\n");
                printf("%s\n", filename);
                scanf("%10[^\n]%*c", yn);
                while (strcasecmp(yn, "yes") != 0 && strcasecmp(yn, "no") != 0) {
                    printf("Sorry, that isn't a valid response. Enter yes or no, then hit enter.\n");
                    scanf("%10[^\n]%*c", yn);
                }
                if (strcasecmp(yn, "yes") == 0) {
                    diaryTracker = readFromPrevJournal(diaryArray, filename);
                    break;
                }
                numJournals++;
                sprintf(numJournalsString, "%d", numJournals);
                filename[strlen(filename) - 5] = '\0';
                strcat(filename, numJournalsString);
                strcat(filename, ".log");
                journal = fopen(filename, "r");
            }
            writeToLog(filename, diaryArray);
        }
    }

    else {
        writeToLog(filename, diaryArray);
    }

    //opening choice menu
    printf("\nHi %s!\n", name);
    printf("This is a program to build you a food diary. You can have a maximum of 1000 entries.\n");
    printf("Here are your options:\n\nView diary\nSearch for all foods containing keyword\nSearch for and add entry (search must be first word in food name)\nUpdate entry\nDelete entry\nQuit\n");
    printf("\nEnter your choice, then hit enter: ");
    //reading in what the user's choice is
    char choice[50];
    scanf("%50[^\n]%*c", choice);

    while (1) {
        for (int i = 0; i < strlen(choice); i++) {
                choice[i] = toupper(choice[i]);
        }

        //view diary
        if (strstr(choice, "VIEW") != NULL) {
            journal = fopen(filename, "r");
            printf("\n");
            while (fgets(temp, 10000, journal) != NULL) {
                printf("%s", temp);
            }
        }

        else if (strcmp(choice, "SEARCH") == 0) {
            printf("Please be more specific. Would you like to search for a keyword, or search and add?\n");
            printf("Enter your choice, then hit enter: ");
            scanf("%50[^\n]%*c", choice);
            if (strcasecmp(choice, "search") == 0) {
                scanf("%50s", choice);
            }
            continue;
        }

        //search foods
        else if ((strstr(choice, "SEARCH FOR ALL")) != NULL || (strstr(choice, "KEYWORD") != NULL)) {
            char food[1000];
            char brand[1000];
            int number = 0;
            //asks the user if they know the name of the brand
            printf("Do you know the brand? Enter yes or no, then hit enter.\n");
            scanf("%10[^\n]%*c", yn);
            while (strcasecmp(yn, "yes") != 0 && strcasecmp(yn, "no") != 0) {
                printf("Sorry, that isn't a valid response. Enter yes or no, then hit enter.\n");
                scanf("%10[^\n]%*c", yn);
            }
            //if they know the name of the brand, the food and brand are searched for
            if (strcasecmp(yn, "yes") == 0) {
                printf("Enter the brand name, then hit enter.\n");
                scanf("%1000[^\n]%*c", brand);
                printf("Enter the name of the food, then hit enter.\n");
                scanf("%1000[^\n]%*c", food);
                printf("\n");
                search(root, food, brand, &number);
            }
            //if not, just the name of the food is searched for
            else {
                printf("Enter the name of the food, then hit enter.\n");
                scanf("%1000[^\n]%*c", food);
                printf("\n");
                search(root, food, "UNKNOWN", &number);
            }
        }

        //add entry to diary
        else if (strstr(choice, "ADD") != NULL) {
            if (diaryTracker == 999) {
                diaryTracker = 0;
            }
            Food *searchResults[1000];
            for (int k = 0; k < 1000; k++) {
                searchResults[k] = (struct Food*)malloc(sizeof(struct Food));
                searchResults[k] = initialize();
            }

            char foodName[1000];
            printf("Enter the name of your food, then hit enter. Please be specific--only 1000 results will be available, loaded 20 at a time.\n");
            scanf("%1000[^\n]%*c", foodName);
            getArray(root, searchResults, foodName);

            //if the found isn't found, an error message is printed in the function, and the choice menu appears again
            if (searchResults[0]->ID == -1) {
                printf("Food not found\n");
                printf("\nHere are your options:\n\nView diary\nSearch for all foods containing keyword\nSearch for and add entry (search must be first word in food name)\nUpdate entry\nDelete entry\nQuit\n");
                printf("\nEnter your choice, then hit enter: ");
                scanf("%50[^\n]%*c", choice);
                continue;
            }

            int i = 0;
            int j = 0;
            printf("Results:\n");
            while (i < 20 && searchResults[j]->ID != -1) {
               printf("%d. Food: %s Brand: %s Calories: %g\n", (j + 1), searchResults[j]->name, searchResults[j]->manufacturer, searchResults[j]->calories * searchResults[j]->serving_size /100);
                j++;
                if (i == 19) {
                    printf("Would you like to see more results? Enter yes or no, then hit enter.\n");
                    scanf("%10[^\n]%*c", yn);
                    while (strcasecmp(yn, "yes") != 0 && strcasecmp(yn, "no") != 0) {
                        printf("Sorry, that isn't a valid response. Enter yes or no, then hit enter.\n");
                        scanf("%10[^\n]%*c", yn);
                    }
                    if (strcasecmp(yn, "yes") == 0) {
                        i = 0;
                    }
                }
                i++;
            }

            printf("Would you like to add any of these? Enter yes or no, then hit enter.\n");
            scanf("%10[^\n]%*c", yn);
            while (strcasecmp(yn, "yes") != 0 && strcasecmp(yn, "no") != 0) {
                printf("Sorry, that isn't a valid response. Enter yes or no, then hit enter.\n");
                scanf("%10[^\n]%*c", yn);
            }

            if (strcasecmp(yn, "no") == 0) {
                printf("\nHere are your options:\n\nView diary\nSearch for all foods containing keyword\nSearch for and add entry (search must be first word in food name)\nUpdate entry\nDelete entry\nQuit\n");
                printf("\nEnter your choice, then hit enter: ");
                scanf("%50[^\n]%*c", choice);
                continue;
            }
            
            char entryChoice[10];
            printf("Which entry would you like to add? Enter the number, then hit enter.\n");
            scanf("%10[^\n]%*c", entryChoice);
            while (isdigit(atoi(entryChoice)) != 0) {
                printf("Sorry, that is not a valid option. Please enter a number, then hit enter.\n");
                scanf("%10[^\n]%*c", entryChoice);
            }

            newEntry = searchResults[atoi(entryChoice) - 1];
            printf("You chose %s from %s.\n", newEntry->name, newEntry->manufacturer);
            diaryArray[diaryTracker]->ID = newEntry->ID;
            strncpy(diaryArray[diaryTracker]->name, newEntry->name, 500);
            strncpy(diaryArray[diaryTracker]->manufacturer, newEntry->manufacturer, 500);
            diaryArray[diaryTracker]->serving_size = newEntry->serving_size;
            diaryArray[diaryTracker]->isGrams = newEntry->isGrams;
            diaryArray[diaryTracker]->house_serving_size = newEntry->house_serving_size;
            strncpy(diaryArray[diaryTracker]->house_serving_size_unit, newEntry->house_serving_size_unit, 100);
            diaryArray[diaryTracker]->leftChild = NULL;
            diaryArray[diaryTracker]->rightChild = NULL;
            diaryArray[diaryTracker]->calories = newEntry->calories * newEntry->serving_size / 100;
            diaryArray[diaryTracker]->carbs = newEntry->carbs * newEntry->serving_size / 100;
            diaryArray[diaryTracker]->fat = newEntry->fat * newEntry->serving_size / 100;
            diaryArray[diaryTracker]->protein = newEntry->protein * newEntry->serving_size / 100;
            
            //this prints the correct serving size, with household and grams or mL depending on which it had in the database
            if (strcmp(diaryArray[diaryTracker]->house_serving_size_unit, "none") != 0) {
                if (diaryArray[diaryTracker]->isGrams == true) {
                    printf("How many servings did you have? One serving is %g %s or %g grams. Please enter a number, then hit enter.\n", diaryArray[diaryTracker]->house_serving_size, diaryArray[diaryTracker]->house_serving_size_unit, newEntry->serving_size);
                }
                else {
                    printf("How many servings did you have? One serving is %g %s or %g mL. Please enter a number, then hit enter.\n", diaryArray[diaryTracker]->house_serving_size, diaryArray[diaryTracker]->house_serving_size_unit, newEntry->serving_size);
                }
            }
            
            else {
                if (diaryArray[diaryTracker]->isGrams == true) {
                    printf("How many servings did you have? One serving is %g grams. Please enter a number, then hit enter.\n", newEntry->serving_size);
                }
                else {
                    printf("How many servings did you have? One serving is %g mL. Please enter a number, then hit enter.\n", newEntry->serving_size);
                }
            }

            char tempValue[10];
            scanf("%10[^\n]%*c", tempValue);
            //checking for user error where they don't enter a number
            while (isdigit(atof(tempValue)) != 0) {
                printf("Sorry, that is not a valid option. Please enter a number, then hit enter.\n");
                scanf("%10[^\n]%*c", tempValue);
            }
            diaryArray[diaryTracker]->servings = atof(tempValue);
            
            //gets the right number of calories depending on the amount of servings entered by the user
            diaryArray[diaryTracker]->calories *= diaryArray[diaryTracker]->servings;
            diaryArray[diaryTracker]->carbs *= diaryArray[diaryTracker]->servings;
            diaryArray[diaryTracker]->fat *= diaryArray[diaryTracker]->servings;
            diaryArray[diaryTracker]->protein *= diaryArray[diaryTracker]->servings;

            //checking what meal this entry should be logged under
            printf("Did you eat this for breakfast, lunch, dinner, or a snack? Enter the meal's name, then hit enter.\n");
            char meal[40];
            scanf("%40[^\n]%*c", meal);
            while (1) {
                if (strcasecmp(meal, "breakfast") == 0) {
                    strcpy(diaryArray[diaryTracker]->meal, "breakfast");
                    break;
                }

                else if (strcasecmp(meal, "lunch") == 0) {
                    strcpy(diaryArray[diaryTracker]->meal, "lunch");
                    break;
                }
                else if (strcasecmp(meal, "dinner") == 0) {
                    strcpy(diaryArray[diaryTracker]->meal, "dinner");
                    break;
                }
                else if (strcasecmp(meal, "snack") == 0) {
                    strcpy(diaryArray[diaryTracker]->meal, "snack");
                    break;
                }
                //checking for user error
                printf("Sorry, that is not an option. Please choose breakfast, lunch, dinner, or snack, then hit enter.\n");
                scanf("%40[^\n]%*c", meal);
            }
            
            //writes this new value into the file, in addition to any old values
            diaryTracker++;
            writeToLog(filename, diaryArray);

        }

        //update previous entries (one at a time)
        else if (strstr(choice, "UPDATE") != NULL) {
            journal = fopen(filename, "r");
            //checking if the diary exists
            if (journal == NULL || diaryArray[0]->ID == -1) {
                printf("\nDiary is empty.\n");
                printf("\nHere are your options:\n\nView diary\nSearch for all foods containing keyword\nSearch for and add entry (search must be first word in food name)\nUpdate entry\nDelete entry\nQuit\n");
                printf("\nEnter your choice, then hit enter: ");
                scanf("%50[^\n]%*c", choice);
                continue;
            }

            //prints the diary before asking for update data, just so the user sees it
            else {
                printf("\nThis is your current diary.\n");
                while (fgets(temp, 10000, journal) != NULL) { 
                    printf("%s", temp);
                }

                char toUpdate[500];
                char mealUpdate[50];
                //getting the name and meal of the food to be updated
                printf("\nWhich item would you like to update? Type in the full name of the food, then hit enter.\n");
                scanf("%500[^\n]%*c", toUpdate);
                printf("What meal was this food logged under? Please enter the meal's name, then hit enter.\n");
                scanf("%50[^\n]%*c", mealUpdate);
                while (1) {
                    if ((strcasecmp(mealUpdate, "breakfast") == 0) || (strcasecmp(mealUpdate, "lunch") == 0) || (strcasecmp(mealUpdate, "dinner") == 0) || (strcasecmp(mealUpdate, "snack") == 0)) {
                        break;
                    }
                    else {
                        printf("Sorry, that is not an option. Please choose breakfast, lunch, dinner, or snack, then hit enter.\n");
                        scanf("%50[^\n]%*c", mealUpdate);
                    }
                }

                bool notThere = false;
                int tracker = 0;

                //checking if the inputted values match an actual entry in the diary
                while (diaryArray[tracker]->ID != -1) {
                    if (strcasecmp(diaryArray[tracker]->name, toUpdate) == 0 && strcasecmp(diaryArray[tracker]->meal, mealUpdate) == 0) {
                        break;
                    }
                    if (diaryArray[tracker + 1]->ID == -1) {
                        notThere = true;
                        printf("Food not found\n");
                        break;
                    }
                    tracker++;
                }

                //if the entry isn't found, the choice menu comes up again
                if (notThere == true) {
                    printf("\nHere are your options:\n\nView diary\nSearch for all foods containing keyword\nASearch for and add entry (search must be first word in food name)\nUpdate entry\nDelete entry\nQuit\n");
                    printf("\nEnter your choice, then hit enter: ");
                    scanf("%50[^\n]%*c", choice);
                    continue;
                }

                //the only value that can be updated is the amount of servings, so that is prompted for
                else {
                    //if there is a household serving size
                    char tempValue[10];
                    if (strcmp(diaryArray[tracker]->house_serving_size_unit, "none") != 0) {
                        printf("How many servings did you have? Previously, the value was %g %s.\n", diaryArray[tracker]->servings, diaryArray[tracker]->house_serving_size_unit);
                        //the amount of nutrients depends on the amount of servings, so these values are changed accordingly
                        diaryArray[tracker]->calories /= diaryArray[tracker]->servings;
                        diaryArray[tracker]->carbs /= diaryArray[tracker]->servings;
                        diaryArray[tracker]->fat /= diaryArray[tracker]->servings;
                        diaryArray[tracker]->protein /= diaryArray[tracker]->servings;

                        scanf("%10[^\n]%*c", tempValue);
                        //checking for user error where they don't enter a number
                        while (isdigit(atof(tempValue)) != 0) {
                            printf("Sorry, that is not a valid option. Please enter a number, then hit enter.\n");
                            scanf("%10[^\n]%*c", tempValue);
                        }
                        diaryArray[tracker]->servings = atof(tempValue);

                        diaryArray[tracker]->calories *= diaryArray[tracker]->servings;
                        diaryArray[tracker]->carbs *= diaryArray[tracker]->servings;
                        diaryArray[tracker]->fat *= diaryArray[tracker]->servings;
                        diaryArray[tracker]->protein *= diaryArray[tracker]->servings;
                        //these new, updated values are written to the diary
                        writeToLog(filename, diaryArray); 
                    }
                    else {
                        //if there is no household serving size, but functions in the same way as above
                        if (diaryArray[tracker]->isGrams == true) {
                            printf("How many servings did you have? Previously, the value was %g grams.\n", diaryArray[tracker]->servings);
                        }
                        else {
                            printf("How many servings did you have? Previously, the value was %g mL.\n", diaryArray[tracker]->servings);
                        }
                        diaryArray[tracker]->calories /= diaryArray[tracker]->servings;
                        diaryArray[tracker]->carbs /= diaryArray[tracker]->servings;
                        diaryArray[tracker]->fat /= diaryArray[tracker]->servings;
                        diaryArray[tracker]->protein /= diaryArray[tracker]->servings;

                        scanf("%10[^\n]%*c", tempValue);
                        //checking for user error where they don't enter a number
                        while (isdigit(atof(tempValue)) != 0) {
                            printf("Sorry, that is not a valid option. Please enter a number, then hit enter.\n");
                            scanf("%10[^\n]%*c", tempValue);
                        }

                        diaryArray[tracker]->servings = atof(tempValue);
                        diaryArray[tracker]->calories *= diaryArray[tracker]->servings;
                        diaryArray[tracker]->carbs *= diaryArray[tracker]->servings;
                        diaryArray[tracker]->fat *= diaryArray[tracker]->servings;
                        diaryArray[tracker]->protein *= diaryArray[tracker]->servings;
                        writeToLog(filename, diaryArray);
                    }
                }
            }
            //closes the diary file
            fclose(journal);
        }

        //delete entry
        else if (strstr(choice, "DELETE") != NULL) {
            journal = fopen(filename, "r");
            //checking if the journal file exists
            //if it does not exist, the choice menu comes up again
            if (journal == NULL || diaryArray[0]->ID == -1) {
                printf("\nDiary is empty.\n");
                printf("\nHere are your options:\n\nView diary\nSearch for all foods containing keyword\nSearch for and add entry (search must be first word in food name)\nUpdate entry\nDelete entry\nQuit\n");
                printf("\nEnter your choice, then hit enter: ");
                scanf("%50[^\n]%*c", choice);
                continue;
            }
            else {
                //prints the current diary file, just so the user can see it
                printf("\nThis is your current diary.\n");
                while (fgets(temp, 10000, journal) != NULL) {
                    printf("%s", temp);
                }
                printf("\n");
                char toDelete[500];
                char mealDelete[50];
                //prompts the user for the name and meal of the entry to be deleted
                printf("Which item would you like to delete? Please enter the full name, then hit enter.\n");
                scanf("%500[^\n]%*c", toDelete);
                printf("What meal was this food logged under? Please enter the meal's name, then hit enter.\n");
                scanf("%50[^\n]%*c", mealDelete);
                while (1) {
                    //makes sure the user enters a valid meal category
                    if (strncasecmp(mealDelete, "breakfast", 9) == 0 || strncasecmp(mealDelete, "lunch", 5) == 0 || strncasecmp(mealDelete, "dinner", 6) == 0 || strncasecmp(mealDelete, "snack", 5) == 0) {
                        break;
                    }
                    printf("Sorry, that is not an option. Please enter breakfast, lunch, dinner, or snack, then hit enter.\n");
                    scanf("%50[^\n]%*c", mealDelete);
                } 

                bool notThere = true;
                int tracker = 0;

                while (diaryArray[tracker]->ID != -1) {
                    if (strncasecmp(diaryArray[tracker]->name, toDelete, strlen(toDelete)) == 0 && strncasecmp(mealDelete, diaryArray[tracker]->meal, strlen(diaryArray[tracker]->meal)) == 0) {
                        notThere = false;
                        break;
                    }
                    if (diaryArray[tracker + 1]->ID == -1) {
                        printf("Food not found\n");
                            break;
                        }
                    tracker++;
                }

                //if no entry is found that matches the user input the choice menu comes up again
                if (notThere == true) {
                    printf("\nHere are your options:\n\nView diary\nSearch for all foods containing keyword\nSearch for and add entry (search must be first word in food name)\nUpdate entry\nDelete entry\nQuit\n");
                    printf("\nEnter your choice, then hit enter: ");
                    scanf("%50[^\n]%*c", choice);
                    continue;
                }

                //otherwise the given node is deleted
                diaryArray[tracker] = initialize();
                diaryArray[tracker]->ID = -5;
                writeToLog(filename, diaryArray);
            }
        }

        //quit
        else if (strstr(choice, "QUIT") != NULL) {
            break;
        }

        //if they entered an invalid option
        else {
            printf("Sorry, that is not an available option. Please choose to view, search, update, delete, or quit.\n");
            scanf("%50s", choice);
        }

        int m = 0;
        while (1) {
            if (diaryArray[m]->ID == -5 || diaryArray[m]->ID == -1) {
                diaryTracker = m;
                break;
            }
            m++;

        }

        printf("\nHere are your options:\n\nView diary\nSearch for all foods containing keyword\nSearch for and add entry (search must be first word in food name)\nUpdate entry\nDelete entry\nQuit\n");
        printf("\nEnter your choice, then hit enter: ");
        scanf("%50[^\n]%*c", choice);
        printf("Your choice is %s.\n", choice);
    }

    return;
}