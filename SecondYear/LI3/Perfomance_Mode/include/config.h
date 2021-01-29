#ifndef __CONFIG__
#define __CONFIG__

#include <gmodule.h>

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

/* Checks if a code exists somewhere in the array of strings*/
int existsCode(char* code, char** lookIn);

#endif
