#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "catalog.h"

#define VALID_RANGE(I, B, C) !(I >= B && I <= C)

struct catalog {
    int depth;
    int size;
    int range;
    int total_size;
    int lower_limit;
    void** values;
    void (*free_func)(void*);
};

int int_pow(int value, int power){
    int acum = 1, i;
    for(i = 0; i < power; i++){
        acum *= value;
    }
    return acum;
}

int hash(Catalog c, char* code){
    int index = 0, i;
    int numeric = atoi(code + c->depth);
    for(i = 0; !VALID_RANGE(code[i],65,90) ; i++){
        index += ((code[i] - 65) * int_pow(c->size,c->depth - (i+1)));
    }
    index *= c->range;
    index += numeric - c->lower_limit;
    return index;
}

Catalog init_catalog(int size, int depth, int range, int lower_limit, void (*f)(void*)){
    int i;
    Catalog cat = malloc(sizeof(struct catalog));
    cat->size = size;
    cat->depth = depth;
    cat->total_size = 1 + (int_pow(size,depth) * range);
    cat->lower_limit = lower_limit;
    cat->range = range;
    cat->free_func = f;
    cat->values = malloc(sizeof(void*) * cat->total_size);
    for(i = 0; i < cat->total_size; i++) cat->values[i] = NULL;

    /*printf("%d %d %d\n", hash(cat, "AA1000"), hash(cat, "BB1000"), hash(cat, "ZZ9000"));*/
    /*printf("%d %d %d\n", hash(cat, "Z5000"), hash(cat, "A1000"), hash(cat, "B1000"));*/

    return cat;
}

void destroy_catalog(Catalog c){
    int i;
    for(i = 0; i < c->total_size; i++){
        if(c->values[i]) c->free_func(c->values[i]);
    }
    free(c->values);
    free(c);
}

/* Returns NULL if the key didn't existed, returns the value associated with the key oherwise */
void* catalog_get(Catalog c, char* string_code){
    int index = hash(c, string_code);

    return c->values[index];
}

/* Returns 0 if the code exists in the catalog and returns 1 if the code didn't existed */
int catalog_exists(Catalog c, char* string_code){
    int index = hash(c, string_code);

    return (c->values[index] == NULL);
}

/* Returns 0 if the insertion was sucessful */
int catalog_insert(Catalog c, char* string_code, void* value){
    int index = hash(c, string_code);
    c->values[index] = value;

    return 0;
}

/* Returns 0 if the removal was sucessful */
int catalog_remove(Catalog c, char* string_code){
    int index = hash(c, string_code);
    c->values[index] = NULL;

    return 0;
}

/* Performs a traverse through all catalog */
void catalog_foreach(Catalog c, gboolean (*traverse_func)(void*, void*, void*), void* data){
    int i, *numeric_value = malloc(sizeof(int));
    *numeric_value = 0;

    for(i = 0; i < c->total_size; i++){
        if(c->values[i]) traverse_func(numeric_value, c->values[i], data);
    }
    free(numeric_value);
}

/* Performs a traverse through one section of the catalog */
void catalog_foreach_in_letter_range(Catalog c, char letter, gboolean (*traverse_func)(void*, void*, void*), void* data){
    int i, overhead = c->range * (letter-65), *numeric_value = malloc(sizeof(int));
    *numeric_value = 0;

    for(i = 0; i < c->range * c->size; i++){
        if(c->values[i]) traverse_func(numeric_value, c->values[i+overhead], data);
    }
    free(numeric_value);
}

/*
int main(int argc, char** argv){
    Catalog c = init_catalog(26, 2, 9000, 1000);

    catalog_insert(c, "AA1234", "AA1234");

    destroy_catalog(c);

    return 0;
}*/
