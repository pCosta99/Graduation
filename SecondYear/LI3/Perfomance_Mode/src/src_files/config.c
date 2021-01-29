#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

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
