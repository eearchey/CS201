#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Food {
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

    struct Food *next;
};

int main (int argc, char *argv[]) {
    /*if (argc != 4) {
        printf("Usage: productsfile nutrientsfile servingsizefile");
    }*/

    FILE *nutrients;
    FILE *products;
    FILE *serving_size;

    products = fopen(argv[1], "r");
    struct Food *head = NULL;
    head = (struct Food*)malloc(sizeof(struct Food));
    string temp[100];
    fgets(temp, 100, products);

    return 0;
}