#include "simulator.h"
#define SIZE 30

//Hash Table
struct HashItem* hashArray[SIZE];

//total reference
int *TOTAL_REFERENCE;
unsigned int TOTAL_SIZE;
unsigned int TOTAL_INDEX;

//Window size and reference
int *WINDOW_REFERENCE;
unsigned int WINSIZE;
unsigned int WINDOW_INDEX;

//record the number of working set for window
int *RESULT;
unsigned int RESULT_SIZE;
unsigned int RESULT_INDEX;

unsigned int PSIZE;
unsigned int COUNT;


void init(int psize, int winsize) {
	PSIZE = psize;
	WINSIZE = winsize;
	TOTAL_SIZE = WINSIZE;

	COUNT = 0;
	RESULT_SIZE = 10;
	
	WINDOW_INDEX = 0;
	TOTAL_INDEX = 0;
	RESULT_INDEX = 0;

	resetReferenceArrays(WINSIZE, TOTAL_SIZE);
	resetResultArray(RESULT_SIZE);
}

void put(unsigned int address, unsigned int value) {
	unsigned int hex = PSIZE-1 + 0x00;
	unsigned int offset = address & hex;
	unsigned int key = address/PSIZE;


	struct HashItem* item = search(key);
	if(item == NULL) {
		struct Page *page = (struct Page*) malloc(sizeof(struct Page));
		page->pn = key;
		page->addr = (int*) malloc(WORDSIZE * PSIZE);
		page->next = NULL;
		*(page->addr + offset) = value;
		insert(key, page);
		TOTAL_REFERENCE[TOTAL_INDEX++] = page->pn;
		WINDOW_REFERENCE[WINDOW_INDEX++] = page->pn;
	} else {
		struct Page *page = item->page;
		while(page->pn != key && page->next != NULL){
			page = page->next;
		}
		if(page->next == NULL && page->pn != key){
			struct Page *newPage = (struct Page*) malloc(sizeof(struct Page));
			newPage->pn = key;
			newPage->addr = (int*) malloc(WORDSIZE * PSIZE);
			newPage->next = NULL;
			*(newPage->addr + offset) = value;
			page->next = newPage;
			TOTAL_REFERENCE[TOTAL_INDEX++] = newPage->pn;
			WINDOW_REFERENCE[WINDOW_INDEX++] = newPage->pn;
		}
		else{
			int* addr = item->page->addr;
			*(addr + offset) = value;
			checkTotalReference(item->page->pn);
			checkWindowReference(item->page->pn);
		}
	}
	if(++COUNT % WINSIZE == 0) {
		addToResult();
		WINDOW_INDEX = 0;
		int i;
		for(i = 0; i < WINSIZE; i++) WINDOW_REFERENCE[i] = -1;		
	}
}

unsigned int get(unsigned int address) {
	unsigned int hex = PSIZE-1 + 0x00;
	unsigned int offset = address & hex;
	unsigned int key = address/PSIZE;

	struct HashItem* item = search(key);

	checkTotalReference(item->page->pn);
	checkWindowReference(item->page->pn);
	if(++COUNT % WINSIZE == 0) {
		addToResult();
		WINDOW_INDEX = 0;
		int i;
		for(i = 0; i < WINSIZE; i++) WINDOW_REFERENCE[i] = -1;		
	}
	return *(item->page->addr + offset);
}

void done() {
	addToResult();
	int i;
	for(i = 0; i < RESULT_SIZE; i++) {
		if(RESULT[i] != -1) {
			printf("Window %d has WSS: %d\n", i+1, RESULT[i]);
		}
	}


	printf("Total page referenced: %d\n", TOTAL_INDEX);
	printf("Average: %f\n", TOTAL_INDEX * 1.0 / (COUNT * 1.0));
	printf("COUNT: %d\n", COUNT);

	free(WINDOW_REFERENCE);
	free(TOTAL_REFERENCE);
	free(RESULT);
	for(i=0;i<SIZE;i++){
		struct HashItem* item = search(i);
		if(i != NULL){
			free(item);
		}
	}
	//free(hashArray);
	exit(0);
}

void checkTotalReference(unsigned int key) {
	int i;
	for(i = 0; i < TOTAL_SIZE; i++) {
		if(TOTAL_REFERENCE[i] == key) {
			return;
		} else if (TOTAL_REFERENCE[i] == -1) {
			TOTAL_REFERENCE[i] = key;
			TOTAL_INDEX++;
			return;
		}
	}
	TOTAL_SIZE *= 2;
	int *total = (int*) malloc(sizeof(int) * TOTAL_SIZE);
	for(i = 0; i < TOTAL_SIZE; i++) {
		if(i >= TOTAL_INDEX) {
			total[i] = -1;
		} else {
			total[i] = TOTAL_REFERENCE[i];
		}
	}
	free(TOTAL_REFERENCE);
	TOTAL_REFERENCE = total;
}

void checkWindowReference(unsigned int key) {
	int i;
	for(i = 0; i < TOTAL_SIZE; i++) {
		if(WINDOW_REFERENCE[i] == key) {
			return;
		} else if (WINDOW_REFERENCE[i] == -1) {
			WINDOW_REFERENCE[i] = key;
			WINDOW_INDEX++;
			return;
		}
	}
}

void addToResult(){
	if(RESULT_INDEX == RESULT_SIZE) {
		RESULT_SIZE *= 2;
		unsigned int *result = (unsigned int*) malloc(sizeof(unsigned int) * RESULT_SIZE);
		int i;
		for(i = 0; i < RESULT_SIZE; i++) {
			if(i >= RESULT_INDEX) {
				result[i] = -1;
			} else {
				result[i] = RESULT[i];
			}
		}
		RESULT = result;
	}
	RESULT[RESULT_INDEX++] = WINDOW_INDEX;
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

void resetReferenceArrays(int winsize, int totalSize){
	WINDOW_REFERENCE = (int*) malloc(sizeof(int) * winsize);
	TOTAL_REFERENCE = (int*) malloc(sizeof(int) * totalSize);

	int i;
	for(i = 0; i<winsize; i++){
		WINDOW_REFERENCE[i] = -1;
		TOTAL_REFERENCE[i] = -1;
	}
}
void resetResultArray(int resultSize){
	RESULT = (unsigned int*) malloc(sizeof(unsigned int) * resultSize);
	int i;
	for(i = 0; i<resultSize; i++){
		RESULT[i] = -1;
	}
}
