#ifndef FIND_AND_UNION
#define FIND_AND_UNION

#define INFINITY 9999
#define NW 9

typedef struct sedge {
	int o, d;
	int cost;
} EDGE;

void initA (int c[]);

int findA (int c[], int v);

int unionA (int c[], int id1, int id2);

void initF (int c[]);

int findF (int c[], int v);

int unionF (int c[], int id1, int id2);

void pushRight (EDGE e[], int start, int used);

void insert (EDGE e[], EDGE new, int used);

#endif
