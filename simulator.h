#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"

#define SIZE 30

void init(int psize, int winsize);

void put(unsigned int address, int value);

int get(unsigned int address);

void done();

void checkTotalReference(unsigned int key);

void checkWindowReference(unsigned int key);

void addToResult();