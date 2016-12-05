#include "simulator.h"
#define SIZE 30

//Hash Array
struct HashItem* hashArray[SIZE];

//total reference
int *TOTAL_REFERENCE;
unsigned int TOTAL_SIZE;
unsigned int TOTAL_INDEX;

//Window set and reference
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
	RESULT_SIZE = SIZE;
	
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
	/*
	If item is null
	create a new page, create a hash map, and insert to hasharray
	*/
	if (item == NULL){
		struct Page *page = (struct Page*) malloc(sizeof(struct Page));
		page->pn = key;
		page->addr = (int*) malloc(WORDSIZE * PSIZE);
		page->next = NULL;
		*(page->addr + offset) = value;
		insert(key, page);
		TOTAL_REFERENCE[TOTAL_INDEX++] = page->pn;
		WINDOW_REFERENCE[WINDOW_INDEX++] = page->pn;
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
			checkWindowReference(page->pn);
		}
		else if(page->next == NULL && page->pn != key){
			struct Page *newPage = (struct Page*) malloc(sizeof(struct Page));
			newPage->pn = key;
			newPage->addr = (int*) malloc(WORDSIZE * PSIZE);
			newPage->next = NULL;
			*(newPage->addr + offset) = value;
			page->next = newPage;
			TOTAL_REFERENCE[TOTAL_INDEX++] = newPage->pn;
			WINDOW_REFERENCE[WINDOW_INDEX++] = newPage->pn;
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
	struct Page *page = item->page;
	while(page->pn != key && page->next != NULL){
		page = page->next;
	}

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

void done(char *sort) {
	addToResult();

	char message[100];
	char FILENAME[100];
	sprintf(FILENAME, "pSize%d,wSize%d,%s.csv",PSIZE, WINSIZE, sort);

	FILE *file;

	if((file = fopen(FILENAME, "a+"))){
	 	//Do nothing
	}
	else{
		file = fopen(FILENAME, "w+");
	}

	int i;
	fprintf(file, "Page:%d\n", PSIZE);
	fprintf(file, "Win:%d\n", WINSIZE);
	for(i = 0; i < RESULT_SIZE; i++) {
		if(RESULT[i] != -1) {
			fprintf(file, "%d\n",RESULT[i]);
			printf("Window %d has WSS: %d\n", i+1, RESULT[i]);
		}
	}

	printf("Total page referenced: %d\n", TOTAL_INDEX);
	printf("Average: %f\n", TOTAL_INDEX * 1.0 / (RESULT_SIZE * 1.0));



	free(WINDOW_REFERENCE);
	free(TOTAL_REFERENCE);
	free(RESULT);
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
