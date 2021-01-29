#ifndef HASH_OPEN_ADRESSING
#define HASH_OPEN_ADRESSING

#define HASHSIZE 31
#define EMPTY    ""
#define DELETED  "-"

#define LP       1
#define QP       -1

typedef char KeyType[9];
typedef void *Info;

typedef struct entry {
	KeyType key;
	Info info;
} Entry, HashTableOA[HASHSIZE];

int hashOA (KeyType key);

void initTable (HashTableOA h);

void clearTable (HashTableOA h);

int findProbeQP (HashTableOA h, KeyType k);

int findProbeLP (HashTableOA h, KeyType k);

void insertTable (HashTableOA h, KeyType k, Info i, int probeType);

void deleteTable (HashTableOA h, KeyType k, int probeType);

int lookupTable (HashTableOA h, KeyType k, int probeType);

#endif
