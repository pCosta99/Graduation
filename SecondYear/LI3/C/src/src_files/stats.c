#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <glib.h>
#include "config.h"
#include "stats.h"

#define PATH_PARSING    "Parsing"
#define PATH_QUERIES    "Queries"

struct entry{
    struct timeval t1, t2;
    long total_time_spent;
    FILE* f;
    int markFile; /* If markfile = 0 the file will have a break on top of the result */
    char* commentary; /* If not null then the commentary will be added before the timestamp */
};

typedef struct entry *Entry;

Entry initEntry(FILE* f){
    Entry e = malloc(sizeof(struct entry));

    e->total_time_spent = 0;
    e->f = f;
    e->markFile = 1;
    e->commentary = NULL;

    return e;
}

Entry initEntryWithMarkfile(FILE* f){
    Entry e = malloc(sizeof(struct entry));

    e->total_time_spent = 0;
    e->f = f;
    e->markFile = 0;
    e->commentary = NULL;

    return e;
}

Entry initEntryWithCommentary(FILE* f, char* commentary){
    Entry e = malloc(sizeof(struct entry));

    e->total_time_spent = 0;
    e->f = f;
    e->markFile = 1;
    e->commentary = my_strdup(commentary);

    return e;
}
void freeEntry(void* ptr){
    Entry e = (Entry) ptr;
    if(e->f) fclose(e->f);
    if(e->commentary) free(e->commentary);
    free(e);
}

struct stats{
    GHashTable* filesTable;
    int active;
};

