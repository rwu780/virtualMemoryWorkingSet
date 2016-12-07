#include <stdio.h>
#include <stdlib.h>

/*Link list for hash table items*/
struct Page {
	unsigned int pn;
	int *addr;
	struct Page *next;
};

/*Hash table item*/
struct HashItem {
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

void checkWsReference(unsigned int key);

void addToResult();

void resetWsReference(int wSize);
void resetTotalReference(int totalSize);
void resetResultArray(int resultSize);



