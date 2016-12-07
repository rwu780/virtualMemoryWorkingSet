#include "simulator.h"

#define SIZE 30
#define WORDSIZE 4


//Hash Array
struct HashItem* hashArray[SIZE];

//total reference
int *totalReferenced;
unsigned int tSize;
unsigned int tIndex;

//record working set per window
int *wsReference;
unsigned int wSize;
unsigned int wIndex;

//record the number of working set for window
int *windowReference;
unsigned int windowSize;
unsigned int windowIndex;

unsigned int pageSize;
unsigned int count;


void init(int psize, int winSize) {
	pageSize = psize;
	//pass to global, so we can referenced later
	wSize = (unsigned int)winSize;
	tSize = wSize;

	count = 0;
	windowSize = SIZE;
	
	wIndex = 0;
	tIndex = 0;
	windowIndex = 0;

	resetWsReference(wSize);
	resetTotalReference(tSize);
	resetResultArray(windowSize);
}

void put(unsigned int address, unsigned int value) {
	unsigned int hex = pageSize-1 + 0x00;
	unsigned int offset = address & hex;
	unsigned int key = address/pageSize;

	struct HashItem* item = search(key);
	/*
	If item is null
	create a new page, create a hash map, and insert to hasharray
	*/
	if (item == NULL){
		struct Page *page = (struct Page*) malloc(sizeof(struct Page));
		page->pn = key;
		page->addr = (int*) malloc(WORDSIZE * pageSize);
		page->next = NULL;
		*(page->addr + offset) = value;
		insert(key, page);
		totalReferenced[tIndex++] = page->pn;
		wsReference[wIndex++] = page->pn;
	}
	
	if(item != NULL){
		struct Page *page = item->page;
		while(page->pn != key && page->next != NULL){
			page = page->next;
		}
		//Only two situtation
		//One: a key is found
		//Two: no key is found
		if(page->pn == key){
			int* addr = item->page->addr;
			*(addr + offset) = value;
			checkTotalReference(page->pn);
			checkWsReference(page->pn);
		}
		else if(page->next == NULL && page->pn != key){
			struct Page *newPage = (struct Page*) malloc(sizeof(struct Page));
			newPage->pn = key;
			newPage->addr = (int*) malloc(WORDSIZE * pageSize);
			newPage->next = NULL;
			*(newPage->addr + offset) = value;
			page->next = newPage;
			totalReferenced[tIndex++] = newPage->pn;
			wsReference[wIndex++] = newPage->pn;
		}

	}

	if(++count % wSize == 0) {
		addToResult();
		wIndex = 0;
		free(wsReference);
		resetWsReference(wSize);

		//int i;
		//for(i = 0; i < wSize; i++) wsReference[i] = -1;		
	}
}

unsigned int get(unsigned int address) {
	unsigned int hex = pageSize-1 + 0x00;
	unsigned int offset = address & hex;
	unsigned int key = address/pageSize;

	struct HashItem* item = search(key);
	struct Page *page = item->page;
	while(page->pn != key && page->next != NULL){
		page = page->next;
	}

	checkTotalReference(item->page->pn);
	checkWsReference(item->page->pn);
	if(++count % wSize == 0) {
		addToResult();
		wIndex = 0;
		free(wsReference);
		resetWsReference(wSize);

		// int i;
		// for(i = 0; i < wSize; i++) wsReference[i] = -1;		
	}
	return *(item->page->addr + offset);
}

void done() {
	addToResult();

	int i;
	for(i = 0; i < windowSize; i++) {
		if(windowReference[i] != -1) {
			printf("Window %d has WSS: %d\n", i+1, windowReference[i]);
		}
	}

	printf("Total page referenced: %d\n", tIndex);
	printf("Average: %f\n", tIndex * 1.0 / (windowSize * 1.0));



	free(wsReference);
	free(totalReferenced);
	free(windowReference);
	for(i=0;i<SIZE;i++){
		struct HashItem* item = search(i);
		if(item != NULL){
			free(item);
		}
	}
	exit(0);
}

void checkTotalReference(unsigned int key) {
	int i;
	for(i = 0; i < tSize; i++) {
		if(totalReferenced[i] == key) {
			return;
		} else if (totalReferenced[i] == -1) {
			totalReferenced[i] = key;
			tIndex++;
			return;
		}
	}
	tSize *= 2;
	int *total = (int*) malloc(sizeof(int) * tSize);
	for(i = 0; i < tSize; i++) {
		if(i >= tIndex) {
			total[i] = -1;
		} else {
			total[i] = totalReferenced[i];
		}
	}
	free(totalReferenced);
	totalReferenced = total;
}


void checkWsReference(unsigned int key) {
	int i;
	for(i = 0; i < tSize; i++) {
		if(wsReference[i] == key) {
			return;
		} else if (wsReference[i] == -1) {
			wsReference[i] = key;
			wIndex++;
			return;
		}
	}
}

void addToResult(){
	if(windowIndex == windowSize) {
		windowSize *= 2;
		unsigned int *newResult = (unsigned int*) malloc(sizeof(unsigned int) * windowSize);
		int i;
		for(i = 0; i < windowSize; i++) {
			if(i >= windowIndex) {
				newResult[i] = -1;
			} else {
				newResult[i] = windowReference[i];
			}
		}
		windowReference = newResult;
	}
	windowReference[windowIndex++] = wIndex;
}


unsigned int hash(unsigned int key) {
	return key % SIZE;
}

struct HashItem *search(unsigned int key) {
	int hashIndex = hash(key);
	return hashArray[hashIndex];
}

void insert(unsigned int key, struct Page* page) {
	struct HashItem *item = (struct HashItem*) malloc(sizeof(struct HashItem));
	item->page = page;
	int hashIndex = hash(key);
	hashArray[hashIndex] = item;
}

void resetWsReference(int wSize){
	wsReference = (int*) malloc(sizeof(int) * wSize);
	int i;
	for(i = 0; i<wSize; i++){
		wsReference[i] = -1;
	}	
}
void resetTotalReference(int totalSize){
	totalReferenced = (int*) malloc(sizeof(int) * totalSize);
	int i;
	for(i = 0; i<totalSize; i++){
		totalReferenced[i] = -1;
	}

}
void resetResultArray(int windowSize){
	windowReference = (unsigned int*) malloc(sizeof(unsigned int) * windowSize);
	int i;
	for(i = 0; i<windowSize; i++){
		windowReference[i] = -1;
	}
}
