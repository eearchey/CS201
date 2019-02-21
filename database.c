#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

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

Food *addProductData(char temp[]) {
    //we enter the first food's number
    Food *new = NULL;
    new = (struct Food*)malloc(sizeof(struct Food));
    char* token = strtok(temp, "\t"); 
    new->NDB_number = atoi(token);

    //entering the food's name
    token = strtok(NULL, "\t");
    strcpy(new->name, token);

    //skipping unnecessary data
    token = strtok(NULL, "\t");
    token = strtok(NULL, "\t");

    //entering the manufacturer's name
    token = strtok(NULL, "\t");
    strcpy(new->manufacturer, token);

    return new;
}

int main (int argc, char *argv[]) {
    /*if (argc != 4) {
        printf("Usage: productsfile nutrientsfile servingsizefile");
    }*/

    FILE *nutrients;
    FILE *products;
    FILE *serving_size;

    products = fopen(argv[1], "r");
    Food *head = NULL;
    Food *cur = NULL;
    head = (struct Food*)malloc(sizeof(struct Food));
    cur = (struct Food*)malloc(sizeof(struct Food));
    head->next = NULL;

    //getting the titles of the columns out of the way
    char tempy[1000];
    fgets(tempy, 105, products);

    char temp[1000];
    fgets(temp, 1000, products);
    head = addProductData(temp);
    printf("%d number \n %s name \n %s manufacturer \n", head->NDB_number, head->name, head->manufacturer);
    

    while (fgets(temp, 1000, products) != NULL) {
        cur = addProductData(temp);
        cur->next = head;
        head = cur;
        printf("%d number \n %s name \n %s manufacturer \n", head->NDB_number, head->name, head->manufacturer);
    }


    fclose(products);

    return 0;
}