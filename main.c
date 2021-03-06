#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "database.h"


int main (int argc, char *argv[]) {
    //making sure the user runs the file with the correct command
    if (argc != 2) {
        printf("Usage: ./port database_file\n");
    }

    printf("Loading...\n");
    //creates the BST that stores the food data from the USDA website
    char fileName[100];
    strcpy(fileName, argv[1]);
    
    Food *root = NULL;
    root = (struct Food*)malloc(sizeof(struct Food));
    root->leftChild = root->rightChild = NULL;
    createTree(fileName, root);

    printf("Welcome to your diary!\n");

    //asks for user name
    printf("Who are you? Enter your name, then hit enter.\n");
    char name[100];
    scanf("%100[^\n]%*c", name);
    //taken from https://stackoverflow.com/questions/30065675/what-does-scanf-nc-mean and used in various places throughout the code 

    while (1) {
        //lets the user edit their journal
        editJournal(name, root);
        printf("If you want to create a new journal, edit, or view another user's journal, enter their name here. If not, then enter quit again.\n");
        //checks if the user wants to edit another journal, or exits
        scanf("%100[^\n]%*c", name);
        if (strcasecmp(name, "quit") == 0) {
            break;
        }
    }

    free(root);
    root = NULL;
    return 0;
}