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
    char serving_size_unit[100];


    struct Food *leftChild;
    struct Food *rightChild;
} Food;

#endif

void createTree(char[]); 