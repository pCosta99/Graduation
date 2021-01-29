#ifndef __CONFIG__
#define __CONFIG__

#include <glib.h>

/* Interface to stuff defined in config file (or default in lack of one) */
void initConfig(char* pathToConfig);

void destroyConfig();

int get_num_branches();

int get_num_meses();

int get_num_types();

int get_num_letters();

int get_products_depth();

int get_customers_depth();

char* get_sales_file_name();

char* get_customers_file_name();

char* get_products_file_name();

char* get_sales_path();

char* get_customers_path();

char* get_products_path();

char* get_stats_path();

char* get_results_path();

/* Frees a depth 1 array */
void depth1_free(void* ptr);

/* Frees a g_ptr_array with option TRUE */
void free_gptr_array(void* ptr);

/* Function used to turn a GTree into a GArray */
gboolean insert_in_array(gpointer key, gpointer value, gpointer data);

/* Function to compare floats with the same type of return values of strcmp */
int compare_floats_two_args(const void* gpa, const void* gpb);

/* Function to compare int with the same type of return values of strcmp */
int compare_ints_two_args(const void* gpa, const void* gpb);

/* Function to compare int with the same type of return values of strcmp */
int compare_ints(const void* gpa, const void* gpb, void* something);

/* Function to compare strings with the same type of return values of strcmp */
int compare_strings(const void* gpa, const void* gpb, void* something);

/* Function to compare strings with the same type of return values of strcmp */
int compare_strings_two_args(const void* gpa, const void* gpb);

/* Same purpose as strdup (only done since ANSI doesn't allow strdup) */
char* my_strdup(const char* string);

/* Splits a line into several pieces separated by a regex */
char** words(const char* line, char regex, int* nWords);

/* Checks if a char exists in a string */
int contains(char compare, char* regex);

/* Checks if a code exists somewhere in the array of strings. 0 means yes */
int existsCode(char* code, char** lookIn);

/* Gets the suffix of a given preffix */
char* getSuffixOf(char* prefix, char** lookIn);

#endif
