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

    printf("Who are you?\n");
    char name[100];
    scanf("%s", name);

    editJournal(name);

    return 0;
}