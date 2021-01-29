#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "config.h"

#define NUM_BRANCHES 3
#define NUM_MESES    12
#define NUM_TYPES    2
#define PRODUCTS_DEPTH       2
#define NUM_LETTERS          26
#define CUSTOMERS_DEPTH      1
#define PATH_VENDAS          "Files/Vendas_1M.txt"
#define PATH_PRODUCTS        "Files/Produtos.txt"
#define PATH_CUSTOMERS       "Files/Clientes.txt"
#define PATH_STATS           "Stats"
#define PATH_RESULTS         "Stats/Results"

struct config{
    int num_branches, num_meses, num_types;
    char *salesPath, *customersPath, *productsPath;
    char *statsPath, *resultsPath;
    int num_letters, products_depth, customers_depth;
};

typedef struct config *Config;

Config c;

void initConfig(char* pathToConfig){
    c = malloc(sizeof(struct config));

    /* Setup the standard */
    c->num_branches = NUM_BRANCHES;
    c->num_meses = NUM_MESES;
    c->num_types = NUM_TYPES;
    c->salesPath = my_strdup(PATH_VENDAS);
    c->customersPath = my_strdup(PATH_CUSTOMERS);
    c->productsPath = my_strdup(PATH_PRODUCTS);
    c->statsPath = my_strdup(PATH_STATS);
    c->resultsPath = my_strdup(PATH_RESULTS);
    c->num_letters = NUM_LETTERS;
    c->products_depth = PRODUCTS_DEPTH;
    c->customers_depth = CUSTOMERS_DEPTH;

    /* If there is a config file then proceed to parse it's information */
    if(pathToConfig){
        int counter = 0;
        FILE* f = fopen(pathToConfig, "r");
        char* buffer = malloc(sizeof(char) * 100);
        char** array = malloc(sizeof(char*) * 100);

        /* Read the whole file and store it in the array */
        while(fgets(buffer, 100, f)){
            int last = strcspn(buffer, "\n\0");
            if(last > 3 && counter < 99 && (buffer[last] == '\n' || buffer[last] == '\0')){
                buffer[last] = 0;
                array[counter++] = my_strdup(buffer);
            }
        }
        array[counter] = 0;

        /* Re-parse all possible options */
        char* value;
        value = getSuffixOf("NUM_BRANCHES", array); if(value) { c->num_branches = atoi(value); free(value); }
        value = getSuffixOf("NUM_MESES", array); if(value) { c->num_meses = atoi(value); free(value); }
        value = getSuffixOf("NUM_TYPES", array); if(value) { c->num_types = atoi(value); free(value); }
        value = getSuffixOf("NUM_LETTERS", array); if(value) { c->num_letters = atoi(value); free(value); }
        value = getSuffixOf("CUSTOMERS_DEPTH", array); if(value) { c->customers_depth = atoi(value); free(value); }
        value = getSuffixOf("PRODUCTS_DEPTH", array); if(value) { c->products_depth = atoi(value); free(value); }
        value = getSuffixOf("SALES_PATH", array); if(value) { free(c->salesPath); c->salesPath = my_strdup(value); free(value); }
        value = getSuffixOf("PRODUCTS_PATH", array); if(value) { free(c->productsPath); c->productsPath = my_strdup(value); free(value); }
        value = getSuffixOf("CUSTOMERS_PATH", array); if(value) { free(c->customersPath); c->customersPath = my_strdup(value); free(value); }
        value = getSuffixOf("STATS_PATH", array); if(value) { free(c->statsPath); c->statsPath = my_strdup(value); free(value); }
        value = getSuffixOf("RESULTS_PATH", array); if(value) { free(c->resultsPath); c->resultsPath = my_strdup(value); free(value); }

        for(counter = 0; array[counter]; counter++) free(array[counter]);
        free(array);
        free(buffer);
    }
}

void destroyConfig(){
    if(c->salesPath) free(c->salesPath);
    if(c->productsPath) free(c->productsPath);
    if(c->customersPath) free(c->customersPath);
    if(c->statsPath) free(c->statsPath);
    if(c->resultsPath) free(c->resultsPath);
    free(c);
}

/* --------------------------------------- CONFIG PUBLIC INTERFACE ------------------------------------------- */
int get_num_branches(){ return c->num_branches; }

int get_num_meses(){ return c->num_meses; }

int get_num_types(){ return c->num_types; }

int get_num_letters(){ return c->num_letters; }

int get_products_depth(){ return c->products_depth; }

int get_customers_depth(){ return c->customers_depth; }

char* get_sales_file_name(){
    int nTokens, i;
    char** splitted = words(c->salesPath, '/', &nTokens);
    char* ret = my_strdup(splitted[nTokens-1]);
    for(i = 0; i < nTokens; i++) free(splitted[i]);
    free(splitted);
    return ret;
}

