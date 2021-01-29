#ifndef __STATS__
#define __STATS__

typedef struct stats *Stats;

/* Inits the structure and gets it to point to a stats directory */
/* If active != 0 then there no timings will happen even if functions are called */
/* If createDirs = 0 active the directories will be created if they don't exist, otherwise nothing will happen */
Stats init_stats(char* statsFolder, char* resultsFolder, int createDirs, int active);

/* Free's all memory and closes all files opened */
void destroy_stats(Stats s);

/* Starts the timer for the given key */
void start_timing(Stats s, const char* key);

/* Agreggates the time into a given key */
void agreggate_timing(Stats s, const char* key);

/* Finishes timing and writes the result to the file associated with a key */
void finish_timing(Stats s, const char* key);

#endif
