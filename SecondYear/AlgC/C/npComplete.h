#ifndef NP_COMPLETE
#define NP_COMPLETE

#include <stdio.h>
#include <stdlib.h>

int subSetZero (int num[], int sub[], int N);

int subSetSumRec (int num[], int sub[], int N, int k);

int subSetSum (int num[], int N);

int subSetHalfTest (int num[], int sub[], int N, int halfNum);

int subSetHalfRec (int num[], int sub[], int halfNum, int N, int k);

int subSetHalf (int num[], int N);

#endif
