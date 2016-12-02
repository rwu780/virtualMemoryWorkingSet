#include <stdio.h>
#include <stdlib.h>

#define SIZE 30
#define WORDSIZE 4

struct Page {
	unsigned int pn;
	int *addr;
	struct Page *next;
};

struct HashItem {
	unsigned int key;
	struct Page *page;
};

unsigned int hash(unsigned int key);

struct HashItem *search(unsigned int key);

void insert(unsigned int key, struct Page* page);

void init(int psize, int winsize);

void put(unsigned int address, unsigned int value);

unsigned int get(unsigned int address);

void done();

void checkTotalReference(unsigned int key);

void checkWindowReference(unsigned int key);

void addToResult();