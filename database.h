#include <stdbool.h>
#ifndef DATABASE_H
#define DATABASE_H

//this is the data structure for the nodes in the BST and the linked list used to store the entries in the diary
typedef struct Food {
    //not necessary for the diary program, but i kept it nonetheless
    int ID;

    char name[500];
    char manufacturer[500];
    float calories;
    float carbs;
    float fat;
    float protein;
    //serving size in grams or mL
    float serving_size;
    bool isGrams;
    //serving size that makes sense to people (e.g. piece, sticks, etc.)
    float house_serving_size;
    char house_serving_size_unit[1000];
    //amount of servings eaten
    float servings;
    //meal that that entry should go under. only acceptable choices are "breakfast", "lunch", "dinner", and "snacks" (or in caps)
    char meal[20];

    //children for storing the node in the BST
    struct Food *leftChild;
    struct Food *rightChild;

} Food;

#endif

//this function creates the binary search tree that holds the information for each food item
void createTree(char[], Food*);
//this function gets the data from food_database.csv and creates a node with all of the information, then returns it
Food *addData(Food*, char[]);
//this function does the actual editing of the journal for each person
void editJournal(char[], Food*);
//this function initializes structs to have all integer/float values equal to -1 and all string values equal to "empty"
Food *initialize();
//this is a linear search function that goes through the entire tree, printing the values that match the keyword for the name and/or brand. Only 20 are printed at a time, after which the user is asked if they want to continue or not. If they do not, the function stops
void search(Food*, char[], char[], int*);
//this function gets the array used for the results in addEntry
void getArray(Food *, Food *[], char[]);
//this is a non-linear search and add function that edits an array of nodes that match the search criteria
void addEntry(Food*, char[], Food *[], int*);
//this function creates the searchResults array in which the non-linear search results are placed
void getArray(Food *, Food *[], char[]);
//this function creates the actual name.log file, using the diaryArray array of nodes that contain the desired entries
void writeToLog(char[], Food *[]);
//this function goes through the previous journal file, throws away the breakfast, lunch, dinner, and snacks headers, and then goes to the getDataFromPrevJournal function to actually get the nodes for diaryArray
void readFromPrevJournal(Food *[], char[]);
//this function reads in the data from a previous journal, throws away words that aren't required, and adds them to a new node that it returns
Food *getDataFromPrevJournal(Food*, char[], FILE*);