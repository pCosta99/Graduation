#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "catalog.h"

#define VALID_NUMBER(I, B, C) !(I >= B && I <= C)

struct catalog {
    int depth;
    int size;
    int total_size;
    GTree** trees;
};

int int_pow(int value, int power){
    int acum = 1, i;
    for(i = 0; i < power; i++){
        acum *= value;
    }
    return acum;
}

Catalog init_catalog(int size, int depth, void* compare_func, void* free_key_func, void* free_value_func){
    int i, total_size = int_pow(size,depth);
    Catalog cat = malloc(sizeof(struct catalog));
    cat->trees = malloc(sizeof(GTree*) * total_size);
    cat->size = size;
    cat->depth = depth;
    cat->total_size = total_size;
    for(i = 0; i < total_size; i++){
        cat->trees[i] = g_tree_new_full(compare_func, NULL, free_key_func, free_value_func);
    }
    return cat;
}

void destroy_catalog(Catalog c){
    int i;
    for(i = 0; i < c->total_size; i++){
        g_tree_destroy(c->trees[i]);
    }
    free(c->trees);
    free(c);
}

int hash(Catalog c, char* letters){
    int index = 0, i;
    for(i = 0; !(VALID_NUMBER(letters[i],65,90)); i++){
        index += (letters[i]-65) * (int_pow(c->size,c->depth-(i+1)));
    }
    return index;
}

/* Returns the tree that corresponds to a set of letters */
GTree* getTree(Catalog c, char* code){
    int index = hash(c, code);

    return c->trees[index];
}

/* Returns NULL if the key didn't existed, returns the value associated with the key oherwise */
void* catalog_get(Catalog c, char* string_code){
    void* ret;
    int code = atoi(string_code + c->depth);

    GTree* tree = getTree(c, string_code);
    if(tree == NULL) ret = NULL;
    gpointer gcp = &code;
    ret = g_tree_lookup(tree, gcp);

    return ret;
}

/* Returns 0 if the code exists in the catalog and returns 1 if the code didn't existed */
int catalog_exists(Catalog c, char* string_code){
    int ret = 0, code = atoi(string_code + c->depth);

    GTree* tree = getTree(c, string_code);
    if(tree == NULL) return 1;
    gpointer gcp = &code;
    if(g_tree_lookup(tree, gcp) == NULL) ret = 2;

    return ret;
}

/* Returns 0 if the insertion was sucessful */
int catalog_insert(Catalog c, char* string_code, void* value){
    int* code = malloc(sizeof(int));
    (*code) = atoi(string_code + c->depth);

    GTree* tree = getTree(c, string_code);
    if(tree == NULL) return 1;
    g_tree_insert(tree, code, value);

    return 0;
}

/* Returns 0 if the removal was sucessful */
int catalog_remove(Catalog c, char* string_code){
    int code = atoi(string_code + c->depth);

    GTree* tree = getTree(c, string_code);
    if(tree == NULL) return 1;
    g_tree_remove(tree, &code);

    return 0;
}

/* Performs a traverse through all catalog */
void catalog_foreach(Catalog c, gboolean (*traverse_func)(void*, void*, void*), void* data){
    int i;

    for(i = 0; i < c->total_size; i++){
        g_tree_foreach(c->trees[i], traverse_func, data);
    }
}

/* Performs a traverse through one section of the catalog */
void catalog_foreach_in_letter_range(Catalog c, char letter, gboolean (*traverse_func)(void*, void*, void*), void* data){
    int i = c->size * (letter - 65), j;

    for(j = 0; j < c->size; j++){
        g_tree_foreach(c->trees[i+j], traverse_func, data);
    }
}

/*
gint compare_ints(gconstpointer gpa, gconstpointer gpb){
    int a = *((int*) gpa);
    int b = *((int*) gpb);
    return (a-b);
}

int main(int argc, char** argv){
    Catalog c = init_catalog(26, 1, compare_ints, free, free);

    destroy_catalog(c);

    return 0;
}*/
