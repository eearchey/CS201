#ifndef DATABASE_H
#define DATABASE_H

typedef struct Food {
    char name[200];
    int NDB_number;
    char manufacturer[100];
    int householdServingSize;
    char servingSizeUnit[50];

    //nutrients
    int protein;
    int fat;
    int sugars;
    int calories;
    int sodium;

    struct Food *next;
} Food;

#endif

void createList(char[], char[], char[]); 