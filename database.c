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
} Food;

int main (int argc, char *argv[]) {
    /*if (argc != 4) {
        printf("Usage: productsfile nutrientsfile servingsizefile");
    }*/

    FILE *nutrients;
    FILE *products;
    FILE *serving_size;

    products = fopen(argv[1], "r");
    Food *head = NULL;
    head = (struct Food*)malloc(sizeof(struct Food));
    char temp[1000];

    //getting the titles of the columns out of the way
    fgets(temp, 105, products);

    //now we enter the first food's number
    fscanf(products, "%d", &head->NDB_number);
    printf("%d \n", head->NDB_number);

    //now the name
    fscanf(products, "%s", temp);
    while (strcmp(temp, "LI") != 0) {
        strcat(head->name, temp);
        strcat(head->name, " ");
        fscanf(products, "%s", temp);
    }

    printf("%s \n", head->name);


    return 0;
}