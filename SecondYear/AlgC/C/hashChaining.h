#ifndef HASH_CHAINING
#define HASH_CHAINING

#define HASHSIZE_C 31

typedef char KeyType[9]; 
typedef void *Info;

typedef struct entryc {
	KeyType key;
	Info info;
	struct entryc *next;
} EntryC, *HashTableC[HASHSIZE_C];

int hashChain (KeyType key);

void initChain (HashTableC h);

void clearChain (HashTableC h);

void insertChain (HashTableC h, KeyType k, Info i);

void deleteChain (HashTableC h, KeyType k);

EntryC * lookupChain (HashTableC h, KeyType k);

#endif
