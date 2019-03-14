#include <stdio.h>
#include <string.h>
#include <math.h>
#include "database.h"


int main (int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./port database_file\n");
    }

    char fileName[100];
    strcpy(fileName, argv[1]);
    createTree(fileName);

    printf("Welcome to your diary!\n");
    printf("Options:\nView diary\nAdd entry\nUpdate entry\nDelete entry\n");
    printf("Enter your choice: ");
    char choice[50];
    scanf("%s", choice);

    if (strcasestr(choice, "view") != NULL) {
        printf("You've chosen to view!\n");
    }
    else if (strcasestr(choice, "add") != NULL) {
        printf("You've chosen to add!\n");
    }
    else if (strcasestr(choice, "update") != NULL) {
        printf("You've chosen to update!\n");
    }
    else if (strcasestr(choice, "delete") != NULL) {
        printf("You've chosen to delete!\n");
    }

    return 0;
}