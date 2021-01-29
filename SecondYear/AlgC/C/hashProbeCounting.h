#ifndef HASH_PROBE_COUNTING
#define HASH_PROBE_COUNTING

#define HASHSIZE_PC 100
#define FREE     -1

typedef struct bucket {
	int probeC;
	int key;
	int value;
} Bucket;

typedef Bucket HashTable[HASHSIZE_PC];

void init (HashTable h);

int hash (int key);

int update (HashTable h, int key, int value);

int lookup (HashTable h, int key, int *value);

int compress (HashTable h, int free);

int removeKey (HashTable h, int key);

int repetidos (int v[], int N);

int count (HashTable h, int i);

int last (HashTable h, int i);

int lastP (HashTable h, int i);

int GUIhashTables (void);

#endif
