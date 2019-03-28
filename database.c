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
    fscanf(prevDiary, "%f%*c", &cur->house_serving_size);
    fscanf(prevDiary, "%s%*c", cur->house_serving_size_unit);

    //getting the amount of servings
    fscanf(prevDiary, "%s", throwaway);
    fscanf(prevDiary, "%s", throwaway);
    fscanf(prevDiary, "%s", throwaway);
    fscanf(prevDiary, "%f", &cur->servings);
    return cur;
}

//this function goes through the previous journal file, throws away the breakfast, lunch, dinner, and snacks headers, and then goes to the getDataFromPrevJournal function to actually get the nodes for the linked list
void readFromPrevJournal(Food *diaryArray[], char filename[]) {
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

    //returns the first node of the linked list that creates the journal
    return;
}

//this function creates the actual name.log file, using the linked list that is passed in
void writeToLog(char filename[], Food *diaryArray[]) {
    //creates a new file
    int tracker = 0;
    FILE *journal = fopen(filename, "w+");

    //entering the breakfast entries
    fprintf(journal, "Breakfast:\n");
    int k = 0;
    while (diaryArray[tracker]->ID != -1) {
        printf("hello %d\n", k);
        if (strcmp(diaryArray[tracker]->meal, "breakfast") == 0) {
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
            fprintf(journal, "Household Serving Size: %g %s\n", diaryArray[tracker]->house_serving_size, diaryArray[tracker]->house_serving_size_unit);
            fprintf(journal, "Number of servings: %g\n", diaryArray[tracker]->servings);
        }
        tracker++;
        k++;
    }

    //entering the lunch entries
    tracker = 0;
    fprintf(journal, "\nLunch:\n");
    while (diaryArray[tracker]->ID != -1) {
        if (strcmp(diaryArray[tracker]->meal, "lunch") == 0) {
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
            fprintf(journal, "Household Serving Size: %g %s\n", diaryArray[tracker]->house_serving_size, diaryArray[tracker]->house_serving_size_unit);
            fprintf(journal, "Number of servings: %g\n", diaryArray[tracker]->servings);
        }
        tracker++;
    }

    //entering the dinner entries
    tracker = 0;
    fprintf(journal, "\nDinner:\n");
    while (diaryArray[tracker]->ID != -1) {
        if (strcmp(diaryArray[tracker]->meal, "dinner") == 0) {
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
            fprintf(journal, "Household Serving Size: %g %s\n", diaryArray[tracker]->house_serving_size, diaryArray[tracker]->house_serving_size_unit);
            fprintf(journal, "Number of servings: %g\n", diaryArray[tracker]->servings);
        }
        tracker++;
    }

    //entering the snack entries
    tracker = 0;
    fprintf(journal, "\nSnacks:\n");
    while (diaryArray[tracker]->ID != -1) {
        if (strcmp(diaryArray[tracker]->meal, "breakfast") == 0) {
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
            fprintf(journal, "Household Serving Size: %g %s\n", diaryArray[tracker]->house_serving_size, diaryArray[tracker]->house_serving_size_unit);
            fprintf(journal, "Number of servings: %g\n", diaryArray[tracker]->servings);
        }
        tracker++;
    }
    //closing the file
    fclose(journal);
    return;
}

//this is a non-linear search and add function that finds the node described by the user input then returns it for adding
void addEntry(Food *cur, char food[], Food *searchResults[], int * index) {
    //if the entire tree has been recursed down and the node does not exist, this returns
    if (cur == NULL) {
        searchResults[*index] = NULL;
        return;
    }
    if ((*index) == 1000) {
        printf("Too many entries available. Pick a more specific keyword.\n");
        return;
    }
     
    //if the node is found, it adds to the array
    else if (strncasecmp(food, cur->name, strlen(food)) == 0) {
            searchResults[(*index)++] = cur;
            addEntry(cur->leftChild, food, searchResults, (index));
            addEntry(cur->rightChild, food, searchResults, (index));
    }

    else {
        //checks for alphabetical order (recursing down)
        if (strncasecmp(food, cur->name, strlen(food)) < 0) {
            addEntry(cur->leftChild, food, searchResults, index);
        }
        else {
            addEntry(cur->rightChild, food, searchResults, index);
        }
    }
    return;
}

