#ifndef __UTILITIES__
#define __UTILITIES__

#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <limits.h>
#include <errno.h>
#include <time.h>

#define NAME_SIZE   128
#define PUT_LINE    write(1, "\n", 1);
#define CACHE       4096

#define ARTIGOS "Files/artigos"
#define STRINGS "Files/strings"
#define STOCKS  "Files/stocks"
#define VENDAS  "Files/vendas"
#define ERRORS  "Files/errorLogging"

struct venda {
	int32_t code;
	int32_t quantity;
	float total;
};

struct artigo {
	char name[NAME_SIZE];
	float price;
	long long code;
};

struct entry{
	float price;
	float total;
	int32_t artCode;
	int32_t quantity;
	int flag;
};

typedef struct hashTable{
	struct entry *entries;
	int size;
} HashTable;

typedef struct circularArray{
	struct entry *entries;
	int size;
	int currentPos;
} CircularArray;

char** words(const char* string, int *size);

int mypopen(char* command, char* type);

ssize_t readln(int fildes, void *buf, size_t bytes);

CircularArray* initCA(int size);

void insertCA(CircularArray *c, int artCode, float price);

void insertCAintoAG(CircularArray *c, int artCode, float totalSold, int artQuantity);

float findCA(CircularArray *c, int artCode);

void freeCircularArray(CircularArray *c);

void flushCircularArray(CircularArray *c, int control);

char* timeToString(void);

int getGlobals(long long *code, long long *next);

void readVenda(struct venda v);

struct artigo readArtigo(long long artCode);

void display(struct artigo art);

pid_t getPidFromPath(char* path);

void getServerPath(char* serverPath, int serverPid);

#endif