char* get_customers_file_name(){
    int nTokens, i;
    char** splitted = words(c->customersPath, '/', &nTokens);
    char* ret = my_strdup(splitted[nTokens-1]);
    for(i = 0; i < nTokens; i++) free(splitted[i]);
    free(splitted);
    return ret;
}

char* get_products_file_name(){
    int nTokens, i;
    char** splitted = words(c->productsPath, '/', &nTokens);
    char* ret = my_strdup(splitted[nTokens-1]);
    for(i = 0; i < nTokens; i++) free(splitted[i]);
    free(splitted);
    return ret;
}

char* get_sales_path(){ return my_strdup(c->salesPath); }

char* get_customers_path(){ return my_strdup(c->customersPath); }

char* get_products_path(){ return my_strdup(c->productsPath); }

char* get_stats_path(){ return my_strdup(c->statsPath); }

char* get_results_path(){ return my_strdup(c->resultsPath); }

/* Useful functions all around */

/* Frees a depth 1 array */
void depth1_free(void* ptr){
    void** actual_ptr = (void**) ptr;
    int i;
    for(i = 0; actual_ptr[i]; i++) free(actual_ptr[i]);
    free(actual_ptr);
}

/* Frees a g_ptr_array with option TRUE */
void free_gptr_array(void* ptr){
    GPtrArray* array = (GPtrArray*) ptr;
    g_ptr_array_free(array, TRUE);
}

/* Function used to turn a GTree into a GArray */
gboolean insert_in_array(gpointer key, gpointer value, gpointer data){
    GPtrArray* array = (GPtrArray*) data;
    char* str = my_strdup((char*) value);
    g_ptr_array_add(array, str);

    return FALSE;
}

/* Function to compare floats with the same type of return values of strcmp */
int compare_floats_two_args(const void* gpa, const void* gpb){
    float a = *((float*) gpa);
    float b = *((float*) gpb);
    return (a > b);
}

/* Function to compare int with the same type of return values of strcmp */
int compare_ints_two_args(const void* gpa, const void* gpb){
    int a = *((int*) gpa);
    int b = *((int*) gpb);
    return (a-b);
}

/* Function to compare int with the same type of return values of strcmp */
int compare_ints(const void* gpa, const void* gpb, void* something){
    int a = *((int*) gpa);
    int b = *((int*) gpb);
    return (a-b);
}

/* Function to compare strings with the same type of return values of strcmp */
int compare_strings(const void* gpa, const void* gpb, void* something){
    char* sa = (char*) gpa;
    char* sb = (char*) gpb;
    return strcmp(sa,sb);
}

/* Function to compare strings with the same type of return values of strcmp */
int compare_strings_two_args(const void* gpa, const void* gpb){
    char* sa = (char*) gpa;
    char* sb = (char*) gpb;
    return strcmp(sa,sb);
}

/* Same purpose as strdup (only done since ANSI doesn't allow strdup) */
char* my_strdup(const char* string){
    size_t len = strlen(string) + 1;
    char* ret = malloc(len);
    if(ret == NULL) return NULL;
    memcpy(ret, string, len);
    return ret;
}

char** words(const char* line, char regex, int* nTokens)
{
    char regexAsString[1] = {regex};
    char* input = my_strdup(line);
    size_t word_count = 1;
    while (*line) {
        if (*line == regex) {
            while (*line && *line == regex) {
                line++;
            }
            word_count++;
        } else {
            line++;
        }
    }
    char** words = malloc((word_count + 1) * sizeof(char*) + 1);
    size_t word = 0;
    char* token;
    for (token = strtok(input, regexAsString); token; token = strtok(NULL, regexAsString)) {
        words[word++] = my_strdup(token);
    }
    words[word] = NULL;
    (*nTokens) = word;
    free(input);
    return words;
}

int contains(char compare, char* regex){
    int i;
	for(i = 0; regex[i]; i++)
		if(regex[i] == compare) return 0;
	return 1;
}

int existsCode(char* code, char** lookIn){
    int i;
    for(i = 0; lookIn[i]; i++){
        if(!strcmp(lookIn[i], code)) return 0;
    }
    return 1;
}

char* getSuffixOf(char* prefix, char** lookIn){
    int i, j;
    char* ret = NULL;
    for(i = 0; lookIn[i]; i++){
        /* Split the field we are looking at by the separator '=' */
        int nTokens;
        char** splitted = words(lookIn[i], '=', &nTokens);
        if(nTokens == 2 && !strcmp(prefix, splitted[0])) ret = my_strdup(splitted[1]);
        for(j = 0; splitted[j]; j++) free(splitted[j]);
        free(splitted);
        if(ret) break;
    }

    return ret;
}
