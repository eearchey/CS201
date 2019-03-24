#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "database.h"


int main (int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./port database_file\n");
    }

    char fileName[100];
    strcpy(fileName, argv[1]);
    Food *root = NULL;
    root = (struct Food*)malloc(sizeof(struct Food));
    root->leftChild = root->rightChild = NULL;
    createTree(fileName, root);

    printf("Welcome to your diary!\n");

    printf("Who are you?\n");
    char name[100];
    scanf("%[^\n]%*c", name);

    while (1) {
        editJournal(name, root);
        printf("If you want to edit or view another user's journal, enter their name here. If not, then enter quit again.\n");
        scanf("%s", name); 
        if (strcasestr(name, "quit") != NULL) {
            break;
        }
    }

    return 0;
}