#include "hashtable.h"
#include <stdio.h>

struct HashItem* hashArray[SIZE];

unsigned int hash(unsigned int key) {
	return key % SIZE;
}

struct HashItem *search(unsigned int key) {
	//printf("search key\n");
	unsigned int hashIndex = hash(key);
	return hashArray[hashIndex];
	// while(hashArray[hashIndex] != NULL) {
	// 	printf("key %d\n", key);
	// 	if(hashArray[hashIndex]->key == key) {
	// 		return hashArray[hashIndex];
	// 	}
	// 	hashIndex++;
	// 	hashIndex %= SIZE;
	// }
	// return NULL;
}

void insert(unsigned int key, struct Page* page) {
	struct HashItem *item = (struct HashItem*) malloc(sizeof(struct HashItem));
	item->key = key;
	item->page = page;

	int hashIndex = hash(key);
	hashArray[hashIndex] = item;
}