Stats init_stats(char* statsFolder, char* resultsFolder, int createDirectories, int active){
    char* totalPath = malloc(sizeof(char) * 200);
    char *key;
    char *temp = get_sales_file_name();

    Stats s = malloc(sizeof(struct stats));
    s->active = active;
    s->filesTable = g_hash_table_new_full(g_str_hash, g_str_equal, free, freeEntry);

    if(!createDirectories){
        sprintf(totalPath, "mkdir -p %s", statsFolder);
        system(totalPath);
        sprintf(totalPath, "mkdir -p %s", resultsFolder);
        system(totalPath);
        sprintf(totalPath, "mkdir -p %s/%s", statsFolder, temp);
        system(totalPath);
        sprintf(totalPath, "mkdir -p %s/%s/%s", statsFolder, temp, PATH_PARSING);
        system(totalPath);
        sprintf(totalPath, "mkdir -p %s/%s/%s", statsFolder, temp, PATH_QUERIES);
        system(totalPath);
        sprintf(totalPath, "mkdir -p %s/%s/%s/%s", statsFolder, temp, PATH_QUERIES, "Query11");
        system(totalPath);
    }

/* --------------------------- PARSING ------------------------------- */
    key = my_strdup("loadCatalogClientes");
    sprintf(totalPath, "%s/%s/%s/%s", statsFolder, temp, PATH_PARSING, key);
    Entry e = initEntry(fopen(totalPath, "a"));
    g_hash_table_insert(s->filesTable, key, e);

    key = my_strdup("loadCatalogProducts");
    sprintf(totalPath, "%s/%s/%s/%s", statsFolder, temp, PATH_PARSING, key);
    e = initEntry(fopen(totalPath, "a"));
    g_hash_table_insert(s->filesTable, key, e);

    key = my_strdup("loadVendas");
    sprintf(totalPath, "%s/%s/%s/%s", statsFolder, temp, PATH_PARSING, key);
    e = initEntryWithMarkfile(fopen(totalPath, "a"));
    g_hash_table_insert(s->filesTable, key, e);

    key = my_strdup("validaVendaWords");
    sprintf(totalPath, "%s/%s/%s/%s", statsFolder, temp, PATH_PARSING, key);
    e = initEntry(fopen(totalPath, "a"));
    g_hash_table_insert(s->filesTable, key, e);

    key = my_strdup("validaVendaCatalogExists");
    sprintf(totalPath, "%s/%s/%s/%s", statsFolder, temp, PATH_PARSING, key);
    e = initEntryWithCommentary(fopen(totalPath, "a"), "teste");
    g_hash_table_insert(s->filesTable, key, e);

    key = my_strdup("billProduct");
    sprintf(totalPath, "%s/%s/%s/%s", statsFolder, temp, PATH_PARSING, key);
    e = initEntry(fopen(totalPath, "a"));
    g_hash_table_insert(s->filesTable, key, e);

    key = my_strdup("customerBought");
    sprintf(totalPath, "%s/%s/%s/%s", statsFolder, temp, PATH_PARSING, key);
    e = initEntry(fopen(totalPath, "a"));
    g_hash_table_insert(s->filesTable, key, e);

/* --------------------------- QUERIES ------------------------------- */
    key = my_strdup("query2");
    sprintf(totalPath, "%s/%s/%s/%s", statsFolder, temp, PATH_QUERIES, key);
    e = initEntry(fopen(totalPath, "a"));
    g_hash_table_insert(s->filesTable, key, e);

    key = my_strdup("query3");
    sprintf(totalPath, "%s/%s/%s/%s", statsFolder, temp, PATH_QUERIES, key);
    e = initEntry(fopen(totalPath, "a"));
    g_hash_table_insert(s->filesTable, key, e);

    key = my_strdup("query4");
    sprintf(totalPath, "%s/%s/%s/%s", statsFolder, temp, PATH_QUERIES, key);
    e = initEntry(fopen(totalPath, "a"));
    g_hash_table_insert(s->filesTable, key, e);

    key = my_strdup("query5");
    sprintf(totalPath, "%s/%s/%s/%s", statsFolder, temp, PATH_QUERIES, key);
    e = initEntry(fopen(totalPath, "a"));
    g_hash_table_insert(s->filesTable, key, e);

    key = my_strdup("query6");
    sprintf(totalPath, "%s/%s/%s/%s", statsFolder, temp, PATH_QUERIES, key);
    e = initEntry(fopen(totalPath, "a"));
    g_hash_table_insert(s->filesTable, key, e);

    key = my_strdup("query7");
    sprintf(totalPath, "%s/%s/%s/%s", statsFolder, temp, PATH_QUERIES, key);
    e = initEntry(fopen(totalPath, "a"));
    g_hash_table_insert(s->filesTable, key, e);

    key = my_strdup("query8");
    sprintf(totalPath, "%s/%s/%s/%s", statsFolder, temp, PATH_QUERIES, key);
    e = initEntry(fopen(totalPath, "a"));
    g_hash_table_insert(s->filesTable, key, e);

    key = my_strdup("query9");
    sprintf(totalPath, "%s/%s/%s/%s", statsFolder, temp, PATH_QUERIES, key);
    e = initEntry(fopen(totalPath, "a"));
    g_hash_table_insert(s->filesTable, key, e);

    key = my_strdup("query10");
    sprintf(totalPath, "%s/%s/%s/%s", statsFolder, temp, PATH_QUERIES, key);
    e = initEntry(fopen(totalPath, "a"));
    g_hash_table_insert(s->filesTable, key, e);

    key = my_strdup("query11");
    sprintf(totalPath, "%s/%s/%s/%s", statsFolder, temp, PATH_QUERIES, key);
    e = initEntry(fopen(totalPath, "a"));
    g_hash_table_insert(s->filesTable, key, e);

    key = my_strdup("query11_loadTree1");
    sprintf(totalPath, "%s/%s/%s/%s/%s", statsFolder, temp, PATH_QUERIES, "Query11", key);
    e = initEntry(fopen(totalPath, "a"));
    g_hash_table_insert(s->filesTable, key, e);

    key = my_strdup("query11_loadTree2");
    sprintf(totalPath, "%s/%s/%s/%s/%s", statsFolder, temp, PATH_QUERIES, "Query11", key);
    e = initEntry(fopen(totalPath, "a"));
    g_hash_table_insert(s->filesTable, key, e);

    key = my_strdup("query11_loadTree3");
    sprintf(totalPath, "%s/%s/%s/%s/%s", statsFolder, temp, PATH_QUERIES, "Query11", key);
    e = initEntry(fopen(totalPath, "a"));
    g_hash_table_insert(s->filesTable, key, e);

    key = my_strdup("query11_treeToHeap");
    sprintf(totalPath, "%s/%s/%s/%s/%s", statsFolder, temp, PATH_QUERIES, "Query11", key);
    e = initEntry(fopen(totalPath, "a"));
    g_hash_table_insert(s->filesTable, key, e);

    key = my_strdup("query12");
    sprintf(totalPath, "%s/%s/%s/%s", statsFolder, temp, PATH_QUERIES, key);
    e = initEntry(fopen(totalPath, "a"));
    g_hash_table_insert(s->filesTable, key, e);

    free(totalPath);
    free(temp);

    return s;
}

void destroy_stats(Stats s){
    g_hash_table_destroy(s->filesTable);
    free(s);
}

void start_timing(Stats s, const char* key){
    if(!s->active){
        Entry e = g_hash_table_lookup(s->filesTable, key);
        gettimeofday(&e->t1, NULL);
    }
}

void agreggate_timing(Stats s, const char* key){
    if(!s->active){
        Entry e = g_hash_table_lookup(s->filesTable, key);

        gettimeofday(&e->t2, NULL);
        long seconds = (e->t2.tv_sec - e->t1.tv_sec);
        e->total_time_spent += ((seconds * 1000000) + e->t2.tv_usec) - (e->t1.tv_usec);
        gettimeofday(&e->t1, NULL);
    }
}

void finish_timing(Stats s, const char* key){
    if(!s->active){
        Entry e = g_hash_table_lookup(s->filesTable, key);

        gettimeofday(&e->t2, NULL);
        long seconds = (e->t2.tv_sec - e->t1.tv_sec);
        e->total_time_spent += ((seconds * 1000000) + e->t2.tv_usec) - (e->t1.tv_usec);

        if(e->f) {
            double seconds = (double) e->total_time_spent / 1000000;
            if(!e->markFile) fprintf(e->f, "----------------------------------------------------------------------------\n");
            if(e->commentary) fprintf(e->f, "%s --> %lf\n", e->commentary, seconds);
            else fprintf(e->f, "%lf\n", seconds);
        }
    }
}
