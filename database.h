#include <stdbool.h>
#ifndef DATABASE_H
#define DATABASE_H

typedef struct Food {
    int ID;
    char name[200];
    char manufacturer[200];
    float calories;
    float carbs;
    float fat;
    float protein;
    float serving_size;
    bool isGrams;
    float house_serving_size;
    char house_serving_size_unit[1000];

    struct Food *leftChild;
    struct Food *rightChild;
} Food;

#endif

void createTree(char[], Food*);
void editJournal(char[], Food*);