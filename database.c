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
            if ((strcmp(cur->name, checker->name) == 0) || (cur->name[0] < checker->name[0])) {
                if (checker->leftChild == NULL) {
                    checker->leftChild = cur;
                    break;
                }
                else {
                    checker = checker->leftChild;
                }
            } 
            //iif the first letter of the new node is after that of the first letter of the current node, go down to the right child or insert the new node as the right child
            else if (cur->name[0] > checker->name[0]) {
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
Food *readFromPrevJournal(Food *first, char filename[]) {
    first = NULL;
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

    //returns the first node of the linked list that creates the journal
    return first;
}

//this function creates the actual name.log file, using the linked list that is passed in
void writeToLog(char filename[], Food* head) {
    //creates a new file
    FILE *journal = fopen(filename, "w+");
    Food *current;
    current = (struct Food*)malloc(sizeof(struct Food));
    current = head;

    //entering the breakfast entries
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
            fprintf(journal, "Number of servings: %g\n", current->servings);
        }
        current = current->nextFood;
    }

    //entering the lunch entries
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
            fprintf(journal, "Number of servings: %g\n", current->servings);
        }
        current = current->nextFood;
    }

    //entering the dinner entries
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
            fprintf(journal, "Number of servings: %g\n", current->servings);
        }
        current = current->nextFood;
    }

    //entering the snack entries
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
            fprintf(journal, "Number of servings: %g\n", current->servings);
        }
        current = current->nextFood;
    }
    //closing the file
    fclose(journal);
    return;
}

//this is a non-linear search and add function that finds the node described by the user input then returns it for adding
Food *addEntry(Food *cur, char food[], char brand[], float cals) {
    //if the entire tree has been recursed down and the node does not exist, this returns with an error message
    if (cur == NULL) {
        printf("Food not found\n");
        return NULL;
    }
    //if the node is found, it returns
    else if ((strcasestr(cur->name, food) != NULL) && (strcasestr(cur->manufacturer, brand) != NULL) && cur->calories == cals) {
        return cur;
    }

    //if the foods have the exact same name the node was added as a left child, so go down the left child
    if (strcmp(food, cur->name) == 0) {
        cur = addEntry(cur->leftChild, food, brand, cals);
    }

    //I believe all the names of the food are in uppercase, but just in case I checked if the node's name is in uppercase
    else if (isupper(cur->name[0])) {
        //checks for alphabetical order in the same way the nodes were inserted in createTree function (recursing down)
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
            cur = addEntry(cur->leftChild, food, brand, cals);
        }
        else {
            cur = addEntry(cur->rightChild, food, brand, cals);
        }
    }
    else {
        //if the letters aren't in uppercase
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
            cur = addEntry(cur->leftChild, food, brand, cals);
        }
        else {
            cur = addEntry(cur->rightChild, food, brand, cals);
        }
    }
    return cur;
}

