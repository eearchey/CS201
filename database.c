#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct food {
    char name[200];
    int NDB_number;
    char manufacturer[100];
    int servingSize;
    char servingSizeUnit[50];

    //nutrients
    int protein;
    int fat;
    int sugars;
    int calories;
    int sodium;
};

int main (void) {

    return 0;
}