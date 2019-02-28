#include <stdio.h>
#include <string.h>
#include <math.h>

#include "database.h"


int main (int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: productsfile servingsizefile nutrientsfile");
    }

    char productsFile[20];
    char serving_sizeFile[20];
    char nutrientsFile[20];

    strcpy(productsFile, argv[1]);
    strcpy(serving_sizeFile, argv[2]);
    strcpy(nutrientsFile, argv[3]);

    createList(productsFile, serving_sizeFile, nutrientsFile);




    return 0;
}