#include <stddef.h>
#include <stdlib.h>

#define WORDSIZE 4
#define SIZE 30

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