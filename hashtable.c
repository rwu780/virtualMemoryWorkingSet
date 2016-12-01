#include "hashtable.h"
#include <stdio.h>

struct HashItem* hashArray[SIZE];

unsigned int hash(unsigned int key) {
	return key % SIZE;
}

struct HashItem *search(unsigned int key) {
	unsigned int hashIndex = hash(key);
	while(hashArray[hashIndex] != NULL) {
		if(hashArray[hashIndex]->key == key) {
			return hashArray[hashIndex];
		}
		hashIndex++;
		hashIndex %= SIZE;
	}
	return NULL;
}

void insert(unsigned int key, struct Page* page) {
	struct HashItem *item = (struct HashItem*) malloc(sizeof(struct HashItem));
	item->key = key;
	item->page = page;

	int hashIndex = hash(key);

	while(hashArray[hashIndex] != NULL && hashArray[hashIndex]->key != key) {
		hashIndex++;
		hashIndex %= SIZE;
	}
	hashArray[hashIndex] = item;
}