//this is a linear search function that goes through the entire tree, printing the values that match what was input for the name and/or brand
void search(Food *cur, char food[], char brand[]) {
    //returns when a leaf is reached
    if (cur == NULL) {
        return;
    }
    //if the user didn't know the brand when prompted
    else if (strcmp(brand, "unknown") == 0) {
            //prints when a match is found
            if (strcasestr(cur->name, food) != NULL) {
            printf("Food: %s, Brand: %s, Calories: %g kcal\n", cur->name, cur->manufacturer, cur->calories);
            }
    }
    
    //prints when a match is found
    else if ((strcasestr(cur->name, food) != NULL) && (strcasestr(cur->manufacturer, brand) != NULL)) {
        printf("Food: %s, Brand: %s Calories: %g kcal\n", cur->name, cur->manufacturer, cur->calories);
    }
    //recurses down the tree
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
    //this is a yes/no string that I use multiple times across view, add etc., so I declare it up top
    char yn[10];
    char temp[1000];
    //here i'm allocating memory for the various temporary nodes i use in the sections for add, update, etc. so that they can easily be freed at the end of the program
    Food *head = NULL; 
    head = (struct Food*)malloc(sizeof(struct Food));
    head->leftChild = head->rightChild = head->nextFood = NULL;
    Food *cur = NULL;
    cur = (struct Food*)malloc(sizeof(struct Food));
    cur->leftChild = cur->rightChild = NULL;
    Food *newEntry = NULL;
    newEntry = (struct Food*)malloc(sizeof(struct Food));
    newEntry->nextFood = NULL;

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
            head = readFromPrevJournal(head, filename);
        }
    }

    //opening choice menu
    printf("\nHi %s!\n", name);
    printf("Here are your options:\nView diary\nSearch for food\nAdd entry\nUpdate entry\nDelete entry\nQuit\n");
    printf("Enter your choice: ");
    //reading in what the user's choice is
    char choice[50];
    scanf("%[^\n]%*c", choice);

    while (1) {
        //view diary
        if (strcasestr(choice, "view") != NULL) {
            journal = fopen(filename, "r");
            //if nothing has been written into the file, a message saying the diary is empty is delivered and the choice menu is printed again
            if (journal == NULL) {
                printf("\nDiary is empty.\n");
                printf("\nHere are your options:\nView diary\nSearch for food\nAdd entry\nUpdate entry\nDelete entry\nQuit\n");
                printf("Enter your choice: ");
                scanf("%[^\n]%*c", choice);
                continue;
            }
            //otherwise, the diary is printed
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
                search(root, food, brand);
            }
            //if not, just the name of the food is searched for
            else {
                printf("What did you eat?\n");
                scanf("%[^\n]%*c", food);
                printf("\n");
                search(root, food, "unknown");
            }
        }

        //add entry to diary
        else if (strcasestr(choice, "add") != NULL) {
            //this prompts the user for the name, brand, and calorie counts of the food, as those are required to recurse down the tree in a non-linear search (lg(n) time)
            printf("Do you know the name, brand, and calorie count of the food? Enter yes or no (no will return you to the choice selection, where you can use the search function to find this information).\n");
            char understood[10];
            scanf("%[^\n]%*c", understood);

            //if they do not know these values, the choice menu comes up again
            if (strcasecmp(understood, "yes") != 0) {
                printf("\nHere are your options:\nView diary\nSearch for food\nAdd entry\nUpdate entry\nDelete entry\nQuit\n");
                printf("Enter your choice: ");
                scanf("%[^\n]%*c", choice);
                continue;
            }

            //if they do know, they are prompted for these values
            printf("Enter the FULL name of the food and the brand, as well as the calorie count for a serving.\n");
            printf("Food name: ");
            char foodName[1000];
            scanf("%[^\n]%*c", foodName);
            printf("Brand name: ");
            char brandName[1000];
            scanf("%[^\n]%*c", brandName);
            printf("Calorie count: ");
            float cals;
            scanf("%f%*c", &cals);

            //searches for the food based on these values using the addEntry function
            cur = addEntry(root, foodName, brandName, cals);

            //if the found isn't found, an error message is printed in the function, and the choice menu appears again
            if (cur == NULL) {
                printf("\nHere are your options:\nView diary\nSearch for food\nUpdate entry\nDelete entry\nQuit\n");
                printf("Enter your choice: ");
                scanf("%[^\n]%*c", choice);
                continue;
            }

            //the data values from the returned node (if the food is found) are copied into a new node, so that if a user wants to add the same food twice that it doesn't have pointers to the same value and crashes the linked list
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
            strncpy(newEntry->house_serving_size_unit, cur->house_serving_size_unit, strlen(cur->house_serving_size_unit) - 1);
            newEntry->leftChild = newEntry->rightChild = NULL;
            
            //this prints the correct serving size, with household and grams or mL depending on which it had in the database
            if (newEntry->isGrams == true) {
                printf("How many servings did you have? One serving is %g %s or %g grams. Please enter a number.\n", newEntry->house_serving_size, newEntry->house_serving_size_unit, newEntry->serving_size);
            }
            else {
                printf("How many servings did you have? One serving is %g %s or %g mL. Please enter a number.\n", newEntry->house_serving_size, newEntry->house_serving_size_unit, newEntry->serving_size);
            }

            scanf("%f%*c", &newEntry->servings);
            //checking for user error where they don't enter a number
            while (isdigit(newEntry->servings) != 0) {
                printf("Sorry, that is not a valid option. Please enter a number.\n");
                scanf("%f%*c", &newEntry->servings);
            }
            
            //gets the right number of calories depending on the amount of servings entered by the user
            newEntry->calories *= newEntry->servings;
            newEntry->carbs *= newEntry->servings;
            newEntry->fat *= newEntry->servings;
            newEntry->protein *= newEntry->servings;

            newEntry->nextFood = head;
            head = newEntry;

            //checking what meal this entry should be logged under
            printf("Did you eat this for breakfast, lunch, dinner, or a snack?\n");
            char meal[40];
            scanf("%[^\n]%*c", meal);
            while (1) {
                if (strcasecmp(meal, "breakfast") == 0) {
                    strcpy(head->meal, "breakfast");
                    break;
                }

                else if (strcasecmp(meal, "lunch") == 0) {
                    strcpy(head->meal, "lunch");
                    break;
                }
                else if (strcasecmp(meal, "dinner") == 0) {
                    strcpy(head->meal, "dinner");
                    break;
                }
                else {
                    strcpy(head->meal, "snack");
                    break;
                }
                //checking for user error
                printf("Sorry, that is not an option. Please choose breakfast, lunch, dinner, or snack.\n");
            }

            //writes this new value into the file, in addition to any old values
            writeToLog(filename, head);

        }

        //update previous entries (one at a time)
        else if (strcasestr(choice, "update") != NULL) {
            journal = fopen(filename, "r");
            //checking if the diary exists
            if (journal == NULL || head == NULL) {
                printf("\nDiary is empty.\n");
                printf("\nHere are your options:\nView diary\nSearch for food\nAdd entry\nUpdate entry\nDelete entry\nQuit\n");
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

                cur = head;
                bool notThere = false;

                //checking if the inputted values match an actual entry in the diary
                while (cur != NULL) {
                    if (strcasecmp(cur->name, toUpdate) == 0 && strcasecmp(cur->meal, mealUpdate) == 0) {
                        break;
                    }
                    if (cur->nextFood == NULL) {
                        notThere = true;
                        printf("Food not found\n");
                        break;
                    }
                }

                //if the entry isn't found, the choice menu comes up again
                if (notThere == true) {
                    printf("\nHere are your options:\nView diary\nSearch for food\nAdd entry\nUpdate entry\nDelete entry\nQuit\n");
                    printf("Enter your choice: ");
                    scanf("%[^\n]%*c", choice);
                    continue;
                }

                //the only value that can be updated is the amount of servings, so that is prompted for
                else {
                    //if there is a household serving size
                    if (strcmp(cur->house_serving_size_unit, "none") != 0) {
                        printf("How many servings did you have? Previously, the value was %g %s.\n", cur->servings, cur->house_serving_size_unit);
                        //the amount of nutrients depends on the amount of servings, so these values are changed accordingly
                        cur->calories /= cur->servings;
                        cur->carbs /= cur->servings;
                        cur->fat /= cur->servings;
                        cur->protein /= cur->servings;
                        scanf("%f%*c", &cur->servings);
                        cur->calories *= cur->servings;
                        cur->carbs *= cur->servings;
                        cur->fat *= cur->servings;
                        cur->protein *= cur->servings;
                        //these new, updated values are written to the diary
                        writeToLog(filename, head); 
                    }
                    else {
                        //if there is no household serving size, but functions in the same way as above
                        if (cur->isGrams == true) {
                            printf("How many servings did you have? Previously, the value was %g grams.\n", cur->servings);
                        }
                        else {
                            printf("How many servings did you have? Previously, the value was %g mL.\n", cur->servings);
                        }
                        cur->calories /= cur->servings;
                        cur->carbs /= cur->servings;
                        cur->fat /= cur->servings;
                        cur->protein /= cur->servings;
                        scanf("%f%*c", &cur->servings);
                        cur->calories *= cur->servings;
                        cur->carbs *= cur->servings;
                        cur->fat *= cur->servings;
                        cur->protein *= cur->servings;
                        writeToLog(filename, head);
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
            if (journal == NULL || head == NULL) {
                printf("\nDiary is empty.\n");
                printf("\nHere are your options:\nView diary\nSearch for food\nAdd entry\nUpdate entry\nDelete entry\nQuit\n");
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

                cur = head;
                bool notThere = false;

                //checking for a list with only one entry where the entry has the entered values
                if (strcasecmp(head->name, toDelete) == 0 && head->nextFood == NULL && strcasecmp(mealDelete, head->meal) == 0) {
                    head = NULL;
                    writeToLog(filename, head);
                    printf("\nHere are your options:\nView diary\nSearch for food\nAdd entry\nUpdate entry\nDelete entry\nQuit\n");
                    printf("Enter your choice: ");
                    scanf("%[^\n]%*c", choice);
                    continue;
                }

                //if the list only has one entry but it is not what the user entered
                else if ((strcasecmp(head->name, toDelete) != 0 || strcasecmp(mealDelete, head->meal) == 0) && head->nextFood == NULL) {
                    notThere = true;
                    printf("Food not found\n");
                }

                //checking if a node with the entered values exists in a list with more than one entry
                else {
                    while (cur != NULL) {
                        if (cur->nextFood != NULL && strcasecmp(cur->nextFood->name, toDelete) == 0 && strcasecmp(mealDelete, cur->meal) == 0) {
                            break;
                        }
                        if (cur->nextFood == NULL) {
                            notThere = true;
                            printf("Food not found\n");
                            break;
                        }
                    }
                }

                //if no entry is found that matches the user input the choice menu comes up again
                if (notThere == true) {
                    printf("\nHere are your options:\nView diary\nSearch for food\nAdd entry\nUpdate entry\nDelete entry\nQuit\n");
                    printf("Enter your choice: ");
                    scanf("%[^\n]%*c", choice);
                    continue;
                }

                //otherwise the given node is deleted
                newEntry = cur->nextFood->nextFood;
                free(cur->nextFood);
                cur->nextFood = newEntry;
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

        printf("\nHere are your options:\nView diary\nSearch for food\nUpdate entry\nDelete entry\nQuit\n");
        printf("Enter your choice: ");
        scanf("%[^\n]%*c", choice);
    }

    //free(head);
    //free(newEntry);
    //free(cur);
    return;
}