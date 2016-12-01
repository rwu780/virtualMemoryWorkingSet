#include "simulator.h"

unsigned int PSIZE;
unsigned int COUNT;
unsigned int WINSIZE;
int *TOTAL_REFERENCE;
unsigned int TOTAL_SIZE;
unsigned int TOTAL_INDEX;
int *WINDOW_REFERENCE;
unsigned int WINDOW_INDEX;
unsigned int *RESULT;
unsigned int RESULT_SIZE;
unsigned int RESULT_INDEX;

void init(int psize, int winsize) {
	PSIZE = psize;
	WINSIZE = winsize;
	COUNT = 0;
	TOTAL_SIZE = WINSIZE;
	RESULT_SIZE = 10;
	WINDOW_INDEX = 0;
	TOTAL_INDEX = 0;
	RESULT_INDEX = 0;
	WINDOW_REFERENCE = (int*) malloc(sizeof(int) * WINSIZE);
	TOTAL_REFERENCE = (int*) malloc(sizeof(int) * TOTAL_SIZE);
	RESULT = (unsigned int*) malloc(sizeof(unsigned int) * RESULT_SIZE);
	int i;
	for(i = 0; i < WINSIZE; i++) WINDOW_REFERENCE[i] = -1;
	for(i = 0; i < TOTAL_SIZE; i++) TOTAL_REFERENCE[i] = -1;
	for(i = 0; i < RESULT_SIZE; i++) RESULT[i] = -1;
}

void put(unsigned int address, int value) {
	unsigned int offset = address & 0x7F;
	unsigned int key = address >> 7;

	//printf("In Put key:%d offset: %d\n", key, offset);

	unsigned int refrencedPage;
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
		int* addr = item->page->addr;
		*(addr + offset) = value;
		checkTotalReference(item->page->pn);
		checkWindowReference(item->page->pn);
	}
	if(++COUNT % WINSIZE == 0) {
		addToResult();
		WINDOW_INDEX = 0;
		int i;
		for(i = 0; i < WINSIZE; i++) WINDOW_REFERENCE[i] = -1;		
	}
}

int get(unsigned int address) {
	unsigned int offset = address & 0x7F;
	unsigned int key = address >> 7;

	//printf("In Get key:%d offset: %d\n", key, offset);

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
	//RESULT[RESULT_INDEX] = WINDOW_INDEX;
	int i;
	for(i = 0; i < RESULT_SIZE; i++) {
		if(RESULT[i] != -1) {
			printf("Window %d has WSS: %d\n", i+1, RESULT[i]);
		}
	}

	printf("Total page referenced: %d\n", TOTAL_INDEX);
	printf("Average: %f\n", TOTAL_INDEX * 1.0 / (COUNT * 1.0));

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
		free(RESULT);
		RESULT = result;
	}
	RESULT[RESULT_INDEX++] = WINDOW_INDEX;
}