void getArray(Food *root, Food *searchResults[], char foodName[]) {
    int i = 0;
    addEntry(root, foodName, searchResults, &i);
    return;
}

//this is a linear search function that goes through the entire tree, printing the values that match what was input for the name and/or brand
void search(Food *cur, char food[], char brand[], int * number) {
    //returns when a leaf is reached
    if (cur == NULL || (*number) == -10) {
        return;
    }

    else if ((*number) == 20) {
        char yn[15];
        printf("Would you like to view more results? Enter yes or no.\n");
        scanf("%[^\n]%*c", yn);
        if (strcasecmp(yn, "no") == 0) {
            (*number) = -10;
            return;
        }
        (*number) = 0;
    }

    //if the user didn't know the brand when prompted
    else if (strcmp(brand, "unknown") == 0) {
            //prints when a match is found
            if (strcasestr(cur->name, food) != NULL) {
            printf("Food: %s, Brand: %s, Calories: %g kcal\n", cur->name, cur->manufacturer, cur->calories);
            (*number)++;
            }
    }

    //prints when a match is found
    else if ((strcasestr(cur->name, food) != NULL) && (strcasestr(cur->manufacturer, brand) != NULL)) {
        printf("Food: %s, Brand: %s Calories: %g kcal\n", cur->name, cur->manufacturer, cur->calories);
        (*number)++;
    }
    //recurses down the tree
    search(cur->leftChild, food, brand, number);
    search(cur->rightChild, food, brand, number);
}

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
    strcpy(filename, name);
    strcat(filename, ".log");

    //this is a yes/no string that I use multiple times across view, add etc., so I declare it up top
    char yn[10];
    char temp[1000];

    //here i'm allocating memory for the various temporary nodes i use in the sections for add, update, etc. so that they can easily be freed at the end of the program
    Food *diaryArray[1000];
    for (int k = 0; k < 1000; k++) {
        diaryArray[k] = (struct Food*)malloc(sizeof(struct Food));
        diaryArray[k] = initialize();
    }

    int diaryTracker = 0;

    Food *newEntry = initialize();
    newEntry = (struct Food*)malloc(sizeof(struct Food));

    bool notUsingPrev = false;

    //checks if the log already exists
    journal = fopen(filename, "r");
    if (journal != NULL) {
        fclose(journal);
        //if the journal exists, the user is asked if it belongs to them
        printf("Is this your diary?\n");
        printf("%s\n", filename);
        scanf("%[^\n]%*c", yn);
        //if the journal is theirs, the data is read in and the new entries (or updates, deletions, etc.) are added to that previously existing list
        if (strcasecmp(yn, "yes") == 0) {
            readFromPrevJournal(diaryArray, filename);
        }
        else {
            writeToLog(filename, diaryArray);
        }
    }

    else {
        writeToLog(filename, diaryArray);
    }

    //opening choice menu
    printf("\nHi %s!\n", name);
    printf("This is a program to build you a food diary. You can have a maximum of 1000 entries.\n");
    printf("Here are your options:\nView diary\nSearch for all foods containing keyword\nSearch for and add entry (search must be first word in food name)\nUpdate entry\nDelete entry\nQuit\n");
    printf("Enter your choice: ");
    //reading in what the user's choice is
    char choice[50];
    scanf("%[^\n]%*c", choice);

    while (1) {
        //view diary
        if (strcasestr(choice, "view") != NULL) {
            journal = fopen(filename, "r");
            printf("\n");
            while (fgets(temp, 10000, journal) != NULL) {
                printf("%s", temp);
            }
        }

        else if (strcasecmp(choice, "search") == 0) {
            printf("Please be more specific. Would you like to search for a keyword, or search and add?\n");
            printf("Enter your choice: ");
            scanf("%[^\n]%*c", choice);
            continue;
        }

        //search foods
        else if ((strcasestr(choice, "search for all")) != NULL || (strcasestr(choice, "keyword") != NULL)) {
            char food[1000];
            char brand[1000];
            int number = 0;
            //asks the user if they know the name of the brand
            printf("Do you know the brand?\n");
            scanf("%[^\n]%*c", yn);
            //if they know the name of the brand, the food and brand are searched for
            if (strcasecmp(yn, "yes") == 0) {
                printf("What is the brand name?\n");
                scanf("%[^\n]%*c", brand);
                printf("What did you eat?\n");
                scanf("%[^\n]%*c", food);
                printf("\n");
                search(root, food, brand, &number);
            }
            //if not, just the name of the food is searched for
            else {
                printf("What did you eat?\n");
                scanf("%[^\n]%*c", food);
                printf("\n");
                search(root, food, "unknown", &number);
            }
        }

        //add entry to diary
        else if (strcasestr(choice, "add") != NULL) {
            Food *searchResults[1000];
            for (int k = 0; k < 1000; k++) {
            searchResults[k] = (struct Food*)malloc(sizeof(struct Food));
            searchResults[k] = initialize();
            }

            char foodName[1000];
            printf("What is the name of your food?\n");
            scanf("%[^\n]%*c", foodName);
            getArray(root, searchResults, foodName);

            //if the found isn't found, an error message is printed in the function, and the choice menu appears again
            if (searchResults[0]->ID == -1) {
                printf("Food not found\n");
                printf("\nHere are your options:\nView diary\nSearch for all foods containing keyword\nSearch for and add entry (search must be first word in food name)\nUpdate entry\nDelete entry\nQuit\n");
                printf("Enter your choice: ");
                scanf("%[^\n]%*c", choice);
                continue;
            }

            int i = 0;
            int j = 0;
            printf("Results:\n");
            while (i < 20 && searchResults[j]->ID != -1) {
               printf("%d. Food: %s Brand: %s Calories: %g\n", (j + 1), searchResults[j]->name, searchResults[j]->manufacturer, searchResults[j]->calories);
                j++;
                if (i == 19) {
                    printf("Would you like to see more results? Enter yes or no.\n");
                    scanf("%[^\n]%*c", yn);
                    if (strcasecmp(yn, "yes") == 0) {
                        i = 0;
                    }
                }
                i++;
            }

            printf("Would you like to add any of these? Enter yes or no.\n");
            scanf("%[^\n]%*c", yn);
            if (strcasecmp(yn, "no") == 0) {
                printf("\nHere are your options:\nView diary\nSearch for all foods containing keyword\nSearch for and add entry (search must be first word in food name)\nUpdate entry\nDelete entry\nQuit\n");
                printf("Enter your choice: ");
                scanf("%[^\n]%*c", choice);
                continue;
            }
            
            int entryChoice;
            printf("Which entry would you like to add? Enter the number.\n");
            scanf("%d%*c", &entryChoice);
            while (isdigit(entryChoice) != 0) {
                printf("Sorry, that is not a valid option. Please enter a number.\n");
                scanf("%d%*c", &entryChoice);
            }

            newEntry = searchResults[entryChoice - 1];
            printf("You chose %s from %s.\n", newEntry->name, newEntry->manufacturer);
            diaryArray[diaryTracker]->ID = newEntry->ID;
            strcpy(diaryArray[diaryTracker]->name, newEntry->name);
            strcpy(diaryArray[diaryTracker]->manufacturer, newEntry->manufacturer);
            diaryArray[diaryTracker]->calories = newEntry->calories;
            diaryArray[diaryTracker]->carbs = newEntry->carbs;
            diaryArray[diaryTracker]->fat = newEntry->fat;
            diaryArray[diaryTracker]->protein = newEntry->protein;
            diaryArray[diaryTracker]->serving_size = newEntry->serving_size;
            diaryArray[diaryTracker]->isGrams = newEntry->isGrams;
            diaryArray[diaryTracker]->house_serving_size = newEntry->house_serving_size;
            strncpy(diaryArray[diaryTracker]->house_serving_size_unit, newEntry->house_serving_size_unit, strlen(newEntry->house_serving_size_unit) - 1);
            diaryArray[diaryTracker]->leftChild = NULL;
            diaryArray[diaryTracker]->rightChild = NULL;
            
            //this prints the correct serving size, with household and grams or mL depending on which it had in the database
            if (diaryArray[diaryTracker]->isGrams == true) {
                printf("How many servings did you have? One serving is %g %s or %g grams. Please enter a number.\n", diaryArray[diaryTracker]->house_serving_size, diaryArray[diaryTracker]->house_serving_size_unit, newEntry->serving_size);
            }
            else {
                printf("How many servings did you have? One serving is %g %s or %g mL. Please enter a number.\n", diaryArray[diaryTracker]->house_serving_size, diaryArray[diaryTracker]->house_serving_size_unit, newEntry->serving_size);
            }

            scanf("%f%*c", &diaryArray[diaryTracker]->servings);
            //checking for user error where they don't enter a number
            while (isdigit(diaryArray[diaryTracker]->servings) != 0) {
                printf("Sorry, that is not a valid option. Please enter a number.\n");
                scanf("%f%*c", &diaryArray[diaryTracker]->servings);
            }
            
            //gets the right number of calories depending on the amount of servings entered by the user
            diaryArray[diaryTracker]->calories *= diaryArray[diaryTracker]->servings;
            diaryArray[diaryTracker]->carbs *= diaryArray[diaryTracker]->servings;
            diaryArray[diaryTracker]->fat *= diaryArray[diaryTracker]->servings;
            diaryArray[diaryTracker]->protein *= diaryArray[diaryTracker]->servings;

            //checking what meal this entry should be logged under
            printf("Did you eat this for breakfast, lunch, dinner, or a snack?\n");
            char meal[40];
            scanf("%[^\n]%*c", meal);
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
                else {
                    strcpy(diaryArray[diaryTracker]->meal, "snack");
                    break;
                }
                //checking for user error
                printf("Sorry, that is not an option. Please choose breakfast, lunch, dinner, or snack.\n");
            }
            
            //writes this new value into the file, in addition to any old values
            diaryTracker++;
            writeToLog(filename, diaryArray);

        }

        //update previous entries (one at a time)
        else if (strcasestr(choice, "update") != NULL) {
            journal = fopen(filename, "r");
            //checking if the diary exists
            if (journal == NULL || diaryArray[0]->ID == -1) {
                printf("\nDiary is empty.\n");
                printf("\nHere are your options:\nView diary\nSearch for all foods containing keyword\nSearch for and add entry (search must be first word in food name)\nUpdate entry\nDelete entry\nQuit\n");
                printf("Enter your choice: ");
                scanf("%[^\n]%*c", choice);
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
                printf("\nWhich item would you like to update? Type in the full name of the food.\n");
                scanf("%[^\n]%*c", toUpdate);
                printf("What meal was this food logged under?\n");
                scanf("%[^\n]%*c", mealUpdate);

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
                    printf("\nHere are your options:\nView diary\nSearch for all foods containing keyword\nASearch for and add entry (search must be first word in food name)\nUpdate entry\nDelete entry\nQuit\n");
                    printf("Enter your choice: ");
                    scanf("%[^\n]%*c", choice);
                    continue;
                }

                //the only value that can be updated is the amount of servings, so that is prompted for
                else {
                    //if there is a household serving size
                    if (strcmp(diaryArray[tracker]->house_serving_size_unit, "none") != 0) {
                        printf("How many servings did you have? Previously, the value was %g %s.\n", diaryArray[tracker]->servings, diaryArray[tracker]->house_serving_size_unit);
                        //the amount of nutrients depends on the amount of servings, so these values are changed accordingly
                        diaryArray[tracker]->calories /= diaryArray[tracker]->servings;
                        diaryArray[tracker]->carbs /= diaryArray[tracker]->servings;
                        diaryArray[tracker]->fat /= diaryArray[tracker]->servings;
                        diaryArray[tracker]->protein /= diaryArray[tracker]->servings;
                        scanf("%f%*c", &diaryArray[tracker]->servings);
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
                        scanf("%f%*c", &diaryArray[tracker]->servings);
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
        else if (strcasestr(choice, "delete") != NULL) {
            journal = fopen(filename, "r");
            //checking if the journal file exists
            //if it does not exist, the choice menu comes up again
            if (journal == NULL || diaryArray[0]->ID == -1) {
                printf("\nDiary is empty.\n");
                printf("\nHere are your options:\nView diary\nSearch for all foods containing keyword\nSearch for and add entry (search must be first word in food name)\nUpdate entry\nDelete entry\nQuit\n");
                printf("Enter your choice: ");
                scanf("%[^\n]%*c", choice);
                continue;
            }
            else {
                //prints the current diary file, just so the user can see it
                printf("\nThis is your current diary.\n");
                while (fgets(temp, 10000, journal) != NULL) {
                    printf("%s", temp);
                }
                char toDelete[500];
                char mealDelete[50];
                //prompts the user for the name and meal of the entry to be deleted
                printf("Which item would you like to delete? Type in the full name.\n");
                scanf("%[^\n]%*c", toDelete);
                printf("What meal was this food logged under?\n");
                scanf("%[^\n]%*c", mealDelete);
                while (1) {
                    //makes sure the user enters a valid meal category
                    if (strcasecmp(mealDelete, "breakfast") == 0 || strcasecmp(mealDelete, "lunch") == 0 || strcasecmp(mealDelete, "dinner") == 0 || strcasecmp(mealDelete, "snack") == 0) {
                        break;
                    }
                    printf("Sorry, that is not an option. Please choose breakfast, lunch, dinner, or snack.\n");
                } 

                bool notThere = false;
                int tracker = 0;

                //checking for a list with only one entry where the entry has the entered values
                if (strcasecmp(diaryArray[0]->name, toDelete) == 0 && (diaryArray[1]->ID == -1) && strcasecmp(mealDelete, diaryArray[0]->meal) == 0) {
                    diaryArray[0] = initialize();
                    writeToLog(filename, diaryArray);
                    printf("\nHere are your options:\nView diary\nSearch for all foods containing keyword\nSearch for and add entry (search must be first word in food name)\nUpdate entry\nDelete entry\nQuit\n");
                    printf("Enter your choice: ");
                    scanf("%[^\n]%*c", choice);
                    continue;
                }

                //if the list only has one entry but it is not what the user entered
                else if ((strcasecmp(diaryArray[0]->name, toDelete) != 0 || strcasecmp(mealDelete, diaryArray[0]->meal) == 0) && diaryArray[1]->ID == -1) {
                    notThere = true;
                    printf("Food not found\n");
                }

                //checking if a node with the entered values exists in a list with more than one entry
                else {
                    while (diaryArray[tracker]->ID != -1) {
                        if (strcasecmp(diaryArray[tracker]->name, toDelete) == 0 && strcasecmp(mealDelete, diaryArray[tracker]->meal) == 0) {
                            break;
                        }
                        if (diaryArray[tracker + 1]->ID == -1) {
                            notThere = true;
                            printf("Food not found\n");
                            break;
                        }
                        tracker++;
                    }
                }

                //if no entry is found that matches the user input the choice menu comes up again
                if (notThere == true) {
                    printf("\nHere are your options:\nView diary\nSearch for all foods containing keyword\nSearch for and add entry (search must be first word in food name)\nUpdate entry\nDelete entry\nQuit\n");
                    printf("Enter your choice: ");
                    scanf("%[^\n]%*c", choice);
                    continue;
                }

                //otherwise the given node is deleted
                diaryArray[tracker] = initialize();
                diaryArray[tracker]->ID = -5;
            }

        }

        //quit
        else if (strcasestr(choice, "quit") != NULL) {
            break;
        }

        //if they entered an invalid option
        else {
            printf("Sorry, that is not an available option. Please choose to view, search, update, delete, or quit.\n");
        }

        printf("\nHere are your options:\nView diary\nSearch for all foods containing keyword\nSearch for and add entry (search must be first word in food name)\nUpdate entry\nDelete entry\nQuit\n");
        printf("Enter your choice: ");
        scanf("%[^\n]%*c", choice);
    }

    free(newEntry);
    //free(diaryArray);
    return;
}