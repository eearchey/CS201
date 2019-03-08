#include <stdio.h>
#include <string.h>
#include <math.h>
#include "database.h"


int main (int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: database_file\n");
    }

    char fileName[100];
    strcpy(fileName, argv[1]);

    createTree(fileName);




    return 0;
}